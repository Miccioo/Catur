#include "papan.h"
#include <stdio.h>
#include <stdlib.h> 

void initPapan(Papan *papan) {
    // Initialize empty board
    int x, y; 
    for (y = 0; y < UKURAN_PAPAN; y++) {
        for (x = 0; x < UKURAN_PAPAN; x++) {
            Bidak kosong;
            // Pastikan bidak kosong diinisialisasi dengan TIDAK_ADA dan ID -1
            initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, x, y, -1); 
            papan->grid[y][x] = kosong;
        }
    }
    
    // Initialize black pieces (top of the board, row 0 and 1)
    int idCounter = 0; 
    
    // Row 0 (index 0): Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook (Black)
    initBidak(&(papan->grid[0][0]), BENTENG, HITAM, 0, 0, idCounter++);
    initBidak(&(papan->grid[0][1]), KUDA, HITAM, 1, 0, idCounter++);
    initBidak(&(papan->grid[0][2]), GAJAH, HITAM, 2, 0, idCounter++);
    initBidak(&(papan->grid[0][3]), MENTERI, HITAM, 3, 0, idCounter++);
    initBidak(&(papan->grid[0][4]), RAJA, HITAM, 4, 0, idCounter++);
    initBidak(&(papan->grid[0][5]), GAJAH, HITAM, 5, 0, idCounter++);
    initBidak(&(papan->grid[0][6]), KUDA, HITAM, 6, 0, idCounter++);
    initBidak(&(papan->grid[0][7]), BENTENG, HITAM, 7, 0, idCounter++);
    
    // Row 1 (index 1): Black Pawns
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[1][x]), PION, HITAM, x, 1, idCounter++);
    }
    
    // Initialize white pieces (bottom of the board, row 7 and 6)
    // Row 7 (index 7): Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook (White)
    initBidak(&(papan->grid[7][0]), BENTENG, PUTIH, 0, 7, idCounter++);
    initBidak(&(papan->grid[7][1]), KUDA, PUTIH, 1, 7, idCounter++);
    initBidak(&(papan->grid[7][2]), GAJAH, PUTIH, 2, 7, idCounter++);
    initBidak(&(papan->grid[7][3]), MENTERI, PUTIH, 3, 7, idCounter++);
    initBidak(&(papan->grid[7][4]), RAJA, PUTIH, 4, 7, idCounter++);
    initBidak(&(papan->grid[7][5]), GAJAH, PUTIH, 5, 7, idCounter++);
    initBidak(&(papan->grid[7][6]), KUDA, PUTIH, 6, 7, idCounter++);
    initBidak(&(papan->grid[7][7]), BENTENG, PUTIH, 7, 7, idCounter++);
    
    // Row 6 (index 6): White Pawns
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[6][x]), PION, PUTIH, x, 6, idCounter++);
    }
}

