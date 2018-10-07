# simple_hash.c

This is very simple hashset and hashmap implementation. Project is based on [avsej's hashset.c project](https://github.com/avsej/hashset.c). There are some improvement on Avsej's version.

- Supports string as key
- Custom hash function added
- HashMap had added
- Performance improvement

Tested on MacOSX Clang 9.0. But as soon as posible I will test on Windows and Linux.

## Installing

Just import **simple_hash.c** and **simple_hash.h** files to your project and that is all you need. Please check examples.
If you want to use C++ wrapper also you need to add **simple_hash.hpp** file to your project.

## HashSet C Example

```c
    #include "simple_hash.h"

    unsigned int customHash(char* p, unsigned int key_len)
    {
        unsigned int hash = 0;
        for (; *p; ++p)
            hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }

    char *foo = "foo";
    char *missing = "missing";
    hashset_t set = hashset_create();

    if (set == NULL) {
        fprintf(stderr, "failed to create hashset instance\n");
        abort();
    }

    hashset_set_hash_function(set, customHash);
    hashset_add(set, foo, strlen(foo));
    assert(hashset_is_member(set, foo, strlen(foo)) == 1);
    assert(hashset_is_member(set, missing, strlen(missing)) == 0);
    hashset_destroy(set);

```

## HashSet C++ Example

```c++
    #include "simple_hash.hpp"

    unsigned int customHash(char* p, unsigned int key_len)
    {
        unsigned int hash = 0;
        for (; *p; ++p)
            hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }

    SimpleHashSet set;
    char *foo = "foo";
    char *missing = "missing";
    if (!set.status())
    {
        fprintf(stderr, "failed to create hashset instance\n");
        abort();
    }

    set.setHashFunction(customHash);
    set.add(foo);
    assert(set.exists(foo) == 1);
    assert(set.exists(missing) == 0);

```

## HashMap C Example 

```c
    #include "simple_hash.h"

    unsigned int customHash(char* p, unsigned int key_len)
    {
        unsigned int hash = 0;
        for (; *p; ++p)
            hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }

    char *foo = "foo";
    char *bar = "bar";
    char *missing = "missing";
    hashmap_t map = hashmap_create();
    
    if (map == NULL) {
        fprintf(stderr, "failed to create hashmap instance\n");
        abort();
    }
    
    hashmap_set_hash_function(map, customHash);
    hashmap_add(map, foo, strlen(foo), bar);
    assert(hashmap_is_member(map, foo, strlen(foo)) == 1);
    assert(hashmap_is_member(map, missing, strlen(missing)) == 0);
    assert(strcmp((char*)hashmap_get(map, foo, strlen(foo)), bar) == 0);
    hashmap_destroy(map);

```

## HashMap C++ Example

```c++
    #include "simple_hash.hpp"

    unsigned int customHash(char* p, unsigned int key_len)
    {
        unsigned int hash = 0;
        for (; *p; ++p)
            hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }

    SimpleHashMap<char> map;
    char *foo = "foo";
    char *bar = "bar";
    char *missing = "missing";
    if (!map.status())
    {
        fprintf(stderr, "failed to create hashset instance\n");
        abort();
    }

    map.setHashFunction(customHash);
    map.add(foo, bar);
    assert(map.exists(foo) == 1);
    assert(strcmp(map.get(foo), bar) == 0);

```


### Note for C++

As you can see map is only works with pointer. You have to define value type as type name not with pointer symbol at definition.
SimpleHashMap< char > means that you want to store char**. So you should be careful when you want to use **SimpleHashMap**.

## Todos

 - [ ] Support integer type for **key**
 - [x] Add C++ wrapper
 - [ ] Create unit tests
 - [ ] Test on large dataset
 - [ ] Create performance comperations charts
 - [ ] Create some example project
 - [ ] Add CMake
