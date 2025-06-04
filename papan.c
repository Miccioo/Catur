#include "papan.h"
#include <stdio.h>
#include <stdlib.h>

void initPapan(Papan *papan) {
    // Inisialisasi papan kosong
    int y, x;
    for (y = 0; y < UKURAN_PAPAN; y++) {
        for (x = 0; x < UKURAN_PAPAN; x++) {
            Bidak kosong;
            initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, x, y, -1);
            papan->grid[y][x] = kosong;
        }
    }
    
    // Inisialisasi bidak putih
    int idCounter = 0;
    
    // Benteng
    initBidak(&(papan->grid[0][0]), BENTENG, PUTIH, 0, 0, idCounter++);
    initBidak(&(papan->grid[0][7]), BENTENG, PUTIH, 7, 0, idCounter++);
    
    // Kuda
    initBidak(&(papan->grid[0][1]), KUDA, PUTIH, 1, 0, idCounter++);
    initBidak(&(papan->grid[0][6]), KUDA, PUTIH, 6, 0, idCounter++);
    
    // Gajah
    initBidak(&(papan->grid[0][2]), GAJAH, PUTIH, 2, 0, idCounter++);
    initBidak(&(papan->grid[0][5]), GAJAH, PUTIH, 5, 0, idCounter++);
    
    // Menteri
    initBidak(&(papan->grid[0][3]), MENTERI, PUTIH, 3, 0, idCounter++);
    
    // Raja
    initBidak(&(papan->grid[0][4]), RAJA, PUTIH, 4, 0, idCounter++);
    
    // Pion
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[1][x]), PION, PUTIH, x, 1, idCounter++);
    }
    
    // Inisialisasi bidak hitam
    // Benteng
    initBidak(&(papan->grid[7][0]), BENTENG, HITAM, 0, 7, idCounter++);
    initBidak(&(papan->grid[7][7]), BENTENG, HITAM, 7, 7, idCounter++);
    
    // Kuda
    initBidak(&(papan->grid[7][1]), KUDA, HITAM, 1, 7, idCounter++);
    initBidak(&(papan->grid[7][6]), KUDA, HITAM, 6, 7, idCounter++);
    
    // Gajah
    initBidak(&(papan->grid[7][2]), GAJAH, HITAM, 2, 7, idCounter++);
    initBidak(&(papan->grid[7][5]), GAJAH, HITAM, 5, 7, idCounter++);
    
    // Menteri
    initBidak(&(papan->grid[7][3]), MENTERI, HITAM, 3, 7, idCounter++);
    
    // Raja
    initBidak(&(papan->grid[7][4]), RAJA, HITAM, 4, 7, idCounter++);
    
    // Pion
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[6][x]), PION, HITAM, x, 6, idCounter++);
    }
}

void printPapan(Papan papan) {
    printf("\n   a b c d e f g h\n");
    printf("  +---------------+\n");
    int y, x;
    
    for (y = 0; y < UKURAN_PAPAN; y++) {
        printf("%d |", 8 - y);
        for (x = 0; x < UKURAN_PAPAN; x++) {
            Bidak b = papan.grid[y][x];
            
            char simbol;
            switch(b.tipe) {
                case PION:      simbol = 'P'; break;
                case BENTENG:   simbol = 'B'; break;
                case KUDA:     simbol = 'K'; break;
                case GAJAH:    simbol = 'G'; break;
                case MENTERI:   simbol = 'M'; break;
                case RAJA:      simbol = 'R'; break;
                default:        simbol = '.'; break;
            }
            
            // Huruf kecil untuk hitam
            if (b.warna == HITAM && b.tipe != TIDAK_ADA) {
                simbol += 32; // Konversi ke lowercase
            }
            
            printf("%c ", simbol);
        }
        printf("| %d\n", 8 - y);
    }
    
    printf("  +---------------+\n");
    printf("   a b c d e f g h\n\n");
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
        papan->grid[yAwal][xAwal] = kosong;
        
        // Pindahkan ke posisi tujuan
        setBidakAt(papan, bidak, xTujuan, yTujuan);
    }
}
