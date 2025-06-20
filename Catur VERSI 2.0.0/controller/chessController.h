#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "../ai/ai_engine.h" // Assuming ai_engine uses generateAllValidMoves and minimax correctly
#include "../game/player.h"
#include "../game/papan.h"
#include "../core/validator.h"
#include "../user/account.h"
#include "../io/save_load.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For malloc, free
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

// New: Function to handle Evolve mode actions (combining pieces)
boolean handleEvolveAction(GameState* state, int termWidth);

void classicChessLoaded(GameState* state, int termWidth);
#endif
