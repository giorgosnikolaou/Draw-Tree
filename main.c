#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AVLTree.h"

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

    #ifdef DRAW
        avl_draw(tree, "tree.png");
    #endif
    
    
    avl_destroy(tree);
    
}