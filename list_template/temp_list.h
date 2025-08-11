#ifndef TEMP_LIST_H
#define TEMP_LIST_H

// Libraries
#include <sys/types.h>
#include <stddef.h>
#include <string.h>



// The list type
#define ListOf(type) \
    struct { \
        size_t capacity; \
        size_t length; \
        void *(*alloc)(size_t); \
        void (*dealloc)(void *); \
        type *storage; \
    }



#define list_init(list, allocat, deallocat) \
    memset(&list, 0, sizeof(list)); \
    list.alloc = allocat; list.dealloc = deallocat

#define list_push(list, new_value) \
    /*         storage                 elem_size              cap             len           alloc       dealloc      */\
    grow_list((void **) &list.storage, sizeof(*list.storage), &list.capacity, &list.length, list.alloc, list.dealloc); \
    list.storage[list.length++] = new_value

#define list_action(list, func) \
    for (int i = 0; i < list.length; ++i) { \
        func(list.storage[i]); \
    }

#define list_get(list, index) list.storage[index]

#define list_len(list) list.length

#define list_free(list) \
    list.dealloc(list.storage)



static void grow_list(void **storage, size_t elem_size, size_t *cap, size_t *len,
        void *(*alloc)(size_t), void (*dealloc)(void *))  {
    // Sanity check
    if (elem_size == 0 || cap == NULL || len == NULL || storage == NULL) {
        return;
    }

    // No need to grow
    if (*cap > *len && *storage != NULL) {
        return;
    }

    if (*storage == NULL) { // Needs to be initialized
        *storage = alloc(10 * elem_size);
        *cap = 10;
        if (*storage == NULL) {
            return;
        }
    } else {
        // Allocate new storage
        void *new_storage = alloc(*cap * 2 * elem_size);
        if (new_storage == NULL) {
            return;
        }
        // Copy old storage into new storage
        memcpy(new_storage, *storage, *len * elem_size);

        // free old storage 
        dealloc(*storage);

        // Update capacity
        *cap *= 2;

        // Reassign new storage
        *storage = new_storage;
    }
}


#endif // TEMP_LIST_H
