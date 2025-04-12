# 📦 Quadtree-Based Image Compression - Divide and Conquer Algorithm Implementation

🎯 **Tugas Kecil 2 - Strategi Algoritma**  
📌 **Institut Teknologi Bandung, Informatika, K-3**  

---

## ‼️ **Tentang Projek Ini**  

Program ini mengimplementasikan algoritma **kompresi gambar berbasis Quadtree** menggunakan bahasa **C** dan pendekatan **Divide and Conquer**.  
Tujuannya adalah untuk mengurangi ukuran gambar dengan tetap mempertahankan kualitas visual sebisa mungkin melalui pembagian blok berdasarkan homogenitas warna.

## 🌲 **Apa Itu Quadtree**  

Quadtree adalah struktur data pohon yang digunakan untuk membagi ruang dua dimensi menjadi empat bagian kuadran berulang kali. Pada kompresi gambar, quadtree digunakan untuk merepresentasikan area homogen dengan satu node, sehingga mengurangi kebutuhan penyimpanan piksel individual.
Dalam konteks program ini:
- Setiap node mewakili sebuah persegi di citra.
- Jika warna dalam satu persegi memiliki deviasi di bawah threshold, maka node tersebut menjadi daun.
- Jika tidak, maka node akan membagi diri menjadi 4 sub-node dan proses diulang.

---

## 🚀 Fitur Utama

- Dukungan format input **.jpg, .jpeg, .png**
- Pemilihan metode perhitungan error (Variance, MAD, Max Deviation, Entropy, SSIM)
- Threshold dan ukuran blok minimum dapat dikonfigurasi
- Target rasio kompresi dinamis
- Statistik hasil kompresi (kedalaman quadtree, jumlah simpul, dll.)
- Output file bisa disesuaikan atau di-*default*-kan secara otomatis
- Dukungan output GIF (bonus) - masih dalam pengembangan

---

## 🗂️ Struktur Folder

```

|__ bin/
├── doc/
├── src/                  # Folder utama berisi file source code
│   ├── output/           # Default base hasil kompresi apabila salah
│   ├── main.c
│   ├── image.c
│   ├── image.h
│   ├── quadtree.c
│   ├── quadtree.h
│   ├── utils.c
│   ├── utils.h
│   ├── stb_image.h
│   ├── stb_write_image.h
│  
├── test/             # Testing file untuk kompresi
│   └── images.jpeg
├          
└── README.md             # Dokumentasi ini
```

---

## ▶️ Cara Menjalankan

*Clone* repository
```bash
git clone https://github.com/rlukassa/Tucil2_13523158
```

Masuk kedalam struktur direktori project ini

```bash
cd src 
gcc -o [nama exec] main.c image.c utils.c quadtree.c
./[nama exec]
```

Kemudian ikuti input interaktif:

1. Alamat absolut gambar input (.jpg, .jpeg, .png)  
2. Metode error (1–5):  
   - `1 = Variance`  
   - `2 = Mean Absolute Deviation (MAD)`  
   - `3 = Max Pixel Difference`  
   - `4 = Entropy`  
   - `5 = SSIM (bonus)`  
3. Threshold (menyesuaikan dengan metode error)  
4. Ukuran blok minimum  
5. Target rasio kompresi (0 = nonaktif)  
6. Alamat output gambar hasil kompresi  
7. Alamat output GIF (opsional) - masih dalam pengembangan

Jika input tidak valid, program akan memberikan feedback/error handling dan mengulang input.

---

## 📊 Contoh Output

```
============================================================
                 IMAGE COMPRESSION TOOL
============================================================
------Implementation of Divide and Conquer Algorithm-------
-----------------Author: Lukas Raja Agripa-----------------
============================================================

[INFO] .gif file masih dalam tahap pengembangan ~ Lukas
[INPUT] Enter the absolute path of the image to compress (jpeg, jpg, png): xx/xxx/yy/zz.jpeg
[OK] Yey ! File found!
[INPUT] Enter error calculation method:
        1. Variance
        2. MAD
        3. Max Pixel Difference
        4. Entropy
        5. SSIM
        Choice: 5
[INPUT] Enter threshold value (non-negative): 0.001
[INPUT] Enter minimum block size (positive integer): 10
[INPUT] Enter compression target percentage (0 to disable, 1.0 = 100%): 0 
[INPUT] Enter output image path: xx.jpeg
[INFO] Oh no ! You forgot to give the absolute path program.
[INFO] But That's okay ! -> Using default: Compressed_xx.jpeg

[PROCESS] Compressing image... Please wait...

[SUCCESS] Image saved successfully to ./output/Compressed_xx.jpeg

[SUMMARY] Compression Summary:
------------------------------------------------------------
Original Image Size   : m KB
Final Image Size      : n KB 
Compression Ratio     : yxz%
QuadTree Depth        : 6
Nodes Created         : 1289
Execution Time        : xxx ms
------------------------------------------------------------
Thank you for using my compression tool!
```

---

## 💡 Catatan Tambahan

- Jika nama file output tidak disertakan, program akan otomatis memberi nama `Compressed_[nama_file_input]`
- Jika path output tidak valid, file akan disimpan di folder `output/`
- stb_image_write.h dan stb_image.h diperlukan ! Untuk menyimpan file lebih aman dan mudah.

---

## 📚 Referensi

- Quadtree Compression Algorithm  
- SSIM: https://en.wikipedia.org/wiki/Structural_similarity  
- Entropy in Image Compression  
- Image Processing in C (libjpeg, stb_image)

---

## 🧠 Pengembang

Created with 📸 by:  

<p align="center">
  <table>
    <tr align="center">
      <td>
        <img src="https://github.com/rlukassa.png" width="100" height="100"><br>
        <b>Lukas Raja Agripa</b><br>
        13523158
      </td>
    </tr>
   </table>
</p>


