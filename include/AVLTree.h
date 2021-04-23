#pragma once
#include <stdbool.h>
#include "bmp.h"

typedef struct avl_tree* AVLTree;   // Incomplete definition of an AVLTree, the handle of the AVL
typedef struct avl_tree_node* AVLNode;  // Incomplete definition of an AVLNode

// Pointer to a fuction that compares 2 pointers a and b and returns:
// < 0  if a < b
//   0  if a and b are equivelant
// > 0  if a > b
typedef int (*CompareFunc)(void* a, void* b);

// Pointer to a function that frees the memory allocated for the value
typedef void (*DestroyFunc)(void* value);


// Given a compare and a destroy function, creates a new avl tree
// and returns a pointer to it
AVLTree avl_create(CompareFunc compare, DestroyFunc destroy);


// Returns the size of the avl tree given
int avl_size(AVLTree tree);


// Removes, if it exists, the node with key "key" and returns 1, else returns 0
bool avl_remove(AVLTree tree, void* key);


// Insert, if it doesn't already exist, a new node with key "key"
void avl_insert(AVLTree tree, void* key);


// Returns the node with key "key", if it exists
// else returns NULL
AVLNode avl_find_node(AVLTree tree, void* key);


// Returns the key of the node "node"
void* avl_node_value(AVLNode node);


// Returns the next node of the tree
AVLNode avl_next(AVLTree tree, AVLNode node);


// Returns the next node of the tree
AVLNode avl_prev(AVLTree tree, AVLNode node);


// Return the node of the "smallest" key stored in the tree. Smallest depends solely on the CompareFunc given 
AVLNode avl_first(AVLTree tree);


// Returns the node of the "biggest" key stored in the tree.
AVLNode avl_last(AVLTree tree);


// Destroys a tree
void avl_destroy(AVLTree tree);

void avl_print_tree(AVLTree tree);

#ifdef DRAW    
    void avl_draw(AVLTree tree, const char* image_name);
#endif
