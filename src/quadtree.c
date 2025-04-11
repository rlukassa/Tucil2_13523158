#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "utils.h"
#include <math.h>

// Membuat Quadtree secara rekursif
QuadTreeNode *buildQuadTree(Image *img, int x, int y, int size, int minBlockSize, double threshold, int method) {
    double error = calculateError(img->pixels, x, y, size, method);

    if (error <= threshold || size <= minBlockSize) {
        QuadTreeNode *node = (QuadTreeNode *)malloc(sizeof(QuadTreeNode));
        if (!node) {
            printf("Error: Memory allocation failed for QuadTreeNode.\n");
            return NULL;
        }

        calculateAverageColor(img->pixels, x, y, size, &node->color[0], &node->color[1], &node->color[2]);
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;
        return node;
    }

    int halfSize = size / 2;
    QuadTreeNode *node = (QuadTreeNode *)malloc(sizeof(QuadTreeNode));
    if (!node) {
        printf("Error: Memory allocation failed for QuadTreeNode.\n");
        return NULL;
    }

    node->topLeft = buildQuadTree(img, x, y, halfSize, minBlockSize, threshold, method);
    node->topRight = buildQuadTree(img, x + halfSize, y, halfSize, minBlockSize, threshold, method);
    node->bottomLeft = buildQuadTree(img, x, y + halfSize, halfSize, minBlockSize, threshold, method);
    node->bottomRight = buildQuadTree(img, x + halfSize, y + halfSize, halfSize, minBlockSize, threshold, method);

    if (!node->topLeft || !node->topRight || !node->bottomLeft || !node->bottomRight) {
        freeQuadTree(node->topLeft);
        freeQuadTree(node->topRight);
        freeQuadTree(node->bottomLeft);
        freeQuadTree(node->bottomRight);
        free(node);
        return NULL;
    }

    return node;
}

// Membebaskan memori yang digunakan oleh Quadtree
void freeQuadTree(QuadTreeNode *node) {
    if (!node) return;

    // Rekursif membebaskan memori untuk setiap sub-simpul
    freeQuadTree(node->topLeft);
    freeQuadTree(node->topRight);
    freeQuadTree(node->bottomLeft);
    freeQuadTree(node->bottomRight);

    // Membebaskan simpul saat ini
    free(node);
}

// Menghitung kedalaman maksimum Quadtree
int quadTreeDepth(QuadTreeNode *node) {
    if (!node) return 0;

    int depthTopLeft = quadTreeDepth(node->topLeft);
    int depthTopRight = quadTreeDepth(node->topRight);
    int depthBottomLeft = quadTreeDepth(node->bottomLeft);
    int depthBottomRight = quadTreeDepth(node->bottomRight);

    return 1 + fmax(fmax(depthTopLeft, depthTopRight), fmax(depthBottomLeft, depthBottomRight));
}

// Menghitung jumlah simpul dalam Quadtree
int quadTreeNodeCount(QuadTreeNode *node) {
    if (!node) return 0;

    return 1 + quadTreeNodeCount(node->topLeft) +
               quadTreeNodeCount(node->topRight) +
               quadTreeNodeCount(node->bottomLeft) +
               quadTreeNodeCount(node->bottomRight);
}
