#include "ai_engine.h"

boolean isEmptyTree (Tree X) {
	return X.root == NULL;
}

address createNode (/*GameState* State, Move langkah, */int kedalamanMaks) {
	address node = (address)malloc(sizeof(Tree));
	node->state = *state;
    node->langkah = langkah;
    node->kedalaman = kedalaman;
    node->jumlahAnak = 0;
    node->value = evaluateState(state);
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    return node;
}

GameTree createTree (/*GameState* rootState, */ int kedalamanMaks) {
	
}
