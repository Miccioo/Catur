#ifndef PAPAN_H
#define PAPAN_H

#include "bidak.h"
#include "../core/chess_move.h"
#include "windows.h" // Assuming this is for Windows-specific console functions. Consider conditional compilation for cross-platform.

#define UKURAN_PAPAN 8

// Struct for the chess board
typedef struct {
    Bidak grid[UKURAN_PAPAN][UKURAN_PAPAN];
} Papan;

// Function to initialize the board
void initPapan(Papan *papan);

// Function to display the board (simple UI)
void printPapan(Papan papan);

// Function to get piece at a specific position
Bidak getBidakAt(Papan papan, int x, int y);

// Function to place piece at a specific position
void setBidakAt(Papan *papan, Bidak bidak, int x, int y);

// Function to move a piece
void pindahkanBidak(Papan *papan, Move* move);

// Function to check if a position is empty
boolean isEmptyBidak(Papan* papan, int x, int y);

// New: Function to find the position of a specific king
Position findKingPosition(Papan papan, WarnaBidak kingColor);

// New: Function to get the value of a piece
int getBidakValue(TipeBidak tipe);

#endif
