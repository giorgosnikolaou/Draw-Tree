#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "AVLTree.h"

#ifdef DRAW
    #include "bmp.h"
#endif

struct avl_tree
{
    AVLNode root;
    int size;
    CompareFunc compare;
    DestroyFunc destroy;
};


// An AVL tree node 
struct avl_tree_node
{ 
    void* key; 
    AVLNode left; 
    AVLNode right; 
    int height; 
}; 


void pt(int tabs)
{
    for (int i = 0; i < tabs; i++)
        printf("\t");
}

void rec(AVLNode root, int* tabs)
{
    pt(*tabs);
    if (root == NULL)
    {
        printf("--\n");
        return ;
    }
    

    printf("%d", *(int*)root->key);
    printf("\n");
    
    (*tabs)++;
    rec(root->left, tabs);
    rec(root->right, tabs);
    (*tabs)--;
}

void avl_print_tree(AVLTree tree)
{
    assert(tree != NULL);
    int tabs = 0;
    rec(tree->root, &tabs);
    printf("\n");
}


static int height(AVLNode node) 
{ 
    return node ? node->height : 0; 
} 
static int update_height(AVLNode node) 
{ 
    int left = height(node->left);
    int right = height(node->right);

    return ((left > right) ? left : right) + 1; 
} 

static AVLNode rotate_left(AVLNode node)
{
    AVLNode right = node->right;
    AVLNode right_left = right->left;

    right->left = node;
    node->right = right_left;

    node->height = update_height(node);
    right->height = update_height(right);

    return right;
}
static AVLNode rotate_right(AVLNode node)
{
    AVLNode left = node->left;
    AVLNode left_right = left->right;

    left->right = node;
    node->left = left_right;

    node->height = update_height(node);
    left->height = update_height(left);

    return left;
}

static int balance_factor(AVLNode node) 
{ 
    if (node == NULL) 
        return 0; 

    return height(node->left) - height(node->right); 
} 
static AVLNode fix_balance(AVLTree tree, AVLNode node)
{
    node->height = update_height(node);

    int balance = balance_factor(node);

    if (balance > 1)    // Left - Right or Left - Left case
    {
        if (balance_factor(node->left) < 0)
            node->left = rotate_left(node->left);
        
        return rotate_right(node);
    }
    else if (balance < -1)  // Right - Left or Right - Right case
    {
        if (balance_factor(node->right) > 0)
            node->right = rotate_right(node->right);
        
        return rotate_left(node);
    }
    
    // AVLNode isn't unbalanced
    return node;

}


static AVLNode node_find_max(AVLNode node) 
{
    // If there exist a right subtree the max is there, otherwise it's the node itself
	return node && node->right ? node_find_max(node->right)	: node;									
}
static AVLNode node_find_min(AVLNode node) 
{
    // If there exist a left subtree the min is there, otherwise it's the node itself
	return node && node->left ? node_find_min(node->left) : node;
}

