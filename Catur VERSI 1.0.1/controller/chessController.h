#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "..\game\Gamestate.h"
#include "..\core\chess_move.h" // Perlu ditambahkan untuk Move

void startChess();

// Fungsi terpisah untuk tipe game
void handleGameType(int termWidth, GameType type);

void classicChess(GameType type, VersusOption mode);
void evolveChess(GameType type, VersusOption mode);
void zombieChess(GameType type, VersusOption mode);

GameState modePVP();
GameState modePVE();
GameState modeEVE();

// Fungsi baru untuk menampilkan tabel langkah valid
void displayValidMovesTable(Papan papan, Move* validMoves, int termWidth);

#endif
