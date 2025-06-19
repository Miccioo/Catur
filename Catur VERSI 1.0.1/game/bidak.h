// bidak.h
#ifndef BIDAK_H
#define BIDAK_H

#include "..\io\color.h"

// Enum for piece type
typedef enum {
    PION,      // Pawn
    BENTENG,   // Rook
    KUDA,      // Knight
    GAJAH,     // Bishop
    MENTERI,   // Queen
    RAJA,      // King

    // New Evolve Piece Types
    PION_BERAT,        // Pion + Pion
    KSATRIA_PIONIR,    // Pion + Kuda
    GAJAH_PENJAGA,     // Pion + Gajah
    BENTENG_PENYERBU,  // Pion + Benteng
    KUDA_BERPELINDUNG, // Kuda + Pion
    KSATRIA_GANDA,     // Kuda + Kuda
    KOMANDAN_MEDAN,    // Kuda + Gajah
    GAJAH_BIJAKSANA,   // Gajah + Pion
    PENGUASA_GARIS_DEPAN, // Gajah + Kuda
    GAJAH_AGUNG,       // Gajah + Gajah
    BENTENG_PIONIR,    // Benteng + Pion

    // New Zombie Piece Types
    ZOMBIE_PION,    //
    ZOMBIE_BENTENG, //
    ZOMBIE_KUDA,    //
    ZOMBIE_GAJAH,   // Dulu ada ZOMBIE_GAJAH_Z, sekarang disatukan
    ZOMBIE_MENTERI, //
    ZOMBIE_RAJA,    //

    TIDAK_ADA, // Empty square
    TOTAL_BIDAK_TYPES // Add this to count total types
} TipeBidak;

// Enum for piece color
typedef enum {
    PUTIH,       // White
    HITAM,       // Black
    TANPA_WARNA  // No color (for empty squares)
} WarnaBidak;

// Struct for chess piece
typedef struct {
    TipeBidak tipe;
    WarnaBidak warna;
    int x;          // Position x (0-7, column)
    int y;          // Position y (0-7, row)
    int hasMoved;   // 0 = never moved, 1 = has moved
    int id;         // Unique ID for the piece
} Bidak;

// Values for each piece type for Evolve mode
// Raja dan Menteri tidak bisa digabungkan di mode ini, jadi tidak ada nilainya
#define NILAI_PION 1
#define NILAI_KUDA 3
#define NILAI_GAJAH 3
#define NILAI_BENTENG 5
#define NILAI_MENTERI 9 
#define NILAI_RAJA 100 // Raja tidak bisa digabungkan, nilai sangat tinggi

// Function to initialize a piece
void initBidak(Bidak *bidak, TipeBidak tipe, WarnaBidak warna, int x, int y, int id);

// Function to print piece info (debugging)
void printBidak(Bidak bidak);

// Fungsi printBidakColor sekarang menerima koordinat papan (board_row, board_col)
void printBidakColor(Bidak b, int row_visual, int board_row, int board_col);

// New function: Returns the visual line string for a piece
// We need to define the visual height and width here
#define BIDAK_VISUAL_HEIGHT 15 // Max height of King
#define BIDAK_VISUAL_WIDTH 21  // Cell width adjusted for square display

extern const char* bidakVisualsArray[TOTAL_BIDAK_TYPES][BIDAK_VISUAL_HEIGHT];
const char* getBidakVisualLine(TipeBidak tipe, int line_idx);

// Function to display graphical representation of a piece (individual)
void tampilkanBidakVisual(TipeBidak tipe, WarnaBidak warna);

#endif // BIDAK_H
