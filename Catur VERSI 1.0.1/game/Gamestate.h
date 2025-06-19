#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "..\game\papan.h"
#include "..\game\player.h"
#include "..\io\menu.h"
#include "..\core\chess_move.h"
#include "..\core\boolean.h"
#include "../core/validator.h" // Untuk generateAllValidMoves, isSquareAttacked, findKingPosition


#include <stdlib.h>
#include <string.h>

typedef struct {
    Papan papan;
    Player* pemainPutih;
    Player* pemainHitam;
    Player* giliran;
    MoveNode* history; // stack of moves (linked list based stack)
    int skakPutih; // Flag: 1 jika Putih dalam skak, 0 jika tidak
    int skakHitam; // Flag: 1 jika Hitam dalam skak, 0 jika tidak
    int skakmat; // Flag: 1 jika skakmat, 0 jika tidak
    int remis; // Flag: 1 jika remis, 0 jika tidak
    int langkahTanpaGerak; // Untuk aturan 50-langkah
    // Position enPassantTargetSquare; // Lebih baik menggunakan posisi untuk en passant
    Bidak* bidakEnPassant; // Jika tetap menggunakan pointer, pastikan validasinya ketat
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

// Fungsi tambahan untuk validasi game state
boolean isKingInCheck(GameState* state, WarnaBidak kingColor);
boolean hasLegalMoves(GameState* state, Player* player);


#endif
