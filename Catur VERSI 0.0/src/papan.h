#ifndef PAPAN_H
#define PAPAN_H

#include "bidak.h"

#define UKURAN_PAPAN 8

// Struct untuk papan catur
typedef struct {
    Bidak grid[UKURAN_PAPAN][UKURAN_PAPAN];
} Papan;

// Fungsi untuk inisialisasi papan
void initPapan(Papan *papan);

// Fungsi untuk menampilkan papan (UI sederhana)
void printPapan(Papan papan);

// Fungsi untuk mendapatkan bidak di posisi tertentu
Bidak getBidakAt(Papan papan, int x, int y);

// Fungsi untuk menempatkan bidak di posisi tertentu
void setBidakAt(Papan *papan, Bidak bidak, int x, int y);

// Fungsi untuk memindahkan bidak
void pindahkanBidak(Papan *papan, int xAwal, int yAwal, int xTujuan, int yTujuan);

#endif
