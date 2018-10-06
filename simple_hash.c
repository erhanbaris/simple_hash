/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2018 Erhan BARIS (Ruslan Ognyanov Asenov)
 *     Copyright 2012 Couchbase, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "hashset.h"
#include <assert.h>


/* ############# DEFINITIONS ############# */

static const unsigned int prime_1 = 73;
static const unsigned int prime_2 = 5009;

struct hashmap_item_st {
    void* key;
    void* data;
    unsigned int hash;
};

struct hashmap_st {
    unsigned int nbits;
    unsigned int mask;

    unsigned int capacity;
    struct hashmap_item_st *items;
    unsigned int nitems;
    unsigned int n_deleted_items;

    hash_func_t hash_func;
};


struct hashset_item_st {
    unsigned int hash;
    void* item;
};

struct hashset_st {
    unsigned int nbits;
    unsigned int mask;

    unsigned int capacity;
    struct hashset_item_st *items;
    unsigned int nitems;
    unsigned int n_deleted_items;

    hash_func_t hash_func;
};

/* ############# HASHMAP ############# */

unsigned int hash_function(void* p, unsigned int len)
{
    unsigned int hash = 0;
    for (; *p; ++p)
        hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
}

hashmap_t hashmap_create()
{
    hashmap_t set = (hashmap_t)calloc(1, sizeof(struct hashmap_st));

    if (set == NULL) {
        return NULL;
    }
    
    set->hash_func = hash_function;
    set->nbits = 3;
    set->capacity = (unsigned int)(1 << set->nbits);
    set->mask = set->capacity - 1;
    set->items = (struct hashmap_item_st*)calloc(set->capacity, sizeof(struct hashmap_item_st));
    if (set->items == NULL) {
        hashmap_destroy(set);
        return NULL;
    }
    set->nitems = 0;
    set->n_deleted_items = 0;
    return set;
}

void hashmap_clean(hashmap_t set)
{
    set->nitems = 0;
    set->n_deleted_items = 0;

    unsigned int i = 0;
    while(i != set->capacity)
        set->items[i++].hash = 0;
}

unsigned int hashmap_num_items(hashmap_t set)
{
    return set->nitems;
}

void hashmap_destroy(hashmap_t set)
{
    if (set) {
        free(set->items);
    }
    free(set);
}

void hashmap_set_hash_function(hashmap_t set, hash_func_t func)
{
    set->hash_func = func;
}

static int hashmap_add_member(hashmap_t set, void *item, unsigned int hash, void* data)
{
    unsigned int index;

    if (hash < 2) {
        return -1;
    }

    index = set->mask & (prime_1 * hash);

    while (set->items[index].hash > 1) {
        if (set->items[index].hash == hash) {
            return 0;
        }
        else {
            /* search free slot */
            index = set->mask & (index + prime_2);
        }
    }

    ++set->nitems;
    if (set->items[index].hash == 1) {
        --set->n_deleted_items;
    }

    set->items[index].hash = hash;
    set->items[index].key = item;
    set->items[index].data = data;
    return 1;
}

static void map_maybe_rehash(hashmap_t set)
{
    struct hashmap_item_st *old_items;
    unsigned int old_capacity, index;


    if (set->nitems + set->n_deleted_items >= (double)set->capacity * 0.85) {
        old_items = set->items;
        old_capacity = set->capacity;
        ++set->nbits;
        set->capacity = (unsigned int)(1 << set->nbits);
        set->mask = set->capacity - 1;
        set->items = (struct hashmap_item_st*)calloc(set->capacity, sizeof(struct hashmap_item_st));
        set->nitems = 0;
        set->n_deleted_items = 0;
        assert(set->items);
        for (index = 0; index < old_capacity; ++index) {
            if (old_items[index].key)
                hashmap_add_member(set, old_items[index].key, old_items[index].hash, old_items[index].data);
        }
        free(old_items);
    }
}

