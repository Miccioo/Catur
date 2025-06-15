#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "boolean.h"
#include "chess_move.h"
#include "..\game\papan.h"
#include "..\game\player.h"
#include "..\game\bidak.h"

#define MAX_MOVES 256

// Fungsi untuk mengecek jika jalan antara 2 posisi clear (tidak ada bidak)
boolean isPathClear(Papan papan, Position from, Position to);

// Fungsi untuk memvalidasi gerakan bidak
boolean isValidMove(Papan papan, Move* move, Player* currentPlayer);

// Fungsi untuk generate semua gerakan valid untuk player saat ini
Move* generateAllValidMoves(Papan papan, Player* currentPlayer);

#endif
