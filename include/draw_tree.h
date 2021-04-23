#pragma once
#include "AVLTree.h"
#include "bmp.h"


void draw(Bitmap* bitmap, AVLNode root, int x, int y, int dist_hor, int dist_per, int radius, int levels);


int get_max_height(AVLNode root, int height);


int power(int a, int b);