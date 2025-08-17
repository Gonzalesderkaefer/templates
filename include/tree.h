// Libraries
#include <assert.h>
#include <stddef.h>
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

// Other helper macros
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))

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
#define _Node(t) TreeNode##t
#define __node(t) node_##t
#define NodeOf(t) _Node(t)
#define nodeof(t) __node(t)
#define Node NodeOf(pascal_type)
#define _node nodeof(snake_type)

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



static inline Direction nodefn(dir)(const Node *node) {
    assert(node != NULL);
    if (node->parent == NULL) {
        return Root;
    }
    if (node->parent->left == node) {
        return Left;
    }
    if (node->parent->right == node) {
        return Right;
    }
    // Something's not right
    return Fail;
}


static inline uint64_t nodefn(height)(const Node *node) {
    if (node != NULL) {
        return node->height;
    } else {
        return 0;
    }
}


// NOTE:
// return_val < 0 ==> Right heavy
// return_val > 0 ==> Left heavy
static inline int64_t nodefn(get_balance)(const Node *node) {
    if (node != NULL) {
        return nodefn(height)(node->left) - nodefn(height)(node->right);
    } else {
        return 0;
    }
}



static inline void nodefn(update_height)(Node *node) {
    if (node == NULL) {
        return;
    }
    node->height = 1 + max(nodefn(height)(node->left), nodefn(height)(node->right));
}




static inline Node *nodefn(rotate)(Node *node, const RotationDir dir) {
    // NULL checks
    if (node == NULL) {
        return NULL;
    }

    Node *old_root = node, *old_parent = node->parent;
    Node *new_root, *inner_grandchild;
    switch (dir) {
    case Left:
        new_root = node->right;
        inner_grandchild = new_root->left;
        new_root->left = old_root;
        old_root->right = inner_grandchild;
        old_root->parent = new_root;
        new_root->parent = old_parent;
        nodefn(update_height)(old_root);
        nodefn(update_height)(new_root);
        break;
    case Right:
        new_root = node->left;
        inner_grandchild = new_root->right;
        new_root->right = old_root;
        old_root->left = inner_grandchild;
        nodefn(update_height)(old_root);
        nodefn(update_height)(new_root);
        break;
    }
    return new_root;
}





static inline Node *nodefn(balance)(Node *node) {
    if (node == NULL) {
        return NULL;
    }
    int balance_value = nodefn(get_balance)(node);
    if (balance_value <= -2) { // Is right heavy -> rotate left
        int inner_bal = nodefn(get_balance)(node->right);
        if (inner_bal >= 1) {
            node->right = nodefn(rotate)(node->right, RightRot);
        }
        return nodefn(rotate)(node, LeftRot);

    } else if (balance_value >= 2) { // Is left heavy -> rotate right
        int inner_bal = nodefn(get_balance)(node->left);
        if (inner_bal <= -1) {
            node->left = nodefn(rotate)(node->left, LeftRot);
        }
        return nodefn(rotate)(node, RightRot);
    }
    return node;
}



/********************************* Tree ***************************************/

typedef struct {
    Node *root;
    TreeAllocFn alloc;
    TreeFreeFn dealloc;
    TreeComparator comp;
} Tree;




static inline Tree *fn(init)(const TreeAllocFn alloc, const TreeFreeFn dealloc, const TreeComparator comp) {
    // Check if alloc and free could be NULL
    TreeAllocFn local_alloc = alloc;
    TreeFreeFn local_free = dealloc;
    if (alloc == NULL || dealloc == NULL) {
        local_alloc = malloc;
        local_free = free;
    }
    // Allocate space on heap
    Tree *new_tree = local_alloc(sizeof(Tree));
    if (new_tree == NULL) {
        return NULL;
    }
    // Assign fields
    new_tree->alloc = local_alloc;
    new_tree->dealloc = local_free;
    new_tree->root = NULL;
    new_tree->comp = comp == NULL ? memcmp : comp;

    return new_tree;
}



static inline void fn(insert)(Tree *tree, const T value) {
    // Sanity check
    if (tree == NULL || tree->comp == NULL || 
            tree->alloc == NULL || tree->dealloc == NULL) {
        return;
    }

    // This is the first insertion
    if (tree->root == NULL) {
        tree->root = nodefn(init)(value, NULL, tree->alloc);
        if (tree->root == NULL) {
            return;
        }
        return;
    }

    // Create new node that will be added to the tree
    Node *new_node = nodefn(init)(value, NULL, tree->alloc);
    if (new_node == NULL) {
        return;
    }

    // Find the parent of the new node
    Node *parent = tree->root;
    while (parent != NULL) {
        int compval = tree->comp(&value, &parent->value, sizeof(T)); 

        if (compval == 0) { // NO duplicates!!
            // We have to free this node first
            nodefn(free)(new_node, tree->dealloc);
            return;
        } else if (compval < 0) { // Go left
            if (parent->left == NULL) {
                // Set for parent node
                parent->left = new_node;
                new_node->parent = parent;
                break;
            }
            // Keep going
            parent = parent->left;
        } else if (compval > 0) { // Go right
            if (parent->right == NULL) {
                // Set for parent node
                parent->right = new_node;
                new_node->parent = parent;
                break;
            }
            // Keep going
            parent = parent->right;
        }
    }
    // Parent cannot be null
    assert(parent != NULL);

    // We have to rotate the grand parent node
    Node *g_parent = parent->parent;

    // update heights
    while (parent != NULL) {
        nodefn(update_height)(parent);
        parent = parent->parent;
    }

    if (g_parent == NULL) {
        return;
    }
    const Direction dir = nodefn(dir)(g_parent);
    assert(dir != Fail && "Getting direction failed\n");

    switch (dir) {
    case Left:
        g_parent->parent->left = nodefn(balance)(g_parent);
        break;
    case Right:
        g_parent->parent->right = nodefn(balance)(g_parent);
        break;
    case Root:
        tree->root = nodefn(balance)(g_parent);
        break;
    case Fail:
        break;
    }
}


