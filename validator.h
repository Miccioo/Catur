#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "boolean.h"
#include "chess_move.h"
#include "src/papan.h"
#include "src/player.h"
#include "src/bidak.h"

#define MAX_MOVES 256

// Function to check if path between two positions is clear
boolean isPathClear(Papan papan, Position from, Position to);

// Function to validate if a move is legal
boolean isValidMove(Papan papan, Move* move, Player* currentPlayer);

// Function to generate all valid moves for current player
Move* generateAllValidMoves(Papan papan, Player* currentPlayer);

#endif
