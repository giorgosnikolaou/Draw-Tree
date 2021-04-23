#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "AVLTree.h"

// struct avl_tree 
// {
//     AVLNode root;
//     int size;
//     CompareFunc compare;
//     DestroyFunc destroy;
// };


// An AVL tree node 
// struct avl_tree_node 
// { 
//     void* key; 
//     AVLNode left; 
//     AVLNode right; 
//     int height; 
// }; 
  



int tabs = 0;


void pa()
{
    for (int i = 0; i < tabs; i++)
        printf("\t");
}

void rec(AVLNode root)
{
    pa();
    if (root == NULL)
    {
        printf("--\n");
        return ;
    }
    

    printf("%d", *(int*)root->key);
    printf("\n");
    
    tabs++;
    rec(root->left);
    rec(root->right);
    tabs--;
}

void avl_print_tree(AVLTree tree)
{
    assert(tree != NULL);
    rec(tree->root);
    printf("\n");
}



// Returns the maximum of two integers 
static int max(int a, int b) 
{ 
    return (a > b) ? a : b; 
} 
  


// Returns the height of the tree 
static int height(AVLNode node) 
{ 
    if (node == NULL) 
        return 0; 

    return node->height; 
} 


// Helper function that allocates a new node with the given key and sets
// as NULL left and right pointers. 
static AVLNode new_node(void* key) 
{ 
    AVLNode node = malloc(sizeof(*node)); 

    if (node == NULL)
    {
        printf("Not enough available memory to create the node!\n");
        exit(0);
    }

    node->key   = key; 
    node->left   = NULL; 
    node->right  = NULL; 
    node->height = 1;  // new node is initially added at leaf 
    return(node); 
} 
  

// Right rotates subtree rooted with node 
static AVLNode rotate_right(AVLNode node) 
{ 
    AVLNode node_left = node->left; 
    AVLNode node_right = node_left->right; 
  
    // Perform rotation 
    node_left->right = node; 
    node->left = node_right; 
  
    // Update heights 
    node->height = max(height(node->left), height(node->right)) + 1; 
    node_left->height = max(height(node_left->left), height(node_left->right)) + 1; 
  
    // Return new root 
    return node_left; 
} 
  

// Left rotates subtree rooted with node  
static AVLNode rotate_left(AVLNode node) 
{ 
    AVLNode node_right = node->right; 
    AVLNode node_left = node_right->left; 
  
    // Perform rotation 
    node_right->left = node; 
    node->right = node_left; 
  
    //  Update heights 
    node->height = max(height(node->left), height(node->right)) + 1; 
    node_right->height = max(height(node_right->left), height(node_right->right)) + 1; 
  
    // Return new root 
    return node_right; 
} 


// Get Balance factor of node N 
static int get_balance(AVLNode node) 
{ 
    if (node == NULL) 
        return 0; 

    return height(node->left) - height(node->right); 
} 
  

static AVLNode fix_balance(AVLNode node)
{
    // Update height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right)); 
    
    int balance = get_balance(node);  
  
    // If this node becomes unbalanced,  
    // there are 4 cases  
  
    // Left Left Case  
    if (balance > 1 && get_balance(node->left) >= 0)  
        return rotate_right(node);  
  
    // Left Right Case  
    if (balance > 1 && get_balance(node->left) < 0)  
    {  
        node->left = rotate_left(node->left);  
        return rotate_right(node);  
    }  
  
    // Right Right Case  
    if (balance < -1 && get_balance(node->right) <= 0)  
        return rotate_left(node);  
  
    // Right Left Case  
    if (balance < -1 && get_balance(node->right) > 0)  
    {  
        node->right = rotate_right(node->right);  
        return rotate_left(node);  
    }  
  
    return node;  
}


