#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"




// cek apakah format file gambar valid (.jpg, .jpeg, .png)  
bool isValidImageFormat(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return false;
    return strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".png") == 0;
}

// Cek apakah file ada di path yang diberikan
bool fileExists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// Cek apakah nama file dapat diekstrak dari path yang diberikan
const char *getFilenameFromPath(const char *path) {
    const char *filename = strrchr(path, '/');
    if (!filename) filename = strrchr(path, '\\');
    return filename ? filename + 1 : path;
}

// hitung persentase kompresi berdasarkan ukuran gambar asli dan ukuran quadtree
double calculateCompressedPercentage(const char *originalFile, const char *compressedFile) {
    long originalSize = getFileSizeInKB(originalFile);
    long compressedSize = getFileSizeInKB(compressedFile);

    if (originalSize <= 0 || compressedSize <= 0) {
        printf("[ERROR] Failed to get file sizes.\n");
        return 0.0;
    }

    return ((double)(originalSize - compressedSize) / originalSize) * 100.0;
}

// hitung rata-rata warna dari blok piksel
void calculateAverageColor(Pixel **pixels, int x, int y, int size, double *avgR, double *avgG, double *avgB) {
    if (size <= 0 || pixels == NULL) {
        *avgR = *avgG = *avgB = 0.0;
        return;
    }

    double sumR = 0, sumG = 0, sumB = 0;
    int count = size * size;

    for (int i = y; i < y + size; i++) {
        for (int j = x; j < x + size; j++) {
            sumR += pixels[i][j].r;
            sumG += pixels[i][j].g;
            sumB += pixels[i][j].b;
        }
    }

    *avgR = sumR / count;
    *avgG = sumG / count;
    *avgB = sumB / count;
}

void applyQuadTreeToImage(QuadTreeNode *node, Pixel **pixels) {
    if (!node) return;

    if (!node->topLeft && !node->topRight && !node->bottomLeft && !node->bottomRight) {
        // Simpul daun: Terapkan warna rata-rata ke seluruh blok
        for (int i = node->y; i < node->y + node->size; i++) {
            for (int j = node->x; j < node->x + node->size; j++) {
                pixels[i][j].r = node->color[0];
                pixels[i][j].g = node->color[1];
                pixels[i][j].b = node->color[2];
            }
        }
    } else {
        // Rekursif ke sub-blok
        applyQuadTreeToImage(node->topLeft, pixels);
        applyQuadTreeToImage(node->topRight, pixels);
        applyQuadTreeToImage(node->bottomLeft, pixels);
        applyQuadTreeToImage(node->bottomRight, pixels);
    }
}

