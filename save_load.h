#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "Gamestate.h"
#include "boolean.h"

// Function untuk save status permainan ke file
boolean saveGameState(GameState* state, const char* filename);

// Function untuk load status permainan dari file
boolean loadGameState(GameState* state, const char* filename);

#endif 