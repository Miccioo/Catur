#include "ai_engine.h"

void freeTree(address node) {
    if (!node) return;
    for (int i = 0; i < node->jumlahAnak; i++) {
        freeTree(node->children[i]);
    }
    free(node);
}

address createNode(GameState* state, Move langkah, address parent, int kedalaman) {
    address newNode = (address) malloc(sizeof(ElmtTree));
    newNode->state = *state; // Copy struct GameState
    newNode->langkah = langkah;
    newNode->parent = parent;
    newNode->zobristKey = computeKey(state);  // Simpan Zobrist key
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

void expandNode(address node, GameTree* tree) {
    if (node->kedalaman >= tree->maxKedalaman) return;

    Move* langkahList = generateAllValidMoves(node->state.papan, node->state.giliran);
    int i = 0;

    while (i < MAX_MOVES && langkahList[i].from.row != -1 && node->jumlahAnak < MAX_CHILDREN) {
        GameState newState = node->state;
        applyMove(&newState, &langkahList[i]);

        address child = createNode(&newState, langkahList[i], node, node->kedalaman + 1);
        node->children[node->jumlahAnak++] = child;
        tree->jumlahNodeSekarang++;

        expandNode(child, tree);
        i++;
    }
}

GameTree* createGameTree(GameState* rootState, int kedalamanMaks, boolean isMaximizing) {
    GameTree* tree = (GameTree*) malloc(sizeof(GameTree));
    tree->root = createNode(rootState, (Move){0}, NULL, 0);
    tree->maxKedalaman = kedalamanMaks;
    tree->jumlahNodeSekarang = 1;
    tree->isMaximizingPlayer = isMaximizing;

    expandNode(tree->root, tree);
    return tree;
}

int minimax(address node, int depth, int alpha, int beta, boolean isMaximizingPlayer) {
    if (depth == 0 || node->jumlahAnak == 0) {
        return evaluateState(&node->state);
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (int i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth - 1, alpha, beta, false);
            maxEval = (eval > maxEval) ? eval : maxEval;
            alpha = (alpha > eval) ? alpha : eval;
            
            if (beta <= alpha) break;
        }
        node->value = maxEval;
        return maxEval;
    } else {
        int minEval = INT_MAX;
        int i;
        for (i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth - 1, alpha, beta, true);
            minEval = (eval < minEval) ? eval : minEval;
            beta = (beta < eval) ? beta : eval;
            
            if (beta <= alpha) break;
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
