#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "..\game\papan.h"
#include "..\game\player.h"
#include "..\io\menu.h"
#include "..\core\chess_move.h"
#include "..\core\boolean.h"

typedef struct {
    Papan papan;
    Player* pemainPutih;
    Player* pemainHitam;
    Player* giliran;
    MoveNode* history; // stack of moves
    int skakPutih;
    int skakHitam;
    int skakmat;
    int remis;
    int langkahTanpaGerak;
    Bidak* bidakEnPassant;
} GameState;

// Inisialisasi awal game (reset papan, setup pemain, set giliran pertama)
void initGameState(GameState* state, Player* putih, Player* hitam);

// Copy GameState (digunakan AI untuk simulasi langkah)
void copyGameState(GameState* dest, GameState* src);

// Cek kondisi akhir (skakmat, remis, pat)
boolean isGameOver(GameState* state);

// Tukar giliran pemain
void switchTurn(GameState* state);

// Terapkan langkah pada game state (memindahkan bidak, menyimpan histori)
void applyMove(GameState* state, Move* move);

// Undo langkah terakhir
void undoMove(GameState* state);

#endif
