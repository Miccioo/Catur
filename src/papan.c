#include "papan.h"
#include <stdio.h>
#include <stdlib.h>

void initPapan(Papan *papan) {
    // Inisialisasi papan kosong
    int x, y; 
    for (y = 0; y < UKURAN_PAPAN; y++) {
        for (x = 0; x < UKURAN_PAPAN; x++) {
            Bidak kosong;
            initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, x, y, -1);
            papan->grid[y][x] = kosong;
        }
    }
    
    // Inisialisasi bidak putih
    int idCounter = 0; 
    
    // Baris 0 (indeks 0): Benteng, Kuda, Gajah, Menteri, Raja, Gajah, Kuda, Benteng (Putih)
    initBidak(&(papan->grid[0][0]), BENTENG, PUTIH, 0, 0, idCounter++);
    initBidak(&(papan->grid[0][1]), KUDA, PUTIH, 1, 0, idCounter++);
    initBidak(&(papan->grid[0][2]), GAJAH, PUTIH, 2, 0, idCounter++);
    initBidak(&(papan->grid[0][3]), MENTERI, PUTIH, 3, 0, idCounter++);
    initBidak(&(papan->grid[0][4]), RAJA, PUTIH, 4, 0, idCounter++);
    initBidak(&(papan->grid[0][5]), GAJAH, PUTIH, 5, 0, idCounter++);
    initBidak(&(papan->grid[0][6]), KUDA, PUTIH, 6, 0, idCounter++);
    initBidak(&(papan->grid[0][7]), BENTENG, PUTIH, 7, 0, idCounter++);
    
    // Baris 1 (indeks 1): Pion Putih
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[1][x]), PION, PUTIH, x, 1, idCounter++);
    }
    
    // Inisialisasi bidak hitam
    // Baris 7 (indeks 7): Benteng, Kuda, Gajah, Menteri, Raja, Gajah, Kuda, Benteng (Hitam)
    initBidak(&(papan->grid[7][0]), BENTENG, HITAM, 0, 7, idCounter++);
    initBidak(&(papan->grid[7][1]), KUDA, HITAM, 1, 7, idCounter++);
    initBidak(&(papan->grid[7][2]), GAJAH, HITAM, 2, 7, idCounter++);
    initBidak(&(papan->grid[7][3]), MENTERI, HITAM, 3, 7, idCounter++);
    initBidak(&(papan->grid[7][4]), RAJA, HITAM, 4, 7, idCounter++);
    initBidak(&(papan->grid[7][5]), GAJAH, HITAM, 5, 7, idCounter++);
    initBidak(&(papan->grid[7][6]), KUDA, HITAM, 6, 7, idCounter++);
    initBidak(&(papan->grid[7][7]), BENTENG, HITAM, 7, 7, idCounter++);
    
    // Baris 6 (indeks 6): Pion Hitam
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[6][x]), PION, HITAM, x, 6, idCounter++);
    }
}

void printPapan(Papan papan) {
    // Menggunakan macro dari bidak.h untuk konsistensi
    const int VISUAL_HEIGHT = BIDAK_VISUAL_HEIGHT;
    const int CELL_WIDTH = BIDAK_VISUAL_WIDTH;

    int x, y, i, row_visual; 
    char col_char_val; 

    // Cetak label kolom di atas
    printf("\n    "); // 4 spasi awal untuk mengimbangi label baris
    for (col_char_val = 0; col_char_val < UKURAN_PAPAN; col_char_val++) {
        // CELL_WIDTH = 21. Kita ingin karakter di tengah.
        // spasi kiri = (CELL_WIDTH - 1) / 2 = (21-1)/2 = 10
        // spasi kanan = CELL_WIDTH - 1 - spasi_kiri = 21 - 1 - 10 = 10
        printf("          %c          ", 'a' + col_char_val); // 10 spasi, char, 10 spasi
    }
    printf("\n");

    // Cetak garis horizontal atas
    printf("   +"); // Awalan untuk garis horizontal atas
    for (x = 0; x < UKURAN_PAPAN; x++) {
        for (i = 0; i < CELL_WIDTH; i++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    for (y = 0; y < UKURAN_PAPAN; y++) {
        for (row_visual = 0; row_visual < VISUAL_HEIGHT; row_visual++) {
            if (row_visual == VISUAL_HEIGHT / 2) { 
                printf("%d  |", 8 - y); // Angka baris di tengah tinggi sel
            } else {
                printf("   |"); // Padding untuk angka baris
            }

            for (x = 0; x < UKURAN_PAPAN; x++) {
                Bidak b = papan.grid[y][x];
				
                printBidakColor(b, row_visual, x);
                
                printf("|");
                printf(RESETCOLOR);
            }
            
            if (row_visual == VISUAL_HEIGHT / 2) { 
                 printf(" %d\n", 8 - y); // Angka baris di tengah tinggi sel
            } else {
                 printf("\n");
            }
        }
        // Cetak garis horizontal antar baris
        printf("   +"); 
        for (x = 0; x < UKURAN_PAPAN; x++) {
            for (i = 0; i < CELL_WIDTH; i++) {
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }

    // Cetak label kolom di bawah
    printf("    "); 
    for (col_char_val = 0; col_char_val < UKURAN_PAPAN; col_char_val++) {
        printf("          %c          ", 'a' + col_char_val); // 10 spasi, char, 10 spasi
    }
    printf("\n\n");
}


Bidak getBidakAt(Papan papan, int x, int y) {
    if (x >= 0 && x < UKURAN_PAPAN && y >= 0 && y < UKURAN_PAPAN) {
        return papan.grid[y][x];
    } else {
        Bidak kosong;
        initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
        return kosong;
    }
}

void setBidakAt(Papan *papan, Bidak bidak, int x, int y) {
    if (x >= 0 && x < UKURAN_PAPAN && y >= 0 && y < UKURAN_PAPAN) {
        bidak.x = x;
        bidak.y = y;
        papan->grid[y][x] = bidak;
    }
}

void pindahkanBidak(Papan *papan, int xAwal, int yAwal, int xTujuan, int yTujuan) {
    if (xAwal >= 0 && xAwal < UKURAN_PAPAN && yAwal >= 0 && yAwal < UKURAN_PAPAN &&
        xTujuan >= 0 && xTujuan < UKURAN_PAPAN && yTujuan >= 0 && yTujuan < UKURAN_PAPAN) {
        
        Bidak bidak = papan->grid[yAwal][xAwal];
        bidak.hasMoved = 1;
        
        // Kosongkan posisi awal
        Bidak kosong;
        initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, xAwal, yAwal, -1);
        // Perbaiki typo di baris ini: xAawal menjadi xAwal
        papan->grid[yAwal][xAwal] = kosong; 
        
        // Pindahkan ke posisi tujuan
        setBidakAt(papan, bidak, xTujuan, yTujuan);
    }
}