static const Node *_fn(lookup_node)(const Tree *tree, const T value) {
    Node *cur_node = tree->root;
    int compare_value;
    while (cur_node != NULL) {
        compare_value = tree->comp(&value, &cur_node->value, sizeof(T));
        if (compare_value == 0) {
            return cur_node;
        } else if (compare_value < 0) { // Go left
            cur_node = cur_node->left;
        } else { // Go right
            cur_node = cur_node->right;
        }
    }
    return NULL;
}




static inline const T *fn(lookup)(const Tree *tree, const T value) {
    // Sanity check
    if (tree == NULL) {
        return NULL;
    }


    // Find node
    const Node *found_node = _fn(lookup_node)(tree, value);

    if (found_node == NULL) {
        return NULL;
    }

    // return value
    return &found_node->value;
}


static void _fn(free_nodes)(Node *root, TreeFreeFn dealloc) {
    if (root == NULL) {
        return;
    } else if (root->left == NULL && root->right == NULL) {
        nodefn(free)(root, dealloc);
    } else {
        _fn(free_nodes)(root->left, dealloc);
        _fn(free_nodes)(root->right, dealloc);
        nodefn(free)(root, dealloc);
    }
}


static inline void fn(delete)(Tree *tree, const T value) {
    // Sanity check
    if (tree == NULL) {
        return;
    }

    // find node to delete
    Node *to_delete = (Node *) _fn(lookup_node)(tree, value);
    Node *to_del_parent = to_delete->parent;
    if (to_delete == NULL) {
        return;
    }
    while (to_delete != NULL) {
        if (to_delete->left != NULL && to_delete->right != NULL) {
            // Find next greater
            Node *next = to_delete->left;
            while (next->right != NULL) {
                next = next->right;
            }
            // Replace value
            to_delete-> value = next->value;

            // Now delete the next
            to_delete = next;
        } else if (to_delete->left != NULL) {
            const Direction dir = nodefn(dir)(to_delete);
            switch (dir) {
            case Left:
                to_delete->parent->left = to_delete->left;
                to_delete->left->parent = to_delete->parent;
                break;
            case Right:
                to_delete->parent->right = to_delete->left;
                to_delete->left->parent = to_delete->parent;
                break;
            case Root:
                tree->root = to_delete->left;
                to_delete->left->parent = NULL;
                break;
            case Fail:
                break;
            }
            nodefn(free)(to_delete, tree->dealloc);
            break;
        } else if (to_delete->right != NULL) {
            const Direction dir = nodefn(dir)(to_delete);
            switch (dir) {
            case Left:
                to_delete->parent->left = to_delete->right;
                to_delete->right->parent = to_delete->parent;
                break;
            case Right:
                to_delete->parent->right = to_delete->right;
                to_delete->right->parent = to_delete->parent;
                break;
            case Root:
                tree->root = to_delete->right;
                to_delete->right->parent = NULL;
                break;
            case Fail:
                break;
            }
            nodefn(free)(to_delete, tree->dealloc);
            break;
        } else { // Both are NULL
            const Direction dir = nodefn(dir)(to_delete);
            switch (dir) {
            case Left:
                to_delete->parent->left = NULL;
                break;
            case Right:
                to_delete->parent->right = NULL;
                break;
            case Root:
                tree->root = NULL;
                break;
            case Fail:
                break;
            }
            nodefn(free)(to_delete, tree->dealloc);
            break;
        }

        if (to_del_parent == NULL) {
            return;
        }
        const Direction dir = nodefn(dir)(to_del_parent);

        switch (dir) {
        case Left:
            to_del_parent->parent->left = nodefn(balance)(to_del_parent);
            break;
        case Right:
            to_del_parent->parent->right = nodefn(balance)(to_del_parent);
            break;
        case Root:
            tree->root = nodefn(balance)(to_del_parent);
            break;
        case Fail:
            break;
        }
    }
}



static inline fn(free)(Tree *tree) {
    // Free all nodes
    _fn(free_nodes)(tree->root, tree->dealloc);

    TreeFreeFn dealloc = tree->dealloc;
    dealloc(tree);
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


#undef function
#undef func
#undef fn
#undef _function
#undef _func
#undef _fn
#undef max
#undef min



#undef _Node
#undef __node
#undef NodeOf
#undef nodeof
#undef Node
#undef _node
#undef nodefn


#endif // TREE_TYPE_H
