#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "..\game\Gamestate.h"
#include "../ai/ai_engine.h"  // Tambahkan ini untuk GameTree, minimax, getBestMove
#include "../game/player.h"    // Tambahkan ini untuk Player
#include "../game/papan.h"     // Tambahkan ini untuk Papan
#include "../game/Gamestate.h" // Tambahkan ini untuk GameState, Move, Position, Bidak, isGameOver, applyMove
#include "../core/validator.h" // Tambahkan ini untuk isValidMove
#include <ctype.h> // For tolower
#include <stdio.h> // For sprintf, fgets, sscanf, stdin
#include <string.h> // For strcpy
#include <stdlib.h> // For free

void startChess();

// Fungsi terpisah untuk tipe game
void handleGameType(int termWidth, GameType type);

void classicChess(GameType type, VersusOption mode);
void evolveChess(GameType type, VersusOption mode);
void zombieChess(GameType type, VersusOption mode);

GameState modePVP();
GameState modePVE();
GameState modeEVE();

//void showNewGameOption(int termWidth, Papan papan);
#endif
