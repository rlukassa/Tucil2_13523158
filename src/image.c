#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image.h"

#include <sys/stat.h>


// Membaca gambar dari file
Image *readImage(const char *filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3); // Paksa 3 channel (RGB)
    if (!data) {
        printf("Error: Unable to load image %s\n", filename);
        return NULL;
    }

    Image *img = (Image *)malloc(sizeof(Image)); // Alokasi memori untuk struktur gambar
    if (!img) {
        printf("Error: Memory allocation failed for image.\n");
        stbi_image_free(data);
        return NULL;
    }

    img->width = width; // Set lebar gambar
    img->height = height; // Set tinggi gambar

    // Alokasi memori untuk matriks piksel
    img->pixels = (Pixel **)malloc(height * sizeof(Pixel *));
    if (!img->pixels) {
        printf("Error: Memory allocation failed for image pixels.\n");
        free(img);
        stbi_image_free(data);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        img->pixels[i] = (Pixel *)malloc(width * sizeof(Pixel));
        if (!img->pixels[i]) {
            printf("Error: Memory allocation failed for row %d of image pixels.\n", i);
            for (int j = 0; j < i; j++) {
                free(img->pixels[j]);
            }
            free(img->pixels);
            free(img);
            stbi_image_free(data);
            return NULL;
        }

        // Salin data piksel
        memcpy(img->pixels[i], data + (i * width * 3), width * 3);
    }

    stbi_image_free(data); // Bebaskan memori data mentah
    return img;
}

// Menyimpan gambar ke file
void saveImage(const char *filename, Image *img) {
    unsigned char *data = (unsigned char *)malloc(img->width * img->height * 3);
    if (!data) {
        printf("Error: Memory allocation failed for image data.\n");
        return;
    }

    // Salin data piksel ke array 1D
    for (int i = 0; i < img->height; i++) {
        memcpy(data + (i * img->width * 3), img->pixels[i], img->width * 3);
    }

    // Simpan gambar sebagai PNG
    if (!stbi_write_png(filename, img->width, img->height, 3, data, img->width * 3)) {
        printf("Error: Failed to save image as PNG.\n");
    } else {
        printf("[SUCCESS] Image saved successfully to %s\n", filename);
    }

    free(data); // Bebaskan memori
}

// Membebaskan memori gambar
void freeImage(Image *img) {
    if (!img) return;

    for (int i = 0; i < img->height; i++) {
        free(img->pixels[i]); // Bebaskan setiap baris piksel
    }
    free(img->pixels); // Bebaskan matriks piksel
    free(img); // Bebaskan struktur gambar
}

long getFileSizeInKB(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size / 1024; // Konversi byte ke KB
    }
    printf("[ERROR] Failed to get file size for: %s\n", filename);
    return -1; // Jika gagal mendapatkan ukuran file
}