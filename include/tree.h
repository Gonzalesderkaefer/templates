// Libraries
#include<stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// # How to use this library
/// NOTE: for the function documentations go to the section 'Tree Implementation'
///
/// ## Naming convention
///
/// Function names for this type have the following stucture:
/// tree_<type>_<action>, where <type> is what the constant 'snake_type'
/// has been set to down below.
/// For example, to initialize a tree:
/// tree_int_init
///
/// This type's name has the following structure:
/// Tree<Type>, where <Type> is what the constant 'pascal_type' which should be in
/// pascal case but doesn't have, to be has been set to down below. If you had a
/// tree of ints, the type would be called TreeInt.
///
/// ## Aditional remarks
///
/// If the type is a pointer you cannot use the asterisk (*). It is recommended
/// for the pascal and snake type to replace the asterisk with 'ptr'.
///
/// ## Include guards
///
/// You have to add the ifdef gurads yourself because
/// the type of the tree is defined by you
///
/// Something like TREE_TYPE_H is recommendend where TYPE is the
/// snake_type below but captilaized.
/// If you had a tree of ints the you would define
/// TREE_INT_H
///
///          vvvv  Change this to your type
#ifndef TREE_TYPE_H
#define TREE_TYPE_H

// Edit these
#define T int // The actual typename
#define snake_type int // Used for functions
#define pascal_type Int // Used for the tree typename




// Helper Macros for the type
#define _Tree(t) Tree##t
#define __tree(t) tree_##t
#define TreeOf(t) _Tree(t)
#define treeof(t) __tree(t)
#define Tree TreeOf(pascal_type)
#define _tree treeof(snake_type)

// Helper macros to define functions
#define function(prefix, action) prefix##_##action
#define func(prefix, action) function(prefix, action)
#define fn(action) func(_tree, action) // -> Evaluates to tree_<T>_<action>


// Helper macros to define 'private' functions
#define _function(prefix, action) _##prefix##_##action
#define _func(prefix, action) _function(prefix, action)
#define _fn(action) _func(_tree, action) // -> Evaluates to _tree_<T>_<action>


/****************************** Alloc functions *******************************/
#ifndef TREE_FUNC_TYPES_H
#define TREE_FUNC_TYPES_H
/// This type represents functions that are used to allocate memory
/// the function 'malloc' is of this type
///
/// Parameters:
/// - size_t: amount of bytes needed
typedef void *(*TreeAllocFn)(size_t);

/// This type represents functions that are used to free memory
/// the function 'free' is of this type
///
/// Parameters:
/// - void *: pointer to memory  to free
typedef void (*TreeFreeFn)(void *);

/// This type represents functions that are used to compare to
/// blocks of memory the function 'memcmp' is of this type
///
/// Parameters:
///   - void *: first block of memory
///   - void *: second block of memory
///   - size_t: how many bytes o compare
typedef int (*TreeComparator)(const void *, const void *, size_t);
#endif
/**************************** Node Implementation *****************************/
// These are type agnostic types
#ifndef TREE_HELPER_TYPES_H
#define TREE_HELPER_TYPES_H
typedef enum {
    Left,
    Right,
    Root,
    Fail,
} Direction;

typedef enum {
    LeftRot,
    RightRot,
} RotationDir;
#endif // TREE_HELPER_TYPES_H


// Helper Macros for the node
#define _Node(t) Node##t
#define __node(t) node_##t
#define NodeOf(t) _Node(t)
#define nodeof(t) __node(t)
#define Node NodeOf(pascal_type)
#define _node nodeof(snake_type)

// Helper macro for node functions
#define nodefn(action) _func(_node, action)


// Typedef for the TreeNode
typedef struct Node Node;

struct Node {
    uint64_t height;
    Node *parent;
    Node *left;
    Node *right;
    T value;
};


static inline Node *nodefn(init)(const T value, const Node *parent, TreeAllocFn alloc) {
    // Sanity check for the alloc func
    TreeAllocFn l_alloc = alloc;
    if (alloc == NULL) {
        l_alloc = malloc;
    }
    // Allocate node
    Node *new_node = l_alloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    // Assign values
    new_node->parent = (Node *)parent;
    new_node->value = value;
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}


static inline void nodefn(free)(Node *node, TreeFreeFn dealloc) {
    dealloc(node);
}






#undef T
#undef snake_type
#undef pascal_type
#undef _Tree
#undef __tree
#undef TreeOf
#undef treeof
#undef Tree
#undef _tree


#undef _Node
#undef __node
#undef NodeOf
#undef nodeof
#undef Node
#undef _node


#endif // TREE_TYPE_H
