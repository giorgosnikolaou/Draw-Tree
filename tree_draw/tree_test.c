#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "AVLTree.h"
#include "bmp.h"

// #define NODE AVLNode
#define ELEMENTS_CONST 15

typedef void (*DrawFunc)(Bitmap* bitmap, int x, int x1, int y, int dist_hor, int dist_per, int radius, char* parent, char* child);

// a ^ b
int power(int a, int b)
{
    return (b > 0) ? power(a, b - 1) * a : 1;
}

int logar(int a)
{
    int l = -1;
    while (power(2, ++l) <= a);
    
    return l;
} 


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

void print_int(void* val)
{
    printf("%d ", *(int*)val);
}


AVLTree fill_tree(CompareFunc compare, DestroyFunc destroy)
{
    AVLTree tree = avl_create(compare, destroy);
    // int elements = 37;
    int elements = rand() % ELEMENTS_CONST + ELEMENTS_CONST * 2;
    printf("Elements: %d\n", elements);

    for (int i = 0; i < elements; i++)
        avl_insert(tree, create_int(rand() % (elements * 2)));

    printf("Size: %d\n", avl_size(tree));
    return tree;

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
    
    length += val == 0;

    char* str = calloc(1, sizeof(char) * (length + 1));

    if (length > 0)
        sprintf(str, "%d", val);

    return str;
}


static void draw_node(Bitmap* bitmap, int x, int x1, int y, int dist_hor, int dist_per, int radius, char* parent, char* child)
{
    bm_line(bitmap, x1, y + dist_per, x, y);
    
    bm_set_color(bitmap, bm_atoi("white"));

    bm_fillcircle(bitmap, x, y, radius);
    bm_fillcircle(bitmap, x1, y + dist_per, radius);
    
    bm_set_color(bitmap, bm_atoi("black"));
    
    bm_circle(bitmap, x, y, radius);
    bm_circle(bitmap, x1, y + dist_per, radius);
}


static void draw_val(Bitmap* bitmap, int x, int x1, int y, int dist_hor, int dist_per, int radius, char* parent, char* child)
{
    int th = bm_text_height(bitmap, parent);
    int tw = bm_text_width(bitmap, parent);
    bm_puts(bitmap, x - tw / 2, y - th / 2, parent);

    if (child != NULL)
    {
        th = bm_text_height(bitmap, child);
        tw = bm_text_width(bitmap, child);
        bm_puts(bitmap, x1 - tw / 2, y + dist_per - th / 2, child);
    }
}


static int draw(Bitmap* bitmap, AVLNode root, int x, int y, int dist_hor, int dist_per, int radius, int levels, char* key, DrawFunc func)
{
    if (root == NULL)
        return 0;
    
    char* num = int_to_str(*(int*)root->key);
    key = num;

    levels--;

    char* a = NULL;

    if (draw(bitmap, root->left, x - dist_hor * power(2, levels), y + dist_per, dist_hor, dist_per, radius, levels, a, func))
        func(bitmap, x, x - dist_hor * power(2, levels), y, dist_hor, dist_per, radius, num, a);

    free(a);
    a = NULL;

    if (draw(bitmap, root->right, x + dist_hor * power(2, levels), y + dist_per, dist_hor, dist_per, radius, levels, a, func))
        func(bitmap, x, x + dist_hor * power(2, levels), y, dist_hor, dist_per, radius, num, a);

    free(a);

    draw_val(bitmap, x, x + dist_hor * power(2, levels), y, dist_hor, dist_per, radius, num, NULL);

    free(num);

    return 1;
    
}


int get_max_height(AVLNode root, int height)
{
    if (root == NULL)
        return height - 1;

    int l = get_max_height(root->left, height + 1);
    int r = get_max_height(root->right, height + 1);
    
    return (l > r) ? l : r;
}


int main(void)
{
    srand(time(NULL));
    
    AVLTree tree = fill_tree(compare_ints, free);

    
    int levels = get_max_height(tree->root, 1);
    
    avl_print_tree(tree);

    int radius = 20;

    int height = (radius + 20) * levels * 2; 
    int width = (radius + 20) * power(2, levels);

    int dist_hor = width / power(2, levels);
    int dist_per = height / levels;

    int mid = width / 2;

    printf("mx dist h: %d\nmx dist p: %d\n", dist_hor, dist_per);

	Bitmap* bitmap = bm_create(width, height);

    bm_set_color(bitmap, bm_atoi("white"));
    bm_clear(bitmap);
    bm_set_color(bitmap, bm_atoi("black"));


    int x = mid;
    int y = radius + 20;
    


    draw(bitmap, tree->root, x, y, dist_hor, dist_per, radius, levels - 1, NULL, draw_node);
    draw(bitmap, tree->root, x, y, dist_hor, dist_per, radius, levels - 1, NULL, draw_val);
   
    

    bm_save(bitmap, "tree.png");

    bm_free(bitmap);


    avl_destroy(tree);
    
}