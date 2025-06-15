#ifndef EVALUATE_SYSTEM_H
#define EVALUATE_SYSTEM_H

#include "../game/Gamestate.h" 
#include "../core/chess_move.h"
#include "stdlib.h"             
#include "../core/boolean.h"

int evaluateState(GameState* state);
int evaluateMaterial(Gamestate* state);
int evaluatePositional(Gamestate* state);
int evaluateKingSafety(Gamestate* state);
int evaluateMobility(GameState* state);
int evaluatePawnStructure(Gamestate* state);
GamePhase determineGamePhase(GameState* state);


#endif
