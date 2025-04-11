#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "image.h"
#include "quadtree.h"

#define PIXEL_MAX 255


bool isValidImageFormat(const char *filename); // Memeriksa apakah format file gambar valid
bool fileExists(const char *path); // Memeriksa apakah file ada
const char *getFilenameFromPath(const char *path); // Mendapatkan nama file dari path
double compressedPercentage(Image *img, int nodeCount); // Menghitung persentase kompresi
void calculateAverageColor(Pixel **pixels, int x, int y, int size, double *avgR, double *avgG, double *avgB); // Menghitung rata-rata warna
double calculateError(Pixel **pixels, int x, int y, int size, int method); // Menghitung error berdasarkan metode

#endif