// Stores the largest value of a subtree to key
// Removes its node and fixes the heights going up the stack
static AVLNode remove_max(AVLTree tree, AVLNode node, void** key)
{
    if (!node->right) 
    {
        AVLNode left = node->left;
		*key = node->key;       

        free(node);

		return left;		
	} 

    node->right = remove_max(tree, node->right, key);

    return fix_balance(tree, node);

}
static AVLNode insertR(AVLTree tree, AVLNode root, void* key)
{
    if (!root)
    {
        tree->size++;
        return create_node(key);
    }

    void* root_key = root->key;

    int comp = tree->compare(root_key, key);

    if (comp > 0)
        root->left = insertR(tree, root->left, key);    
    else if (comp < 0)
        root->right = insertR(tree, root->right, key);
    else 
    {
        if (tree->destroy)
            tree->destroy(root_key);
        
        root->key = key;
        return root;
    }
    


    return fix_balance(tree, root);

}
static AVLNode removeR(AVLTree tree, AVLNode root, void* key)
{
    if (!root)
        return NULL;
    
    int comp = tree->compare(root->key, key);

    if (comp > 0)
        root->left = removeR(tree, root->left, key);
    else if (comp < 0)
        root->right = removeR(tree, root->right, key);
    else
    {
        tree->size--;

        if (!root->left)
        {
            AVLNode new_root = root->right;
            destroy_node(root, tree->destroy);
            return new_root;
        }

        if (!root->right)
        {
            AVLNode new_root = root->left;
            destroy_node(root, tree->destroy);
            return new_root;
        }

        // AVLNode is not a leaf
        // Find its inorder predecessor, store its value to the root 
        // and free the old value, the one the user wants to remove

        void* new_key = NULL;
        root->left = remove_max(tree, root->left, &new_key);
        
        if (tree->destroy)
            tree->destroy(root->key);

        root->key = new_key;

        return fix_balance(tree, root);
    }

    if (!root)
        return NULL;
    
    return fix_balance(tree, root);

}
static void destroyR(AVLNode root, DestroyFunc destroy)
{
    if (!root)
        return ;
    
    destroyR(root->left, destroy);
    destroyR(root->right, destroy);

    destroy_node(root, destroy);
}


static void pt(int tabs)
{
    for (int i = 0; i < tabs; i++)
        printf("\t");
}
static void printR(AVLNode root, ActionFunc print, int tabs)
{
    
    pt(tabs);

    if (!root)
    {
        printf("--\n");
        return ;
    }

    print(root->key);
    printf("\n");
    // printf("height: %d\n", root->height);

    if (!root->left && !root->right)
        return ;
    
    printR(root->left, print, tabs + 1);
    printR(root->right, print, tabs + 1);

}
static void inorderR(AVLNode root, ActionFunc visit)
{
    if (!root)
        return ;
    
    inorderR(root->left, visit);

    visit(root->key);

    inorderR(root->right, visit);
}


static AVLNode find_node(AVLNode node, CompareFunc compare, void* key)
{
    if (!node)
        return NULL;    // Not found

    int comp = compare(key, node->key);

    if (comp < 0)   // key < node->key and therfore key is located to the left subtree of node
        return find_node(node->left, compare, key);
    else if (comp > 0)  // key > node->key and therfore key is located to the right subtree of node
        return find_node(node->right, compare, key);
    
    return node;
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


    // target->key > node->key and therefore we go to the right subtree
    AVLNode res = find_prev(node->right, compare, target);
    return res ? res : node;
	
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


    // target->key < node->key and therefore we go to the left subtree
    AVLNode res = find_next(node->left, compare, target);
    return res ? res : node;
	
}


AVLTree avl_create(CompareFunc compare, DestroyFunc destroy)
{
    assert(compare);

    AVLTree tree = malloc(sizeof(*tree));
    assert(tree);

    tree->root    = NULL;
    tree->size    = 0;
    tree->compare = compare;
    tree->destroy = destroy;

    return tree;

}

size_t avl_size(AVLTree tree)
{
    return tree->size;
}

AVLNode avl_first(AVLTree tree)
{
    return node_find_min(tree->root);
}

AVLNode avl_last(AVLTree tree)
{
    return node_find_max(tree->root);
}

AVLNode avl_find_node(AVLTree tree, void* key)
{
    return find_node(tree->root, tree->compare, key);
}

AVLNode avl_next(AVLTree tree, AVLNode node) 
{
	return find_next(tree->root, tree->compare, node);
}

AVLNode avl_prev(AVLTree tree, AVLNode node) 
{
	return find_prev(tree->root, tree->compare, node);
}

void* avl_node_value(AVLNode node)
{
    return node->key;
}

void avl_destroy(AVLTree tree)
{
    destroyR(tree->root, tree->destroy);
    free(tree);
}

void avl_insert(AVLTree tree, void* key)
{
    assert(key);
    tree->root = insertR(tree, tree->root, key);
}

int avl_remove(AVLTree tree, void* key)
{
    assert(key);

    int old_size = tree->size;
    tree->root = removeR(tree, tree->root, key);
    return old_size > tree->size;
}

