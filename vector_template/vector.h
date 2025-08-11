// Libraries
#include<stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// # How to use this library
/// NOTE: for the function documentations go to the section 'Vec Implementation'
///
/// ## Naming convention
///
/// Function names for this type have the following stucture:
/// vec_<type>_<action>, where <type> is what the constant 'snake_type'
/// has been set to down below.
/// For example, to initialize a vector:
/// vec_int_init
///
/// This type's name has the following structure:
/// Vec<Type>, where <Type> is what the constant 'pascal_type' which should be in
/// pascal case but doesn't have, to be has been set to down below. If you had a
/// vector of ints, the type would be called VecInt.
///
/// ## Aditional remarks
///
/// If the type is a pointer you cannot use the asterisk (*). It is recommended
/// for the pascal and snake type to replace the asterisk with 'ptr'.
///
/// ## Include guards
///
/// You have to add the ifdef gurads yourself because
/// the type of the vector is defined by you
///
/// Something like VEC_TYPE_H is recommendend where TYPE is the
/// snake_type below but captilaized.
/// If you had a vector of ints the you would define
/// VEC_INT_H
///
///         vvvv  Change this to your type
#ifndef VEC_TYPE_H
#define VEC_TYPE_H

// Edit these
#define type int // The actual typename
#define snake_type int // Used for functions
#define pascal_type Int // Used for the vec typename




// Helper Macros for the type
#define _Vec(t) Vec##t
#define __vec(t) vec_##t
#define VecOf(t) _Vec(t)
#define vecof(t) __vec(t)
#define Vec VecOf(pascal_type)
#define _vec vecof(snake_type)

// Helper macros to define functions
#define concat2(a, b) a##b
#define comp2(a, b) concat2(a, b)






/****************************** Alloc *****************************************/

#ifndef JAZZY_ALLOC_TYPES
#define JAZZY_ALLOC_TYPES

// Libraries
#include<stddef.h>
#include <stdlib.h>
#include <string.h>


/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Params: - size_t: amount of bytes needed
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

/*************************** Vec Implementaion *****************************/

/// NOTE: If you see Type, type or TYPE, in the documentation you have to
/// mentally replace it with the type you would use.

/// Handle to a VecType
///
/// This type is used as a handle to a 
/// vector and a pointer to this is used
/// to pass into functions and perform operations on.
typedef struct {
    size_t cap;
    size_t len;
    AllocFunc alloc;
    FreeFunc dealloc;
    type *storage;
} Vec;





/// Initialize a vector (vec_type_init)
///
/// This function initializes a vector. It allocates memory
/// according to [alloc].
///
/// Parameters:
///   - alloc: an allocator function the function malloc is of this type
///   - dealloc: a function that frees memory
static Vec *comp2(_vec, _init)(AllocFunc alloc, FreeFunc dealloc) {
    // Both have to be defined or undefined
    if ((alloc == NULL) ^ (dealloc == NULL)) {
        return NULL;
    }

    // Allocate storage
    type *new_storage = alloc(sizeof(type) * 10);
    if (new_storage == NULL) {
        return NULL;
    }
    // Zero out
    memset(new_storage, 0, sizeof(type) * 10);

    // Allocate struct
    Vec *new_vect = alloc(sizeof(Vec));
    if (new_vect == NULL) {
        dealloc(new_storage);
        return NULL;
    }

    if (alloc == NULL && dealloc == NULL) {
        new_vect->alloc = malloc;
        new_vect->dealloc = free;
    } else {
        new_vect->alloc = alloc;
        new_vect->dealloc = dealloc;
    }
    new_vect->cap = 10;
    new_vect->len = 0;
    new_vect->storage = new_storage;


    return new_vect;
}


/// Push a value into the vector (vec_int_push)
///
/// This function appends a value to the end of [vec]
///
/// Parameters:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
///   - value: the value that will be appended to the vector
static void comp2(_vec, _push)(Vec *vec, const type value) {
    if (vec->len >= vec->cap) {
        // Allocate new storage
        type *new_storage = vec->alloc(sizeof(type) * vec->cap * 2);
        if (new_storage == NULL) {
            return;
        }
        // Zero out
        memset(new_storage, 0, sizeof(type) * vec->cap * 2);

        // Copy into new_storage 
        memcpy(new_storage, vec->storage, vec->len * sizeof(type));

        // Free old storage
        vec->dealloc(vec->storage);

        // Reassign new storage
        vec->storage = new_storage;

        // Update vec capacity
        vec->cap *= 2;
    }

    // Add new value 
    vec->storage[vec->len++] = (type)value;
}

/// Delete the value a given index (vec_type_delete)
///
/// This function deletes the value at [index] by shifting
/// every other to the forward by one.
///
/// Parameters:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
///   - index: index to remove
static void comp2(_vec, _delete)(Vec *vec, size_t index) {
    // Sanity check
    if (vec == NULL || vec->len <= index) {
        return;
    }

    // Move everything one to the left
    for (uint64_t i = index; i < vec->len - 1; ++i) {
        vec->storage[i] = vec->storage[i + 1];
    }
    // vector is now shorter
    vec->len--;
}

/// Get the raw storage of a vector (vec_type_storage)
///
/// This function returns a pointer to the undelying memory of a vector
///
/// Parameteters:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
///
/// Returns:
///   A pointer to the underlying storage 
static type *comp2(_vec, _storage)(const Vec *vec) {
    if (vec == NULL) {
        return NULL;
    } 
    return vec->storage;
}


/// Free up memory used by a vector (vec_type_free)
///
/// This function frees the memory used by a vector according to 'dealloc'
/// which was set in 'vec_type_init'
///
/// Parameteter:
///   - vec: a handle to a vector that was returned by 'vec_type_init'
static void comp2(_vec, _free)(Vec *vec) {
    // Store a pointer to the dealloc function
    FreeFunc dealloc = vec->dealloc;

    // Free storage
    dealloc(vec->storage);

    // Free struct itself
    dealloc(vec);
}


#undef type
#undef snake_type
#undef pascal_type
#undef _Vec
#undef __vec
#undef VecOf
#undef vecof
#undef Vec
#undef _vec




#endif // VEC_TYPE_H
