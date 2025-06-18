#include "ai_engine.h"

void freeTree(address node, Move* killerMoves) {
    if (!node) return;
    for (int i = 0; i < node->jumlahAnak; i++) {
        freeTree(node->children[i], killerMoves);
    }
    free(node);
    free(killerMoves);
}

address createNode(GameState* state, Move langkah, address parent, int kedalaman) {
    address newNode = (address) malloc(sizeof(ElmtTree));
    newNode->state = *state; // Copy struct GameState
    newNode->langkah = langkah;
    newNode->parent = parent;
    newNode->kedalaman = kedalaman;
    newNode->jumlahAnak = 0;
//    newNode->value = evaluateState(state);
	newNode->value = 0;
    int i;
    for (i = 0; i < MAX_CHILDREN; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void expandNode(MoveList *list, address node, GameTree* tree, Move* killerMoves) {
    if (node->kedalaman >= tree->maxKedalaman) return;
    
    int numValidMoves;

    Move* langkahList = generateAllValidMoves(list, &node->state.papan, node->state.giliran);
//    orderMoves(langkahList, numValidMoves, &node->state, killerMoves);

    for (int i = 0; i < MAX_MOVES && langkahList[i].from.row != -1; i++) {
        GameState newState = node->state;
        applyMove(&newState, &langkahList[i]);

        address child = createNode(&newState, langkahList[i], node, node->kedalaman + 1);
        node->children[node->jumlahAnak++] = child;
        tree->jumlahNodeSekarang++;

        expandNode(list, child, tree, killerMoves);
    }
    free(langkahList); // Hindari memory leak
}

GameTree* createGameTree(GameState* rootState, int kedalamanMaks, boolean isMaximizing) {
    GameTree* tree = (GameTree*) malloc(sizeof(GameTree));
    tree->root = createNode(rootState, (Move){0}, NULL, 0);
    tree->maxKedalaman = kedalamanMaks;
    tree->jumlahNodeSekarang = 1;
    tree->isMaximizingPlayer = isMaximizing;
    
    Move* killerMoves = (Move*)calloc(kedalamanMaks, sizeof(Move));
    MoveList *list = createMoveList();

    expandNode(list, tree->root, tree, killerMoves);
    return tree;
}

int minimax(address node, int depth, int alpha, int beta, boolean isMaximizingPlayer, Move* killerMoves) {
    if (depth == 0 || node->jumlahAnak == 0) {
        return evaluateState(&node->state);
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (int i = 0; i < node->jumlahAnak; i++) {
        int eval = minimax(node->children[i], depth-1, alpha, beta, false, killerMoves);
        
        // Update killer moves
        if (eval > alpha && node->children[i]->langkah.captured == TIDAK_ADA) {
            killerMoves[node->kedalaman] = node->children[i]->langkah;
        }
    }
        node->value = maxEval;
        return maxEval;
    } else {
        int minEval = INT_MAX;
        int i;
        for (int i = 0; i < node->jumlahAnak; i++) {
        int eval = minimax(node->children[i], depth-1, alpha, beta, true, killerMoves);
        
        // Update killer moves
        if (eval > alpha && node->children[i]->langkah.captured == TIDAK_ADA) {
            killerMoves[node->kedalaman] = node->children[i]->langkah;
        }
    }
        node->value = minEval;
        return minEval;
    }

}

Move getBestMove(GameTree* tree) {
	
	if (tree->jumlahNodeSekarang == 0) return (Move){0};
	
    int bestValue = tree->isMaximizingPlayer ? INT_MIN : INT_MAX;
    Move bestMove = {0};

    for (int i = 0; i < tree->root->jumlahAnak; i++) {
        address child = tree->root->children[i];
        if ((tree->isMaximizingPlayer && child->value > bestValue) ||
            (!tree->isMaximizingPlayer && child->value < bestValue)) {
            bestValue = child->value;
            bestMove = child->langkah;
        }
    }

    return bestMove;
}
