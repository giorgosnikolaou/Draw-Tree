#include <stdio.h>
#include <stdlib.h>
#include "draw_tree.h"


#define TREE AVLTree
#define NODE AVLNode
#define LEFT_CHILD left
#define RIGHT_CHILD right
#define FIND_MIN node_find_min
#define FIND_MAX node_find_max
#define NEXT avl_next
#define PREV avl_prev

static int get_size_left(TREE tree, NODE root)
{
    int count = 1;
    for (NODE node = FIND_MIN(root), max = FIND_MAX(root); node != max; node = NEXT(tree, node))
        count++;
    
    return FIND_MIN(root) ? count : 0;
}

static int get_size_right(TREE tree, NODE root)
{
    int count = 1;
    for (NODE node = FIND_MAX(root), min = FIND_MIN(root); node != min; node = PREV(tree, node))
        count++;
    
    return FIND_MAX(root) ? count : 0;
}


static void draw(Bitmap* bitmap, NODE root, int* x, int y, int dist_hor, int dist_per, int radius, TREE tree)
{
    if (root == NULL)
        return ;
    

    int p_y = y + dist_per;
    
    // We place every node radius + 20 pixels to the right of the right most node of its left subtree
    int pos_x = (get_size_left(tree, root->left) + 1) * (radius + 20);
    int old_x = *x + pos_x;

    // We draw any lines that are to be drawn to the children before any recursion occurs 
    // to be able to just clear the lines that will be inside the circles after all the recursion

    if (root->left)
    {
        int p_x = old_x - pos_x;
        bm_line(bitmap, old_x, y, p_x, p_y);
    }

    if (root->right)
    {
        // Radius + 20 pixels to the right of the right most node of the left subtree
        int p_x = old_x + (get_size_right(tree, root->right->left) + 1) * (radius + 20);
        bm_line(bitmap, old_x, y, p_x, p_y);
    }

    draw(bitmap, root->left, x, y + dist_per, dist_hor, dist_per, radius, tree);

    *x = old_x;

    draw(bitmap, root->right, x, y + dist_per, dist_hor, dist_per, radius, tree);

    bm_set_color(bitmap, bm_atoi("white")); // Since the lines are from the parnent's center to its child we need to remove the line inside the circle
    bm_fillcircle(bitmap, old_x, y, radius);    // We do this here only for the parent because it will recursively be done to the children 

    bm_set_color(bitmap, bm_atoi("black"));
    bm_circle(bitmap, old_x, y, radius);        // Draw the circle

    // Now just write the root's value and continue with its children
    char* num = voidp_to_str(root->key);

    int th = bm_text_height(bitmap, num);
    int tw = bm_text_width(bitmap, num);
    bm_puts(bitmap, old_x - tw / 2, y - th / 2, num);

    free(num);
}


static int get_max_height(NODE root, int height)
{
    if (root == NULL)
        return height - 1;

    int l = get_max_height(root->left, height + 1);
    int r = get_max_height(root->right, height + 1);
    
    return (l > r) ? l : r;
}


void tree_draw(TREE tree, char* image_name)
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
    
    draw(bitmap, tree->root, &x, y, dist_hor, dist_per, radius, tree); 
        
        
    bm_save(bitmap, image_name);       

    bm_free(bitmap);
}   
