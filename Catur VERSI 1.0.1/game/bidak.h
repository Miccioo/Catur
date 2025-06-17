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
    TIDAK_ADA  // Empty square
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

// Function to initialize a piece
void initBidak(Bidak *bidak, TipeBidak tipe, WarnaBidak warna, int x, int y, int id);

// Function to print piece info (debugging)
void printBidak(Bidak bidak);

// Function to print piece with color
void printBidakColor(Bidak b, int row_visual, int cell_x);

// New function: Returns the visual line string for a piece
// We need to define the visual height and width here
#define BIDAK_VISUAL_HEIGHT 15 // Max height of King
#define BIDAK_VISUAL_WIDTH 21  // Cell width adjusted for square display

const char* getBidakVisualLine(TipeBidak tipe, int line_idx);

// Function to display graphical representation of a piece (individual)
void tampilkanBidakVisual(TipeBidak tipe, WarnaBidak warna);

#endif
