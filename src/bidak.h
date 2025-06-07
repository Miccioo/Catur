#ifndef BIDAK_H
#define BIDAK_H

#include "../color.h"

// Enum untuk tipe bidak
typedef enum {
    PION,
    BENTENG,
    KUDA,
    GAJAH,
    MENTERI,
    RAJA,
    TIDAK_ADA  // Untuk kotak kosong
} TipeBidak;

// Enum untuk warna bidak
typedef enum {
    PUTIH,
    HITAM,
    TANPA_WARNA  // Untuk kotak kosong
} WarnaBidak;

// Struct untuk bidak catur
typedef struct {
    TipeBidak tipe;
    WarnaBidak warna;
    int x;          // Posisi x (0-7)
    int y;          // Posisi y (0-7)
    int hasMoved;   // 0 = belum pernah bergerak, 1 = sudah
    int id;         // ID unik untuk bidak
} Bidak;

// Fungsi untuk inisialisasi bidak
void initBidak(Bidak *bidak, TipeBidak tipe, WarnaBidak warna, int x, int y, int id);

// Fungsi untuk menampilkan info bidak (debugging)
void printBidak(Bidak bidak);

// Fungsi Mencetak Bidak beserta warnanya
void printBidakColor(Bidak b, int row_visual, int cell_x);

// Fungsi baru: Mengembalikan string untuk satu baris visual bidak
// Kita perlu mendefinisikan tinggi dan lebar visual di sini
#define BIDAK_VISUAL_HEIGHT 15 // Tinggi maksimum dari Raja
#define BIDAK_VISUAL_WIDTH 21  // Lebar sel disesuaikan untuk tampilan persegi

const char* getBidakVisualLine(TipeBidak tipe, int line_idx);

// Fungsi untuk menampilkan representasi grafis bidak (individu)
void tampilkanBidakVisual(TipeBidak tipe, WarnaBidak warna);


#endif
