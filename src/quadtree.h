#ifndef QUADTREE_H
#define QUADTREE_H

#include "image.h"

typedef struct QuadTreeNode {
    struct QuadTreeNode *topLeft, *topRight, *bottomLeft, *bottomRight; 
    double color[3]; 
} QuadTreeNode;

QuadTreeNode *buildQuadTree(Image *img, int x, int y, int size, int minBlockSize, double threshold, int method);

void freeQuadTree(QuadTreeNode *node);
int quadTreeDepth(QuadTreeNode *node);
int quadTreeNodeCount(QuadTreeNode *node);

#endif