// Inserts a key in the subtree rooted 
// with node and returns the new root of the subtree. 
static AVLNode insert(AVLNode node, CompareFunc compare, void* key, bool* done) 
{ 
    /* 1.  Perform the normal BST insertion */
    if (node == NULL) 
    {
        *done = true;
        return new_node(key); 
    }
  
    int comp = compare(key, node->key);

    if (comp < 0) 
        node->left = insert(node->left, compare, key, done); 
    else if (comp > 0) 
        node->right = insert(node->right, compare, key, done); 
    
    // Equal keys are not allowed in BST, but we never have equal keys
    
    return fix_balance(node); 
} 


static AVLNode find_node(AVLNode node, CompareFunc compare, void* key)
{
    if (node == NULL)
        return NULL;    // Not found

    int comp = compare(key, node->key);

    if (comp < 0)   // key < node->key and therfore key is located to the left subtree of node
        return find_node(node->left, compare, key);
    else if (comp > 0)  // key > node->key and therfore key is located to the right subtree of node
        return find_node(node->right, compare, key);
    else    // key = node->key, we found it
        return node;
    

}


static AVLNode node_find_max(AVLNode node) 
{
    // If there exist a right subtree the max is there, otherwise it's the node itself
	return node != NULL && node->right != NULL ? node_find_max(node->right)	: node;									
}


static AVLNode node_find_min(AVLNode node) 
{
    // If there exist a left subtree the min is there, otherwise it's the node itself
	return node != NULL && node->left != NULL ? node_find_min(node->left) : node;
}


static AVLNode find_prev(AVLNode node, CompareFunc compare, AVLNode target) 
{
	// target: the node we want to find the previous of
    // node: "parent" of node. Starts with tree->root
    if (node == target) 
    {
		// There are two cases: We find the target node and it has a left subree, therefore 
        // the previous node will be the left most node of the left subtrre
        // or we find the target node and it doesn't have a left subtree, therefore
        // the previous node is the parent node. That is target == prev_node->right
        // In the second case the function will return NULL and this: return res != NULL ? res : node; will return the "parent" node of target
		return node_find_max(node->left);

	} 
    else if (compare(target->key, node->key) < 0) 
    {
		// target->key < node->key and therefore we go to the left subtree
        // In this case the previous node will be either the right most node of the left subtrre or target itself will be the smallest node of the tree
		return find_prev(node->left, compare, target);

	} 
    else 
    {
        // target->key > node->key and therefore we go to the right subtree
		AVLNode res = find_prev(node->right, compare, target);
		return res != NULL ? res : node;
	}


}


static AVLNode find_next(AVLNode node, CompareFunc compare, AVLNode target) 
{
	// target: the node we want to find the next of
    // node: "parent" of node. Starts with tree->root
    if (node == target) 
    {
		// There are two cases: We find the target node and it has a right subree, therefore 
        // the next node will be the left most node of the right subtrre
        // or we find the target node and it doesn't have a right subtree, therefore
        // the next node is the parent node. That is target == prev_node->left
        // In the second case the function will return NULL and this: return res != NULL ? res : node; will return the "parent" node of target
		return node_find_min(node->right);

	} 
    else if (compare(target->key, node->key) > 0) 
    {
		// target->key > node->key and therefore we go to the right subtree
        // In this case the next node will be either the left most node of the right subtrre or target itself will be the biggest node of the tree
		return find_next(node->right, compare, target);

	} 
    else 
    {
        // target->key < node->key and therefore we go to the left subtree
		AVLNode res = find_next(node->left, compare, target);
		return res != NULL ? res : node;
	}


}

  
// Recursive function to delete a node with given key from subtree with  
// given root. It returns root of the modified subtree.  
static AVLNode node_delete(AVLNode node, CompareFunc compare, void* key, void** to_destroy, bool* done)  
{  
    
    // to_destroy is the node value we need to destroy with the destroy func given when creating the array
    // done is to see if we actually removed anything

    // STEP 1: PERFORM STANDARD BST DELETE  
    if (node == NULL) 
    {
        *done = false;
        return node;  
    } 
  
    int comp = compare(key, node->key);

    // If the key to be deleted is smaller  
    // than the root's key, then it lies 
    // in left subtree  
    if (comp < 0)  
        node->left = node_delete(node->left, compare, key, to_destroy, done);
  
    // If the key to be deleted is greater  
    // than the root's key, then it lies  
    // in right subtree  
    else if(comp > 0)  
        node->right = node_delete(node->right, compare, key, to_destroy, done);
  
    // if key is same as root's key, then  
    // This is the node to be deleted  
    else
    {  
        *done = true;
        // printf("heheheheheh\n\n");
        *to_destroy = node->key;
        // node with only one child or no child  
        if( (node->left == NULL) || (node->right == NULL) )  
        {  
            // Get the child or NULL if it has no childs
            AVLNode temp = node->left ? node->left : node->right;  
  
            // No child case  
            if (temp == NULL)  
            {  
                temp = node;  
                node = NULL;  
            }  
            else // One child case  
                *node = *temp; // Copy the contents of  
                               // the non-empty child  
            free(temp);  
        }  
        else
        {  
            // node with two children: Get the inorder  
            // successor (smallest in the right subtree)  
            AVLNode temp = node_find_min(node->right);
            node->right = temp;
  
            // Replace node with temp 
			temp->left = node->left;
			temp->right = node->right;

			free(node);

			return fix_balance(temp);
        }  
    }  
  
    // If the tree had only one node before removal
    // then return  
    if (node == NULL)
        return node;
    
       
    
    return fix_balance(node);  
}  


