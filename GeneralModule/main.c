#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "draw_tree.h"


#define ELEMENTS_CONST 7


int compare_ints(void* a, void* b)
{
    return *(int*)a - *(int*)b;
}

int* create_int(int val)
{
    int* p = malloc(sizeof(int));
    *p = val;
    return p;
}

AVLTree fill_tree(CompareFunc compare, DestroyFunc destroy)
{
    AVLTree tree = avl_create(compare, destroy);
    
    int elements = rand() % ELEMENTS_CONST + ELEMENTS_CONST * 2;

    for (int i = 0; i < elements; i++)
        avl_insert(tree, create_int(rand() % (elements * 2)));

    return tree;

}


int main(void)
{
    srand(time(NULL));
    
    AVLTree tree = fill_tree(compare_ints, free);
    
    avl_print_tree(tree);

    
    int radius = 20;

    int height = (radius + 20) * levels * 2; 
    int width = (radius + 20) * power(2, levels);

    int dist_hor = (radius + 20);
    int dist_per = (radius + 20) * 2;

    int mid = width / 2;

	Bitmap* bitmap = bm_create(width, height);

    bm_set_color(bitmap, bm_atoi("white"));
    bm_clear(bitmap);
    bm_set_color(bitmap, bm_atoi("black"));

    int x = mid;
    int y = radius + 20; // Starting coordinates
    
    draw(bitmap, tree->root, x, y, dist_hor, dist_per, radius, levels - 1);   
    

    bm_save(bitmap, "tree.png");

    bm_free(bitmap);
    
    avl_destroy(tree);
    
}