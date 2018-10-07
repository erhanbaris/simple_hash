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

#ifndef HASHSET_H
#define HASHSET_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* hash function */
    typedef size_t(*hash_func_t)(char*, size_t);

    struct hashset_st;
    typedef struct hashset_st *hashset_t;

    struct hashmap_st;
    typedef struct hashmap_st *hashmap_t;

    /*
     * HASHSET FUNCTIONS
     */

    /* create hashset instance */
    hashset_t hashset_create(void);

    /* destroy hashset instance */
    void hashset_destroy(hashset_t set);

    /* set hash function */
    void hashset_set_hash_function(hashset_t set, hash_func_t func);
    
    /* Just clear data but do not create anything*/
    void hashset_clean(hashset_t set);

    /* total items count */
    size_t hashset_num_items(hashset_t set);

    /* add item into the hashset.
     *
     * @note 0 and 1 is special values, meaning nil and deleted items. the
     *       function will return -1 indicating error.
     *
     * returns zero if the item already in the set and non-zero otherwise
     */
    int hashset_add(hashset_t set, char* key, size_t key_len);

    /* remove item from the hashset
     *
     * returns non-zero if the item was removed and zero if the item wasn't
     * exist
     */
    int hashset_remove(hashset_t set, char *key, size_t key_len);

    /* check if existence of the item
     *
     * returns non-zero if the item exists and zero otherwise
     */
    int hashset_is_member(hashset_t set, char* key, size_t key_len);

    /* create hashmap instance */
    hashmap_t hashmap_create(void);

    /*
     * HASHMAP FUNCTIONS
     */

    /* destroy hashmap instance */
    void hashmap_destroy(hashmap_t map);

    /* set hash function */
    void hashmap_set_hash_function(hashmap_t map, hash_func_t func);

    /* Just clear data but do not create anything*/
    void hashmap_clean(hashmap_t map);

    /* total items count */
    size_t hashmap_num_items(hashmap_t map);

    /* add item into the hashmap.
     *
     * @note 0 and 1 is special values, meaning nil and deleted items. the
     *       function will return -1 indicating error.
     *
     * returns zero if the item already in the set and non-zero otherwise
     */
    int hashmap_add(hashmap_t map, char* key, size_t key_len, void* data);

    /* get item from the hashmap.
     *
     * returns stored item
     */
    void* hashmap_get(hashmap_t map, char* key, size_t key_len);

    /* remove item from the hashmap
     *
     * returns non-zero if the item was removed and zero if the item wasn't
     * exist
     */
    int hashmap_remove(hashmap_t map, char* key, size_t key_len);

    /* check if existence of the item
     *
     * returns non-zero if the item exists and zero otherwise
     */
    int hashmap_is_member(hashmap_t map, char* key, size_t key_len);

#ifdef __cplusplus
}
#endif


#endif
