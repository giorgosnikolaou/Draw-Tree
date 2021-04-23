# Draw Tree
 
## Disclaimer

The files: bmp.c, bmp.h are **NOT** mine. <br />
They are from [this](https://github.com/wernsey/bitmap) repository from user [@wernsey](https://github.com/wernsey) and contributors. <br />

## About

Inside GeneralModule there is the program which creates the .png tree reprasentation, with a demo main.c file. <br />

This code needs access to the fields of the nodes and such as it should be implemented inside the tree module. <br />

I modified my AVLTree ADT to demonstrate such an implementation. <br />

It works with any kind of tree that has exacly 2 children, and can be easily modified to color the nodes different colors, for a red - black tree. <br />

## Sample program

To compile the demo main open the initial directory in the terminal and run
```bash
make run
```
To show the steps, while inserting-removing you need to add -DSHOW_STEP onto the CFLAGS. <br />
To only show the tree after the insertion, with the function call from main, you need to add -DDRAW onto the CFLAGS.

## Algorithm

The recursive functions first draws the lines from the parent to its non-NULL children, connecting their centers <br />

Then, since we want the line to begin at the edge we fill the circle with while and then draw its border and place the node's value at the centerl of the circle. <br />

Then just call the function two times for the left and right children respectively with the appropriate distance changes.

