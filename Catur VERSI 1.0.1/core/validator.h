#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "boolean.h"
#include "chess_move.h"
#include "..\game\papan.h"
#include "..\game\player.h"
#include "..\game\bidak.h"

// IMPORTANT: NO forward declaration for GameState here.
// We will use a generic pointer type (void*) in prototypes
// and cast it in the .c file. This is a workaround for extreme circular dependencies.

#define MAX_MOVES 256 

// Function prototypes using void* for GameState parameters
boolean isPathClear(Papan papan, Position from, Position to);
boolean isPseudoLegalMove(Papan papan, Move* move, Player* currentPlayer, Bidak* enPassantTargetPawn);
Move* generateAllPseudoLegalMoves(Papan papan, Player* currentPlayer, Bidak* enPassantTargetPawn);
boolean isSquareAttacked(Papan papan, Position targetPos, WarnaBidak attackingColor);
boolean isKingInCheck(Papan papan, WarnaBidak kingColor, Position kingPos);

// Changed GameState* to void* in prototypes.
boolean isLegalMove(void* state_ptr, Move* move); 
Move* generateAllValidMoves(Papan papan, Player* currentPlayer, void* currentState_ptr, Bidak* enPassantTargetPawn); 
boolean isCheckmate(void* state_ptr); 

#endif
