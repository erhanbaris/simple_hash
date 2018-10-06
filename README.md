# simple_hash.c

This project is based on [avsej's hashset.c project](https://github.com/avsej/hashset.c).There are some improvement on Avsej's version.
- Some codes changed for supporting array and string.
- Custom hash function added.
- HashMap had added.
- Some codes changed for improving performance.

Tested on MacOSX Clang 9.0. But as soon as posible I willl test on Windows and Linux.

## Installing

Just import **simple_hash.c** and **simple_hash.h** files to your project and that is all you need. Please check examples.

## Todo

 - [ ] Support integer type for key.
 - [ ] Add C++ wrapper.
 - [ ] Create unit tests.
 - [ ] Test on large dataset.
 - [ ] Create performance comperations charts.
 - [ ] Create some example project.


## HashSet Example

    #include "simple_hash.h"

    size_t customHash(void* s, size_t len)
    {
        char * p = (char*)s;
        size_t hash = 0;
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

    hashset_set_hash_function(map, customHash);
    hashset_add(set, foo, 3);
    assert(hashset_is_member(set, foo, 3) == 1);
    assert(hashset_is_member(set, missing, 3) == 0);


## HashSet Example

    #include "simple_hash.h"

    size_t customHash(void* s, size_t len)
    {
        char * p = (char*)s;
        size_t hash = 0;
        for (; *p; ++p)
            hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }

    char *foo = "foo";
    char *bar = "bar";
    char *missing = "missing";
    hashmap_t set = hashmap_create();

    if (set == NULL) {
    	fprintf(stderr, "failed to create hashmap instance\n");
    	abort();
    }

    hashmap_set_hash_function(map, customHash);
    hashmap_add(set, foo, 3, bar);
    assert(hashmap_is_member(set, foo, 3) == 1);
    assert(hashmap_is_member(set, missing, 3) == 0);
    assert(strcmp(hashmap_get(set, missing, 3), bar) == 0);
