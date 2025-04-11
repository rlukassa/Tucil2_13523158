#ifndef QUADTREE_H
#define QUADTREE_H

#include "image.h"
#include "utils.h"

// Struktur QuadTreeNode
typedef struct QuadTreeNode {
    int x, y;          // Koordinat blok (pojok kiri atas)
    int size;          // Ukuran blok (panjang sisi)
    double color[3];   // Warna rata-rata (R, G, B)
    struct QuadTreeNode *topLeft, *topRight, *bottomLeft, *bottomRight; // Sub-simpul
} QuadTreeNode;

// Fungsi untuk membangun Quadtree
QuadTreeNode *buildQuadTree(Image *img, int x, int y, int size, int minBlockSize, double threshold, int method);

// Fungsi untuk membebaskan memori Quadtree
void freeQuadTree(QuadTreeNode *node);

// Fungsi tambahan
int quadTreeDepth(QuadTreeNode *node);
int quadTreeNodeCount(QuadTreeNode *node);
void applyQuadTreeToImage(QuadTreeNode *node, Pixel **pixels);

#endif