#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "..\game\Gamestate.h"

void startChess();

// Fungsi terpisah untuk tipe game
void handleGameType(int termWidth, GameType type);

void classicChess(GameType type, VersusOption mode);
void evolveChess(GameType type, VersusOption mode);
void zombieChess(GameType type, VersusOption mode);

GameState modePVP();
GameState modePVE();
GameState modeEVSE();

//void showNewGameOption(int termWidth, Papan papan);
#endif