void avl_print(AVLTree tree, ActionFunc print)
{
    printR(tree->root, print, 0);
}

void avl_inorder(AVLTree tree, ActionFunc visit)
{
    inorderR(tree->root, visit);
}

DestroyFunc avl_set_destroy(AVLTree avl, DestroyFunc destroy)
{
    DestroyFunc old = avl->destroy;
    avl->destroy = destroy;
    return old;
}


#ifdef DRAW
    static int get_size(AVLTree tree, AVLNode root)
    {
      if (root == NULL)
        return 0;
      
      return get_size(tree, root->left) + get_size(tree, root->right) + 1;
    }

    // O(nlogn)
    // Could be O(n) if every node stored the size of the tree rooted with node (Like my btree ATD)
    static void draw(Bitmap* bitmap, AVLTree tree, char* (*to_str)(void* a), AVLNode root, int* x, int y, int dist_hor, int dist_per, int radius)
    {
        if (root == NULL)
            return ;
        

        int p_y = y + dist_per;
        
        // We place every node radius + 20 pixels to the right of the right most node of its left subtree
        int old_x = *x + (get_size(tree, root->left) + 1) * (radius + 20);;

        // We draw any lines that are to be drawn to the children before any recursion occurs 
        // to be able to just clear the lines that will be inside the circles after all the recursion

        if (root->left)
        {
            int p_x = old_x - (get_size(tree, root->left->right) + 1) * (radius + 20);
            bm_line(bitmap, old_x, y, p_x, p_y);
        }

        if (root->right)
        {
            int p_x = old_x + (get_size(tree, root->right->left) + 1) * (radius + 20);
            bm_line(bitmap, old_x, y, p_x, p_y);
        }

        draw(bitmap, tree, to_str, root->left, x, y + dist_per, dist_hor, dist_per, radius);

        *x = old_x;

        draw(bitmap, tree, to_str, root->right, x, y + dist_per, dist_hor, dist_per, radius);

        bm_set_color(bitmap, bm_atoi("white")); // Since the lines are from the parnent's center to its child we need to remove the line inside the circle
        bm_fillcircle(bitmap, old_x, y, radius);    // We do this here only for the parent because it will recursively be done to the children 

        bm_set_color(bitmap, bm_atoi("black"));
        bm_circle(bitmap, old_x, y, radius);        // Draw the circle

        // Now just write the root's value and continue with its childrend
        char* num = to_str(root->key);

        int th = bm_text_height(bitmap, num);
        int tw = bm_text_width(bitmap, num);
        bm_puts(bitmap, old_x - tw / 2, y - th / 2, num);

        free(num);
    }



    static int get_max_height(AVLNode root, int height)
    {
        if (root == NULL)
            return height - 1;

        int l = get_max_height(root->left, height + 1);
        int r = get_max_height(root->right, height + 1);
        
        return (l > r) ? l : r;
    }


    void avl_draw(AVLTree tree, char* image_name, char* (*to_str)(void* a))
    {
        if (tree->size == 0)
        {
            Bitmap* bitmap = bm_create(100, 100);

            bm_set_color(bitmap, bm_atoi("white"));
            bm_clear(bitmap);
            bm_save(bitmap, image_name);       

            bm_free(bitmap);

            return ;
        }


        int levels = get_max_height(tree->root, 1);

        int radius = 20;

        int height = (radius + 20) * levels * 2; 
        int width = (radius + 20) * (avl_size(tree) + 1); 

        int dist_hor = (radius + 20);
        int dist_per = (radius + 20) * 2;


        Bitmap* bitmap = bm_create(width, height);

        bm_set_color(bitmap, bm_atoi("white"));
        bm_clear(bitmap);
        bm_set_color(bitmap, bm_atoi("black"));

        int x = 0;
        int y = radius + 20; // Starting coordinates
        
        draw(bitmap, tree, to_str, tree->root, &x, y, dist_hor, dist_per, radius); 
            
            
        bm_save(bitmap, image_name);       

        bm_free(bitmap);
    }   
#endif


