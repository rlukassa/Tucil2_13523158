#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "image.h"
#include "quadtree.h"
#include "utils.h"

void printHeader() {
    printf("============================================================\n");
    printf("                 IMAGE COMPRESSION TOOL\n");
    printf("============================================================\n");
    printf("------Implementation of Divide and Conquer Algorithm-------\n");
    printf("-----------------Author: Lukas Raja Agripa-----------------\n");
    printf("============================================================\n\n");
}

void printSummary(Image *img, QuadTreeNode *qt, double execTimeMs, double compressionTarget, int method, double threshold, int minBlockSize, const char *inputPath, const char *outputPath) {
    long originalSizeKB = getFileSizeInKB(inputPath);
    long compressedSizeKB = getFileSizeInKB(outputPath);

    printf("\n[SUMMARY] Compression Summary:\n");
    printf("------------------------------------------------------------\n");
    printf("Original Image Size   : %ld KB\n", originalSizeKB);
    printf("Compressed Image Size : %ld KB\n", compressedSizeKB);
    printf("Compression Ratio     : %.2f%%\n", calculateCompressedPercentage(inputPath, outputPath));
    printf("QuadTree Depth        : %d\n", quadTreeDepth(qt));
    printf("Nodes Created         : %d\n", quadTreeNodeCount(qt));
    printf("Execution Time        : %.2f ms\n", execTimeMs);
    printf("------------------------------------------------------------\n");
    printf("Thank you for using my compression tool!\n\n");
}

int main() {
    char inputImage[256], outputImage[256];
    int method, minBlockSize;
    double threshold, compressionTarget;
    clock_t startTime, endTime;

    printHeader();

    // Input: Path gambar
    do {
        printf("[INFO] .gif file masih dalam tahap pengembangan ~ Lukas\n");
        printf("[INPUT] Enter the absolute path of the image to compress (jpeg, jpg, png): ");
        scanf("%s", inputImage);

        if (!isValidImageFormat(inputImage)) {
            printf("[ERROR] Unsupported file format. Please use a .jpeg, .jpg, or .png\n");
            continue;
        }

        if (!fileExists(inputImage)) {
            printf("[ERROR] File does not exist. Please enter a valid file path.\n");
            continue;
        }

        printf("[OK] Yey ! File found!\n");
        break;
    } while (1);

    // Input: Metode error
    do {
        printf("[INPUT] Enter error calculation method:\n");
        printf("        1. Variance\n");
        printf("        2. MAD\n");
        printf("        3. Max Pixel Difference\n");
        printf("        4. Entropy\n");
        printf("        5. SSIM\n");
        printf("        Choice: ");
        if (scanf("%d", &method) != 1 || method < 1 || method > 5) {
            printf("[ERROR] Invalid method. Enter a number between 1 and 5.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (1);

    // Input: Threshold
    do {
        printf("[INPUT] Enter threshold value (non-negative): ");
        if (scanf("%lf", &threshold) != 1 || threshold < 0) {
            printf("[ERROR] Threshold must be a non-negative number.\n");
            while (getchar() != '\n');
        } 
        if (method == 5 && threshold > 1.0) {
            printf("[WARN] SSIM biasanya bernilai 0.0 - 1.0. Apakah nilai %.2f ini dimaksudkan dalam skala 0-1000?\n", threshold);
            printf("[INPUT] Apakah Anda yakin ingin menggunakan nilai ini? (y/n): ");
            char confirm;
            while (getchar() != '\n'); // Clear the input buffer
            scanf("%c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                break;
            } else {
                continue;
            }
        } else {
            break;
        }
    } while (1);

    Image *img = readImage(inputImage);
    if (!img) {
        printf("[ERROR] Failed to load image. Exiting.\n");
        return 1;
    }

    // Input: Minimum block size
    do {
        printf("[INPUT] Enter minimum block size (positive integer): ");
        if (scanf("%d", &minBlockSize) != 1 || minBlockSize <= 0) {
            printf("[ERROR] Minimum block size must be a positive integer.\n");
            while (getchar() != '\n');
        } else if (minBlockSize > img->width || minBlockSize > img->height) {
            printf("[ERROR] Minimum block size cannot be larger than the image dimensions.\n");
        } else {
            break;
        }
    } while (1);

    // Input: Compression target
    do {
        printf("[INPUT] Enter compression target percentage (0 to disable, 1.0 = 100%%): ");
        if (scanf("%lf", &compressionTarget) != 1 || compressionTarget < 0 || compressionTarget > 1.0) {
            printf("[ERROR] Compression target must be between 0 and 1.0.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (1);

    // Output path
    if (isValidImageFormat(inputImage)) {
        do {
            printf("[INPUT] Enter output image path: ");
            scanf("%s", outputImage);

            char *ext = strrchr(outputImage, '.');
            if (!ext) {
                const char *inputFilename = getFilenameFromPath(inputImage);
                snprintf(outputImage, sizeof(outputImage), "./output/Compressed_%s", inputFilename);
                printf("[INFO] Oh no ! You forgot to give the absolute path program.\n");
                printf("[INFO] But That's okay ! -> Using default: Compressed_%s\n", inputFilename);
            } else if (!isValidImageFormat(outputImage)) {
                printf("[ERROR] Unsupported file format for output image. Use .jpeg, .jpg, or .png\n");
                continue;
            }

            break;
        } while (1);
    }

    printf("\n[PROCESS] Compressing image... Please wait...\n\n");
    startTime = clock();

    QuadTreeNode *qt = buildQuadTree(img, 0, 0, img->width, minBlockSize, threshold, method);
    

    if (!qt) {
        printf("[ERROR] Failed to build quadtree. Exiting.\n");
        freeImage(img);
        return 1;
    }

    applyQuadTreeToImage(qt, img->pixels);
    saveImage(outputImage, img);
    
    double compressionPercentage = calculateCompressedPercentage(inputImage, outputImage);
    if (compressionPercentage >= 0) {
        printf("[INFO] Compression Percentage: %.2f%%\n", compressionPercentage);
    } else {
        printf("[ERROR] Compression percentage calculation failed.\n");
    }

    endTime = clock();
    double execTimeMs = ((double)(endTime - startTime) / CLOCKS_PER_SEC) * 1000;


    printSummary(img, qt, execTimeMs, compressionTarget, method, threshold, minBlockSize, inputImage, outputImage);

    freeQuadTree(qt);
    freeImage(img);
    return 0;
}