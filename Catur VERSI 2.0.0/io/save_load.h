#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "../game/Gamestate.h"

// Menyimpan state permainan ke file
// Return 1 jika sukses, 0 jika gagal
int saveGame(const char* filename, const GameState* state);

// Memuat state permainan dari file
// Return 1 jika sukses, 0 jika gagal
int loadGame(const char* filename, GameState* state);

#endif // SAVE_LOAD_H