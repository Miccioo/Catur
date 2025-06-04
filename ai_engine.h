#ifndef AI_ENGINE_H
#define AI_ENGINE_H


//#include "game_state.h"
//#include "move.h"
#include "stdlib.h"
#include "boolean.h"

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
} ElmtTree;

typedef struct {
	address root;
	int maxKedalaman;
	int jumlahNodeSekarang;
	boolean isMaximizingPlayer;
} GameTree;

boolean isEmptyTree(GameTree X);

address createTree(/* GameState* rootState */int kedalamanMaks);
address createNode(/* GameState* rootState , Move langkah, */int kedalamanMaks);
//Move getBestMove(Tree root);

#endif
