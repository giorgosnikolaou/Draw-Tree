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

#ifdef DRAW
    static char* int_to_str(void* a)
    {
        int val = *(int*)a;

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
#endif

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
    
    // AVLTree tree = fill_tree(compare_ints, free);
    
    // avl_print_tree(tree);
    
    // 1, 2, 3, 5, 6, 4, 7, 8, 10, 9

    AVLTree tree = avl_create(compare_ints, free);
    // avl_insert(tree, create_int(1));
    // avl_insert(tree, create_int(2));
    // avl_insert(tree, create_int(3));
    // avl_insert(tree, create_int(5));
    // avl_insert(tree, create_int(6));
    // avl_insert(tree, create_int(4));
    // avl_insert(tree, create_int(7));
    // avl_insert(tree, create_int(8));
    // avl_insert(tree, create_int(10));
    // avl_insert(tree, create_int(9));

    avl_insert(tree, create_int(20));
    avl_insert(tree, create_int(0));
    avl_insert(tree, create_int(10));
    avl_insert(tree, create_int(1));
    avl_insert(tree, create_int(3));
    avl_insert(tree, create_int(4));
    avl_insert(tree, create_int(5));
    avl_insert(tree, create_int(6));
    avl_insert(tree, create_int(7));
    avl_insert(tree, create_int(90));
    avl_insert(tree, create_int(80));
    avl_insert(tree, create_int(88));
    avl_insert(tree, create_int(87));
    avl_insert(tree, create_int(86));
    avl_insert(tree, create_int(85));
    avl_insert(tree, create_int(84));
    avl_insert(tree, create_int(90));
    avl_insert(tree, create_int(84));

    // int a = 3;
    // avl_remove(tree, &a);

    printf("Size: %d\n", avl_size(tree)); // Should be 16

    #ifdef DRAW
        avl_draw(tree, "tree.png", int_to_str);
    #else
        avl_print_tree(tree);
    #endif
    
    avl_destroy(tree);
    
}