int hashmap_add(hashmap_t set, void *item, unsigned int len, void* data)
{
    unsigned int hash = set->hash_func(item, len);
    int rv = hashmap_add_member(set, item, hash, data);
    map_maybe_rehash(set);
    return rv;
}

int hashmap_remove(hashmap_t set, void *item, unsigned int len)
{
    unsigned int hash = set->hash_func(item, len);
    unsigned int index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            set->items[index].hash = 1;
            --set->nitems;
            ++set->n_deleted_items;
            return 1;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return 0;
}

int hashmap_is_member(hashmap_t set, void *item, unsigned int len)
{
    unsigned int hash = set->hash_func(item, len);
    unsigned int index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            return 1;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return 0;
}

void* hashmap_get(hashmap_t set, void *item, unsigned int len)
{
    unsigned int hash = set->hash_func(item, len);
    unsigned int index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            return set->items[index].data;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return NULL;
}


/* ############# HASHSET ############# */

hashset_t hashset_create()
{
    hashset_t set = (hashset_t)calloc(1, sizeof(struct hashset_st));

    if (set == NULL) {
        return NULL;
    }
    set->nbits = 3;
    set->capacity = (unsigned int)(1 << set->nbits);
    set->mask = set->capacity - 1;
    set->items = (struct hashset_item_st*)calloc(set->capacity, sizeof(struct hashset_item_st));
    if (set->items == NULL) {
        hashset_destroy(set);
        return NULL;
    }
    set->nitems = 0;
    set->n_deleted_items = 0;
    return set;
}

void hashset_clean(hashset_t set)
{
    set->nitems = 0;
    set->n_deleted_items = 0;

    unsigned int i = 0;
    while(i != set->capacity)
        set->items[i++].hash = 0;
}

unsigned int hashset_num_items(hashset_t set)
{
    return set->nitems;
}

void hashset_destroy(hashset_t set)
{
    if (set) {
        free(set->items);
    }
    free(set);
}

void hashset_set_hash_function(hashset_t set, hash_func_t func)
{
    set->hash_func = func;
}

static int hashset_add_member(hashset_t set, void *item, unsigned int hash)
{
    unsigned int index;

    if (hash < 2) {
        return -1;
    }

    index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0 && set->items[index].hash != 1) {
        if (set->items[index].hash == hash) {
            return 0;
        }
        else {
            /* search free slot */
            index = set->mask & (index + prime_2);
        }
    }

    ++set->nitems;
    if (set->items[index].hash == 1) {
        --set->n_deleted_items;
    }

    set->items[index].hash = hash;
    set->items[index].item = item;
    return 1;
}

static void set_maybe_rehash(hashset_t set)
{
    struct hashset_item_st *old_items;
    unsigned int old_capacity, index;


    if (set->nitems + set->n_deleted_items >= (double)set->capacity * 0.85) {
        old_items = set->items;
        old_capacity = set->capacity;
        ++set->nbits;
        set->capacity = (unsigned int)(1 << set->nbits);
        set->mask = set->capacity - 1;
        set->items = (struct hashset_item_st*)calloc(set->capacity, sizeof(struct hashset_item_st));
        set->nitems = 0;
        set->n_deleted_items = 0;
        assert(set->items);
        for (index = 0; index < old_capacity; ++index) {
            hashset_add_member(set, old_items[index].item, old_items[index].hash);
        }
        free(old_items);
    }
}

int hashset_add(hashset_t set, void *item, unsigned int len)
{
    unsigned int hash = set->hash_func(item, len);
    int rv = hashset_add_member(set, item, hash);
    set_maybe_rehash(set);
    return rv;
}

int hashset_remove(hashset_t set, void *item, unsigned int len)
{
    unsigned int hash = set->hash_func(item, len);
    unsigned int index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            set->items[index].hash = 1;
            --set->nitems;
            ++set->n_deleted_items;
            return 1;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return 0;
}

int hashset_is_member(hashset_t set, void *item, unsigned int len)
{
    unsigned int hash = set->hash_func(item, len);
    unsigned int index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            return 1;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return 0;
}