// hitung error berdasarkan metode yang dipilih
double calculateError(Pixel **pixels, int x, int y, int size, int method) {
    double avgR = 0, avgG = 0, avgB = 0;
    int count = size * size ; 
    calculateAverageColor(pixels, x, y, size, &avgR, &avgG, &avgB);

    switch (method) {
        case 1: { // Variance
            double varianceR = 0, varianceG = 0, varianceB = 0;
            int count = 0;

            for (int i = y; i < y + size; i++) {
                for (int j = x; j < x + size; j++) {
                    varianceR += pow(pixels[i][j].r - avgR, 2);
                    varianceG += pow(pixels[i][j].g - avgG, 2);
                    varianceB += pow(pixels[i][j].b - avgB, 2);
                    count++;
                }
            }

            varianceR /= count;
            varianceG /= count;
            varianceB /= count;

            return (varianceR + varianceG + varianceB) / 3.0;
        }

        case 2: { // Mean Absolute Deviation (MAD)
            double madR = 0, madG = 0, madB = 0;
            int count = 0;

            for (int i = y; i < y + size; i++) {
                for (int j = x; j < x + size; j++) {
                    madR += fabs(pixels[i][j].r - avgR);
                    madG += fabs(pixels[i][j].g - avgG);
                    madB += fabs(pixels[i][j].b - avgB);
                    count++;
                }
            }

            madR /= count;
            madG /= count;
            madB /= count;

            return (madR + madG + madB) / 3.0;
        }

        case 3: { // Max Pixel Difference
            int maxR = 0, maxG = 0, maxB = 0;
            int minR = 255, minG = 255, minB = 255;

            for (int i = y; i < y + size; i++) {
                for (int j = x; j < x + size; j++) {
                    if (pixels[i][j].r > maxR) maxR = pixels[i][j].r;
                    if (pixels[i][j].g > maxG) maxG = pixels[i][j].g;
                    if (pixels[i][j].b > maxB) maxB = pixels[i][j].b;

                    if (pixels[i][j].r < minR) minR = pixels[i][j].r;
                    if (pixels[i][j].g < minG) minG = pixels[i][j].g;
                    if (pixels[i][j].b < minB) minB = pixels[i][j].b;
                }
            }

            double diffR = maxR - minR;
            double diffG = maxG - minG;
            double diffB = maxB - minB;

            return (diffR + diffG + diffB) / 3.0;
        }

        case 4: { // Entropy
            double histogramR[256] = {0}, histogramG[256] = {0}, histogramB[256] = {0};
            int count = 0;

            for (int i = y; i < y + size; i++) {
                for (int j = x; j < x + size; j++) {
                    histogramR[pixels[i][j].r]++;
                    histogramG[pixels[i][j].g]++;
                    histogramB[pixels[i][j].b]++;
                    count++;
                }
            }

            double entropyR = 0, entropyG = 0, entropyB = 0;

            for (int i = 0; i < 256; i++) {
                if (histogramR[i] > 0) {
                    double p = histogramR[i] / count;
                    entropyR -= p * log2(p);
                }
                if (histogramG[i] > 0) {
                    double p = histogramG[i] / count;
                    entropyG -= p * log2(p);
                }
                if (histogramB[i] > 0) {
                    double p = histogramB[i] / count;
                    entropyB -= p * log2(p);
                }
            }

            return (entropyR + entropyG + entropyB) / 3.0;
        }

        case 5: { // SSIM
            const double C1 = 6.5025, C2 = 58.5225;

            double avgR2 = 0, avgG2 = 0, avgB2 = 0;
            for (int i = y; i < y + size; i++) {
                for (int j = x; j < x + size; j++) {
                    avgR2 += round(pixels[i][j].r / 20.0) * 20.0;
                    avgG2 += round(pixels[i][j].g / 20.0) * 20.0;
                    avgB2 += round(pixels[i][j].b / 20.0) * 20.0;
                }
            }
            avgR2 /= count; avgG2 /= count; avgB2 /= count;

            double varR1 = 0, varG1 = 0, varB1 = 0;
            double varR2 = 0, varG2 = 0, varB2 = 0;
            double covR = 0, covG = 0, covB = 0;

            for (int i = y; i < y + size; i++) {
                for (int j = x; j < x + size; j++) {
                    double r1 = pixels[i][j].r;
                    double g1 = pixels[i][j].g;
                    double b1 = pixels[i][j].b;

                    double r2 = round(r1 / 20.0) * 20.0;
                    double g2 = round(g1 / 20.0) * 20.0;
                    double b2 = round(b1 / 20.0) * 20.0;

                    r2 = fmin(fmax(r2, 0), PIXEL_MAX);
                    g2 = fmin(fmax(g2, 0), PIXEL_MAX);
                    b2 = fmin(fmax(b2, 0), PIXEL_MAX);

                    varR1 += pow(r1 - avgR, 2); varR2 += pow(r2 - avgR2, 2);
                    varG1 += pow(g1 - avgG, 2); varG2 += pow(g2 - avgG2, 2);
                    varB1 += pow(b1 - avgB, 2); varB2 += pow(b2 - avgB2, 2);

                    covR += (r1 - avgR) * (r2 - avgR2);
                    covG += (g1 - avgG) * (g2 - avgG2);
                    covB += (b1 - avgB) * (b2 - avgB2);
                }
            }

            varR1 /= count; varG1 /= count; varB1 /= count;
            varR2 /= count; varG2 /= count; varB2 /= count;
            covR /= count;  covG /= count;  covB /= count;

            double ssimR = ((2 * avgR * avgR2 + C1) * (2 * covR + C2)) /
                           ((avgR * avgR + avgR2 * avgR2 + C1) * (varR1 + varR2 + C2));
            double ssimG = ((2 * avgG * avgG2 + C1) * (2 * covG + C2)) /
                           ((avgG * avgG + avgG2 * avgG2 + C1) * (varG1 + varG2 + C2));
            double ssimB = ((2 * avgB * avgB2 + C1) * (2 * covB + C2)) /
                           ((avgB * avgB + avgB2 * avgB2 + C1) * (varB1 + varB2 + C2));

            double ssim = (ssimR + ssimG + ssimB) / 3.0;
            return 1.0 - ssim; // SSIM to error
        }
    }
}