void printPapan(Papan papan) {
    const int VISUAL_HEIGHT = BIDAK_VISUAL_HEIGHT;
    const int CELL_WIDTH = BIDAK_VISUAL_WIDTH;

    int x, y, i, row_visual; 
    char col_char_val; 

    // Print column labels at the top
    printf("\n    "); 
    for (col_char_val = 0; col_char_val < UKURAN_PAPAN; col_char_val++) {
        printf("%*s%c%*s", (CELL_WIDTH / 2) - 1, "", 'a' + col_char_val, (CELL_WIDTH / 2), ""); 
    }
    printf("\n");

    // Print top horizontal line
    printf("   +"); 
    for (x = 0; x < UKURAN_PAPAN; x++) {
        for (i = 0; i < CELL_WIDTH; i++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    for (y = 0; y < UKURAN_PAPAN; y++) { // Iterasi untuk setiap baris papan
        for (row_visual = 0; row_visual < VISUAL_HEIGHT; row_visual++) { // Iterasi untuk setiap baris visual bidak
            if (row_visual == VISUAL_HEIGHT / 2) { 
                printf("%d  |", 8 - y); // Row number centered vertically in cell
            } else {
                printf("   |"); // Padding for row number
            }

            for (x = 0; x < UKURAN_PAPAN; x++) { // Iterasi untuk setiap kolom
                Bidak b = papan.grid[y][x];
				
                printBidakColor(b, row_visual, y, x); 
                
                printf("|"); 
            }
            
            if (row_visual == VISUAL_HEIGHT / 2) { 
                 printf(" %d\n", 8 - y); // Row number centered vertically in cell
            } else {
                 printf("\n");
            }
        }
        // Print horizontal line between rows
        printf("   +"); 
        for (x = 0; x < UKURAN_PAPAN; x++) {
            for (i = 0; i < CELL_WIDTH; i++) {
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }

    // Print column labels at the bottom
    printf("    "); 
    for (col_char_val = 0; col_char_val < UKURAN_PAPAN; col_char_val++) {
        printf("%*s%c%*s", (CELL_WIDTH / 2) - 1, "", 'a' + col_char_val, (CELL_WIDTH / 2), ""); 
    }
    printf("\n\n");
}


Bidak getBidakAt(Papan papan, int x, int y) { // x is column, y is row
    if (x >= 0 && x < UKURAN_PAPAN && y >= 0 && y < UKURAN_PAPAN) {
        return papan.grid[y][x]; // Access grid as [row][column]
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

void pindahkanBidak(Papan *papan, Move* move) {
	int xAwal = move->from.col;
	int yAwal = move->from.row;
	int xTujuan = move->to.col;
	int yTujuan = move->to.row;
    
    int dx = move->to.col - move->from.col; 
	
    if (xAwal >= 0 && xAwal < UKURAN_PAPAN && yAwal >= 0 && yAwal < UKURAN_PAPAN &&
        xTujuan >= 0 && xTujuan < UKURAN_PAPAN && yTujuan >= 0 && yTujuan < UKURAN_PAPAN) {
        
        Bidak bidak = papan->grid[yAwal][xAwal]; 
        
        if (move->isCastling) {
            Bidak rook;
            Position rookFrom, rookTo;
            if (dx == 2) { 
                rook = papan->grid[yAwal][7]; 
                rookFrom = (Position){yAwal, 7};
                rookTo = (Position){yAwal, 5}; 
            } else { 
                rook = papan->grid[yAwal][0]; 
                rookFrom = (Position){yAwal, 0};
                rookTo = (Position){yAwal, 3}; 
            }
            Bidak emptySquareRookFrom;
            initBidak(&emptySquareRookFrom, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
            papan->grid[rookFrom.row][rookFrom.col] = emptySquareRookFrom; 
            setBidakAt(papan, rook, rookTo.col, rookTo.row); 
            papan->grid[rookTo.row][rookTo.col].hasMoved = 1; 
        }
        
        if (move->isEnPassant) {
            Bidak emptySquareCapturedPawn;
            initBidak(&emptySquareCapturedPawn, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
            papan->grid[move->from.row][move->to.col] = emptySquareCapturedPawn;
        }

        bidak.hasMoved = 1;

        if (bidak.tipe == PION && (yTujuan == 0 || yTujuan == 7)) {
            bidak.tipe = move->promotionTo; 
        }

        Bidak emptySquareFrom; 
        initBidak(&emptySquareFrom, TIDAK_ADA, TANPA_WARNA, xAwal, yAwal, -1);
        papan->grid[yAwal][xAwal] = emptySquareFrom; 

        setBidakAt(papan, bidak, xTujuan, yTujuan);
    }
}

boolean isEmptyBidak(Papan* papan, int x, int y) {
	return papan->grid[y][x].tipe == TIDAK_ADA; 
}

// Position findKingPosition(Papan papan, WarnaBidak kingColor) {
//     Position kingPos = {-1, -1}; 
//     int r, c; 
//     for (r = 0; r < UKURAN_PAPAN; r++) {
//         for (c = 0; c < UKURAN_PAPAN; c++) {
//             Bidak b = getBidakAt(papan, c, r);
//             if (b.tipe == RAJA && b.warna == kingColor) {
//                 kingPos = (Position){r, c};
//                 return kingPos;
//             }
//         }
//     }
//     return kingPos; 
// }

int getBidakValue(TipeBidak tipe) {
    switch (tipe) {
        case PION: return NILAI_PION;
        case KUDA: return NILAI_KUDA;
        case GAJAH: return NILAI_GAJAH;
        case BENTENG: return NILAI_BENTENG;
        case MENTERI: return NILAI_MENTERI;
        case RAJA: return NILAI_RAJA; 
        case PION_BERAT: return NILAI_PION + NILAI_PION; 
        case KSATRIA_PIONIR: return NILAI_PION + NILAI_KUDA;
        case GAJAH_PENJAGA: return NILAI_PION + NILAI_GAJAH;
        case BENTENG_PENYERBU: return NILAI_PION + NILAI_BENTENG;
        case KUDA_BERPELINDUNG: return NILAI_KUDA + NILAI_PION;
        case KSATRIA_GANDA: return NILAI_KUDA + NILAI_KUDA;
        case KOMANDAN_MEDAN: return NILAI_KUDA + NILAI_GAJAH;
        case GAJAH_BIJAKSANA: return NILAI_GAJAH + NILAI_PION;
        case PENGUASA_GARIS_DEPAN: return NILAI_GAJAH + NILAI_KUDA;
        case GAJAH_AGUNG: return NILAI_GAJAH + NILAI_GAJAH;
        case BENTENG_PIONIR: return NILAI_BENTENG + NILAI_PION;
        case ZOMBIE_PION:
        case ZOMBIE_BENTENG:
        case ZOMBIE_GAJAH:
        case ZOMBIE_KUDA:
        case ZOMBIE_MENTERI:
        case ZOMBIE_RAJA:
        case TIDAK_ADA: return 0;
        default: return 0; 
    }
}
