#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "..\game\Gamestate.h"

void startChess();

// Fungsi terpisah untuk mode versus
VersusOption handleVersusMode(int termWidth);

// Fungsi terpisah untuk tipe game
void handleGameType(int termWidth, GameType type);

void classicChess(GameType type, VersusOption mode);
void evolveChess(GameType type, VersusOption mode);
void zombieChess(GameType type, VersusOption mode);

void modePVP(VersusOption mode);
void modePVE(VersusOption mode);
void modeEVSE(VersusOption mode);

//void showNewGameOption(int termWidth, Papan papan);
#endif
