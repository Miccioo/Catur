// ai_engine.h
#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "..\game\Gamestate.h" // For GameState, Move
#include "..\core\chess_move.h" // For Move if not fully in Gamestate.h
#include "stdlib.h"             // For malloc, free, NULL
#include "..\core\boolean.h"    // For boolean type (assuming this defines 'boolean' and related values)
// Consider including <stdbool.h> if you want to use the standard 'bool' type and 'true'/'false'.

#define MAX_CHILDREN 64 // langkah legal maksimal per posisi

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


// Checks if the GameTree is empty
boolean isEmptyTree(GameTree X);

GameTree* createGameTree(GameState* rootState, int maxKedalaman, boolean isMaximizingPlayer);

// Creates and initializes a single node for the game tree.
address createNode(GameState* state, Move langkah, address parent, int kedalaman);

void minimax(address node, int depth, boolean isMaximizingPlayer);

Move getBestMove(GameTree* tree); // Corrected and uncommented

#endif // AI_ENGINE_H
