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

#ifndef SIMPLE_HASH_HPP
#define SIMPLE_HASH_HPP

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <simple_hash.h>

template <class T>
class SimpleHashMap
{
    public:
    SimpleHashMap()
    {
         map = hashmap_create();
    }

    ~SimpleHashMap()
    {
        hashmap_destroy(map);
        map = nullptr;
    }

    bool status()
    {
        return map != nullptr;
    }

    bool add(const char* key, T* data)
    {
        return add(key, strlen(key), data);
    }

    bool add(const char* key, size_t key_len, T* data)
    {
        return hashmap_add(map, const_cast<char*>(key), key_len, data);
    }

    bool remove(const char* key)
    {
        return remove(key, strlen(key));
    }

    bool remove(const char* key, size_t key_len)
    {
        return hashmap_remove(map, const_cast<char*>(key), key_len);
    }

    T* get(const char* key)
    {
        return (T*)get(key, strlen(key));
    }

    void* get(const char* key, size_t key_len)
    {
        return hashmap_get(map, const_cast<char*>(key), key_len);
    }

    bool exists(const char* key)
    {
        return exists(key, strlen(key));
    }

    bool exists(const char* key, size_t key_len)
    {
        return hashmap_is_member(map, const_cast<char*>(key), key_len);
    }

    void clean()
    {
        hashmap_clean(map);
    }

    void setHashFunction(hash_func_t func)
    {
        hashmap_set_hash_function(map, func);
    }

    size_t count()
    {
        return hashmap_num_items(map);
    }

    private:
    hashmap_t map;
};


class SimpleHashSet
{
    public:
    SimpleHashSet()
    {
         set = hashset_create();
    }

    ~SimpleHashSet()
    {
        hashset_destroy(set);
        set = nullptr;
    }

    bool status()
    {
        return set != nullptr;
    }

    bool add(const char* key)
    {
        return add(key, strlen(key));
    }

    bool add(const char* key, size_t key_len)
    {
        return hashset_add(set, const_cast<char*>(key), key_len);
    }

    bool remove(const char* key)
    {
        return remove(key, strlen(key));
    }

    bool remove(const char* key, size_t key_len)
    {
        return hashset_remove(set, const_cast<char*>(key), key_len);
    }

    bool exists(const char* key)
    {
        return exists(key, strlen(key));
    }

    bool exists(const char* key, size_t key_len)
    {
        return hashset_is_member(set, const_cast<char*>(key), key_len);
    }

    void clean()
    {
        hashset_clean(set);
    }

    void setHashFunction(hash_func_t func)
    {
        hashset_set_hash_function(set, func);
    }

    size_t count()
    {
        return hashset_num_items(set);
    }

    private:
    hashset_t set;
};

#endif
