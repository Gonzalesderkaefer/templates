// Libraries
#include<stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// # How to use this library
/// NOTE: for the function documentations go to the section 'Htab Documentation'
///
/// ## Naming convention
///
/// Function names for this type have the following stucture:
/// htab_<k>_<v>_<action>, where <k> and <v> is what the constants 'snake_k'
/// and 'snake_v' have been set to down below.
/// For example, to initialize a htab:
/// htab_int_double_init
///
/// This type's name has the following structure:
/// Htab<K><V>, where <K> and <V> are what the constants 'pascal_k' and 'pascal_v' 
/// have been set to down below. If you had a hashtable of ints(key) and double(value):
/// HtabIntDouble
///
/// ## Aditional remarks
///
/// If the type is a pointer you cannot use the asterisk (*). It is recommended
/// for the pascal and snake constants to replace the asterisk with 'ptr'.
///
/// ## Include guards
///
/// You have to add the ifdef gurads yourself because
/// the type of the hashtable is defined by you
///
/// Something like HTAB_K_T_H is recommendend where K and H are the
/// snake_k and snake_v below but captilaized.
/// If you had a hashtable of ints(key) and double(value) you would define
/// HTAB_INT_DOUBLE_H
///
///          v v   Change these to your types
#ifndef HTAB_K_V_H
#define HTAB_K_V_H

// Keys
// Edit these
#define k int // The actual typename
#define snake_k int // Used for functions
#define pascal_k Int // Used for the vec typename

// Values
// Edit these
#define v int // The actual typename
#define snake_v int // Used for functions
#define pascal_v Int // Used for the vec typename

// Macros for htab
#define _Htab(k, v) Htab##k##v
#define __htab(k, v) htab_##k##_##v
#define HtabOf(k, v) _Htab(k, v)
#define htabof(k, v) __htab(k, v)
#define Htab HtabOf(pascal_k, pascal_v)
#define _htab htabof(snake_k, snake_v)


// Macros for pair
#define _Pair(k, v) Pair##k##v
#define __pair(k, v) pair_##k##_##v
#define PairOf(k, v) _Pair(k, v)
#define pairof(k, v) __pair(k, v)
#define Pair PairOf(pascal_k, pascal_v)
#define _pair pairof(snake_k, snake_v)



// Used to create function names
#define concat2(a, b) a##b
#define comp2(a, b) concat2(a, b)


// Initial size of the hashtable
#define HTAB_INIT_SIZE 4



#ifndef JAZZY_ALLOC_TYPES
#define JAZZY_ALLOC_TYPES

#include<stddef.h>
#include <stdint.h>


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Params:
/// - size_t: amount of bytes needed
typedef void *(*AllocFunc)(size_t);

/// This type represents functions that are used to reallocate memory
/// the function 'realloc' is of this type
///
/// Params:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void *(*ReAllocFunc)(void *, size_t);

/// This type represents functions that are used to allocate initialized memory
/// the function 'calloc' is of this type
///
/// Params:
/// - void *: old pointer
/// - size_t: amount of bytes needed
typedef void (*CalloFunc)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Params:
/// - void *: pointer to memory  to free
typedef void (*FreeFunc)(void *);




#endif // JAZZY_ALLOC_TYPES



// Libraries
#include <sys/types.h>
#include <stdbool.h>

/// A Pair to store a key and a value
///
/// This struct just stores a key and a value as a pair
typedef struct {
    bool occupied;
    k key;
    v value;
} Pair;

/// A Hashtable
///
/// This struct stores the actual key value Pairs, and additional 
/// metadata
typedef struct {
    size_t capacity;
    AllocFunc alloc;
    FreeFunc dealloc;
    unsigned int (*hashfunc)(const k);
    bool (*cmp_func)(const k, const k);
    Pair *storage; // DEALLOC
} Htab;


/*************************** Htab Documentation ******************************/

/// NOTE: If you see Key, Val, key, val or KEY, VAL in the documentation you have to
/// mentally replace them with the key and value you would use.



/// Initialize a hash table (htab_key_value_init)
///
/// This function initializes a hash table and allocates
/// the necessary memory according to [alloc]
///
/// Parameters:
///   - alloc: allocator function (malloc by default)
///   - dealloc: free function (free by default)
///   - hash_func: hash function (fnv1a hash by default)
///   - cmp_func: compare function (something like memcmp by default)
///
/// Returns:
///   A handle to a hash table or NULL on failure to allocate memory
static inline Htab *comp2(_htab, _init)(const AllocFunc alloc, const FreeFunc dealloc,
        unsigned int (*hash_func)(const k), bool (*cmp_func)(const k, const k));




