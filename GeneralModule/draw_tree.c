#include <stdio.h>
#include <stdlib.h>
#include "draw_tree.h"

#ifndef NODE
    #define NODE AVLNode
#endif

#ifndef LEFT_CHILD
    #define LEFT_CHILD left
#endif

#ifndef RIGHT_CHILD
    #define RIGHT_CHILD right
#endif

// a ^ b
int power(int a, int b)
{
    return (b > 0) ? power(a, b - 1) * a : 1;
}


static char* int_to_str(int val)
{
    int temp = val;
    size_t length = 0;

    while (temp)
    {
        length++;
        temp /= 10;
    }

    temp = val;
    
    length += val == 0; // In the spacial case where the value is 0 it will never enter the loop and the length would be 0  

    char* str = calloc(1, sizeof(char) * (length + 1));
    sprintf(str, "%d", val);

    return str;
}


void draw(Bitmap* bitmap, NODE root, int x, int y, int dist_hor, int dist_per, int radius, int levels)
{
    if (root == NULL)
        return ;
    
    int con = dist_hor * power(2, levels - 1);

    // First draw the neccesary circles and lines connecting them
    if (root->LEFT_CHILD != NULL)
        bm_line(bitmap, x - con, y + dist_per, x, y);
    
    if (root->RIGHT_CHILD != NULL)
        bm_line(bitmap, x + con, y + dist_per, x, y);

    bm_set_color(bitmap, bm_atoi("white")); // Since the lines are from the parnent's center to its child we need to remove the line inside the circle
    bm_fillcircle(bitmap, x, y, radius);    // We do this here only for the parent because it will recursively be done to the children 
    
    bm_set_color(bitmap, bm_atoi("black"));
    bm_circle(bitmap, x, y, radius);        // Draw the circle


    // Now just write the root's value and continue with its childrend
    char* num = int_to_str(*(int*)root->key);

    int th = bm_text_height(bitmap, num);
    int tw = bm_text_width(bitmap, num);
    bm_puts(bitmap, x - tw / 2, y - th / 2, num);

    free(num);


    levels--;
    
    draw(bitmap, root->LEFT_CHILD, x - con, y + dist_per, dist_hor, dist_per, radius, levels);
    draw(bitmap, root->RIGHT_CHILD, x + con, y + dist_per, dist_hor, dist_per, radius, levels);

    levels++;
    
}


int get_max_height(NODE root, int height)
{
    if (root == NULL)
        return height - 1;

    int l = get_max_height(root->LEFT_CHILD, height + 1);
    int r = get_max_height(root->RIGHT_CHILD, height + 1);
    
    return (l > r) ? l : r;
}
