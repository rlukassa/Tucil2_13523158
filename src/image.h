#ifndef IMAGE_H
#define IMAGE_H


typedef struct Pixel {
    unsigned char r, g, b; // Komponen warna merah, hijau, biru
} Pixel;

typedef struct Image {
    int width, height; // Dimensi gambar
    Pixel **pixels;    // Matriks piksel gambar
} Image;

Image *readImage(const char *filename); // Membaca gambar dari file
void saveImage(const char *filename, Image *img); // Menyimpan gambar ke file
void freeImage(Image *img); // Membebaskan memori gambar
#endif