/// Insert a value and key into the hash table (htab_k_v_put)
///
/// This function adds a value and a key into the
/// hash table. Both value and key are copied so a key length
/// and value length have to be provided.
///
/// Parameters:
///   - table: handle to a hashtable that was returned by `htab_k_v_init`
///   - value: pointer to the value that will be added
///   - key: key that is associated with [value]
static inline void comp2(_htab, _put)(Htab *table, const k key, const v value);


/// Get a value that is associated with a key (htab_k_v_get)
///
/// This function returns a pointer to the value that is associated with the key
/// if the key exists.
///
/// Parameters:
///   - table: handle to a hashtable that was returned by `htab_k_v_init`
///   - key: key to look for
///
/// Returns:
///   A pointer to the value associated with [key] if the value exists
///   NULL if the value does not exist.
static inline v *comp2(_htab, _get)(Htab *table, const k key);


/// Delete an entry from the table
///
/// This function deletes a value that is associated with the 
/// [key] if it exists
///
/// Parameters:
///   - table: handle to a hashtable that was returned by `htab_k_v_init`
///   - key: key to look and delete its value
static inline void comp2(_htab, _del)(Htab *table, const k key);

/// Free up memory used by the hash table (htab_key_value_free)
///
/// This function frees up memory that is used by the
/// hash table according to the dealloc function that
/// was provided when [htab] was initialized
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_k_v_free`
static inline void comp2(_htab, _free)(Htab *table);





/*************************** Htab Implementation ******************************/



/// Default hash function (htab_key_value_default_hash)
///
/// This is the default hash function and calculates the
/// fnv-1a hash on the bytes of the key. It is actually 
/// not inteded to be used by the user
///
/// Parameters:
///   - key: a key to look for
// NOTE: See (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
static inline unsigned int comp2(_htab, _default_hash)(const k key) {
    const char *key_bytes = (char *)&key;
    uint64_t hash = 14695981039346656037LU;
    for (int i = 0; i < sizeof(k); ++i) {
        hash ^= key_bytes[i];
        hash *= 1099511628211;
    }
    return hash;
}



static inline bool comp2(_htab, _default_cmp)(const k key1, const k key2) {
    return memcmp(&key1, &key2, sizeof(k)) == 0;
}


/// Initialize a hash table (htab_key_value_init)
///
/// This function initializes a hash table and allocates
/// the necessary memory according to [alloc]
///
/// Parameters:
///   - alloc: allocator function (malloc by default)
///   - dealloc: free function (free by default)
///   - hash_func: hash function (fnv1a hash by default)
///
/// Returns:
///   A handle to a hash table or NULL on failure to allocate memory
static inline Htab *comp2(_htab, _init)(const AllocFunc alloc, const FreeFunc dealloc, 
        unsigned int (*hash_func)(const k), bool (*cmp_func)(const k, const k)) {

    // Store alloc and dealloc in the function
    AllocFunc local_alloc = alloc;
    FreeFunc local_free = dealloc;

    // Set default allocator if needed
    if (alloc == NULL || dealloc == NULL) {
        local_alloc = malloc;
        local_free = free;
    }

    // Allocate struct
    Htab *new_htab = local_alloc(sizeof(Htab));
    if (new_htab == NULL) {
        return NULL;
    }

    // Allocate storage
    Pair *new_storage = local_alloc(sizeof(Pair) * HTAB_INIT_SIZE);
    if (new_storage == NULL) {
        local_free(new_htab);
        return NULL;
    }

    // Zero out memory
    memset(new_storage, 0, sizeof(Pair) * HTAB_INIT_SIZE);

    // Keep these here so that they are not considered unused
    /*
    comp2(_htab, _get)(NULL, new_htab->storage[0].key);
    comp2(_htab, _del)(NULL, new_htab->storage[0].key);
    comp2(_htab, _put)(NULL, new_htab->storage[0].key, new_htab->storage[0].value);
    */



    // Assign fields
    new_htab->dealloc = local_free;
    new_htab->alloc = local_alloc;
    new_htab->capacity = HTAB_INIT_SIZE;
    new_htab->hashfunc = hash_func == NULL ? comp2(_htab, _default_hash) : hash_func;
    new_htab->cmp_func = cmp_func == NULL ? comp2(_htab, _default_cmp): cmp_func;
    new_htab->storage = new_storage;

    return new_htab;
}


/// grow a hashtable (htab_k_v_grow)
///
/// This function grows the internal storage of the hash table
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_k_v_init`
static inline void comp2(_htab, _grow)(Htab *htab) {
    // save storage
    Pair *old_storage = htab->storage;
    size_t old_cap = htab->capacity;

    // Allocate new storage
    Pair *new_storage = htab->alloc(sizeof(Pair) * old_cap * 2);
    if (new_storage == NULL) {
        return;
    }
    // Zero out
    memset(new_storage, 0, sizeof(Pair) * old_cap * 2);

    // Assign new_storage to htab
    htab->storage = new_storage;
    htab->capacity = old_cap * 2;

    for (uint64_t i = 0; i < old_cap; i++) {
        const Pair cur_pair = old_storage[i];
        comp2(_htab, _put)(htab, cur_pair.key, cur_pair.value);
    }

    // Cleanup old storage
    htab->dealloc(old_storage);
}






