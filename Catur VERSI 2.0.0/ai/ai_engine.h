// ai_engine.h
#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "..\game\Gamestate.h" // For GameState, Move
#include "..\core\chess_move.h" // For Move if not fully in Gamestate.h
#include "..\core\evaluate_system.h" // For evaluateState function
#include "stdlib.h"             // For malloc, free, NULL
#include "..\core\boolean.h"    // For boolean type (assuming this defines 'boolean' and related values)
#include <limits.h>             // For INT_MIN, INT_MAX

#define MAX_CHILDREN 64 // Max legal moves per position

typedef struct tElmtTree* address;
typedef struct tElmtTree {
    GameState state;
    int value;
    Move langkah;
    address parent;
    address children[MAX_CHILDREN];
    int jumlahAnak;
    int kedalaman;
} ElmtTree; // Renamed from tElmtTree to ElmtTree for clarity in struct definition

typedef struct {
	address root;
	int maxKedalaman;
	int jumlahNodeSekarang;
	boolean isMaximizingPlayer;
} GameTree;

// Checks if the GameTree is empty (not used but declared)
boolean isEmptyTree(GameTree X);

GameTree* createGameTree(GameState* rootState, int maxKedalaman, boolean isMaximizingPlayer);

// Creates and initializes a single node for the game tree.
address createNode(GameState* state, Move langkah, address parent, int kedalaman);

int minimax(address node, int depth, boolean isMaximizingPlayer, int alpha, int beta);

Move getBestMove(GameTree* tree); // Corrected and uncommented

#endif // AI_ENGINE_H