// Recursivly destroys all the nodes of the tree
static void destroy_all(AVLNode node, DestroyFunc destroy)
{
    if (node == NULL)   // if we find a NULL node we reached the end of a subtree so we just return
		return;
	
	destroy_all(node->left, destroy);   // destroy every node's left subtree
	destroy_all(node->right, destroy);  // destroy every node's right subtree

	if (destroy != NULL)    // if a destroy function has been given destroy keys
		destroy(node->key);

	free(node); // finally free the node
}


/* header functions */

AVLTree avl_create(CompareFunc compare, DestroyFunc destroy)
{
    assert(compare != NULL);

    AVLTree tree = malloc(sizeof(*tree));

    assert(tree != NULL);

    tree->root = NULL;
    tree->size = 0;
    tree->compare = compare;
    tree->destroy = destroy;

    return tree;

}


int avl_size(AVLTree tree)
{
    assert(tree != NULL);
    return tree->size;
}


bool avl_remove(AVLTree tree, void* key) 
{
    assert(tree != NULL);

    bool done;
	void* to_remove = NULL;
	tree->root = node_delete(tree->root, tree->compare, key, &to_remove, &done);

	// Change size only if we have actually removed a node
	if (done)
    {
		tree->size--;

		if (tree->destroy != NULL)  // if there was given a non NULL destroy func 
			tree->destroy(to_remove);   // destroy the key of the node we just remove
	}

	return done;
}


void avl_insert(AVLTree tree, void* key) 
{
    assert(tree != NULL);

	bool done = 0;
	tree->root = insert(tree->root, tree->compare, key, &done);
	
	// Change size only if we have actually removed a node
	if (done)
		tree->size++;
    else if (tree->destroy != NULL)
        tree->destroy(key);
	
}


AVLNode avl_find_node(AVLTree tree, void* key) 
{    
    assert(tree != NULL);
	return find_node(tree->root, tree->compare, key);
}


void* avl_node_value(AVLNode node) 
{
    assert(node != NULL);
	return node->key;
}


AVLNode avl_next(AVLTree tree, AVLNode node) 
{
    assert(tree != NULL);
	return find_next(tree->root, tree->compare, node);
}


AVLNode avl_prev(AVLTree tree, AVLNode node) 
{
    assert(tree != NULL);
	return find_prev(tree->root, tree->compare, node);
}


AVLNode avl_first(AVLTree tree)
{
    assert(tree != NULL);
    return node_find_min(tree->root);
}


AVLNode avl_last(AVLTree tree)
{
    assert(tree != NULL);
    return node_find_max(tree->root);
}


void avl_destroy(AVLTree tree)
{
    assert(tree != NULL);
    destroy_all(tree->root, tree->destroy);
    free(tree);
}