/// Insert a value and key into the hash table (htab_k_v_put)
///
/// This function adds a value and a key into the
/// hash table. Both value and key are copied so a key length
/// and value length have to be provided.
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_k_v_init`
///   - value: pointer to the value that will be added
///   - key: key that is associated with [value]
static inline void comp2(_htab, _put)(Htab *table, const k key, const v value) {
    // Sanity check
    if (table == NULL) {
        return;
    }

    // Calculate index
    uint64_t hash = table->hashfunc(key); 
    uint64_t index = hash % table->capacity;

    if (!table->storage[index].occupied) {
        table->storage[index].key = key;
        table->storage[index].value = value;
        table->storage[index].occupied = true;
    } else {
        uint64_t other_index = (index + 1) % table->capacity;
        while(table->storage[other_index].occupied) {
            if (other_index == index) { // If this is true, we need to grow
                comp2(_htab, _grow)(table);
                other_index = hash % table->capacity;
            } else {
                // Increment other_index
                other_index = (other_index + 1) % table->capacity;
            }
        }
        table->storage[other_index].key = key;
        table->storage[other_index].value = value;
        table->storage[other_index].occupied = true;
    }
}



static inline v *comp2(_htab, _get)(Htab *table, const k key) {
    // Sanity check
    if (table == NULL) {
        return NULL;
    }

    // Calculate index
    uint64_t hash = table->hashfunc(key); 
    uint64_t index = hash % table->capacity;

    if (table->cmp_func(table->storage[index].key, key) && table->storage[index].occupied) {
        return &table->storage[index].value;
    } else {
        uint64_t other_index = (index + 1) % table->capacity;
        while(!table->cmp_func(table->storage[other_index].key, key) || !table->storage[other_index].occupied) {
            if (other_index == index) { // Could not find value
                return NULL;
            }
            other_index = (other_index + 1) % table->capacity;
        }
        return &table->storage[other_index].value;
    }
}


static inline void comp2(_htab, _del)(Htab *table, const k key) {
    // Sanity check
    if (table == NULL) {
        return;
    }

    // Calculate index
    uint64_t hash = table->hashfunc(key); 
    uint64_t index = hash % table->capacity;

    if (table->cmp_func(table->storage[index].key, key)) {
        memset(&table->storage[index], 0, sizeof(Pair));
    } else {
        uint64_t other_index = (index + 1) % table->capacity;
        while(!table->cmp_func(table->storage[other_index].key, key)) {
            if (other_index == index) { // Could not find value
                return;
            }
            other_index = (other_index + 1) % table->capacity;
        }
        memset(&table->storage[other_index], 0, sizeof(Pair));
    }
}


/// Free up memory used by the hash table (htab_key_value_free)
///
/// This function frees up memory that is used by the
/// hash table according to the dealloc function that
/// was provided when [htab] was initialized
///
/// Parameters:
///   - htab: handle to a hashtable that was returned by `htab_k_v_free`
static inline void comp2(_htab, _free)(Htab *table) {
    // Store the dealloc function
    FreeFunc dealloc = table->dealloc;

    // free the storage
    dealloc(table->storage);

    // free the struct
    dealloc(table);
}




// Keys
#undef k // The actual
#undef snake_k // Used for
#undef pascal_k // Used for the vec

// Values
#undef v // The actual
#undef snake_v // Used for
#undef pascal_v // Used for the vec

// Macros for htab
#undef _Htab
#undef __htab
#undef HtabOf
#undef htabof
#undef Htab
#undef _htab


// Macros for pair
#undef _Pair
#undef __pair
#undef PairOf
#undef pairof
#undef Pair
#undef _pair



// Used to create function names
#undef concat2
#undef comp2


// Initial size of the hashtable
#undef HTAB_INIT_SIZE



#endif
