// ai_engine.h
#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "../core/evaluate_system.h"
#include "../core/validator.h"
#include <stdint.h>

#define MAX_CHILDREN 64 // langkah legal maksimal per posisi

typedef struct tElmtTree* address;
typedef struct tElmtTree {
    GameState state;
    int value;
    Move langkah;
    address parent;
    uint64_t zobristKey; // Zobrist hashing key untuk trasnsposition table
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

void freeTree(address node);

// Checks if the GameTree is empty
boolean isEmptyTree(GameTree X);

GameTree* createGameTree(GameState* rootState, int maxKedalaman, boolean isMaximizingPlayer);

// Creates and initializes a single node for the game tree.
address createNode(GameState* state, Move langkah, address parent, int kedalaman);

int minimax(address node, int depth, int alpha, int beta, boolean isMaximizingPlayer);

Move getBestMove(GameTree* tree);

#endif // AI_ENGINE_H
