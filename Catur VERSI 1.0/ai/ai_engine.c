#include "ai_engine.h"

int evaluateState(GameState* state) {
    int skor = 0;
    int x, y;
    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            Bidak b = state->papan.grid[y][x];
            if (b.id != -1) {
                skor += (b.warna == PUTIH) ? 1 : -1;
            }
        }
    }
    return skor;
}

address createNode(GameState* state, Move langkah, address parent, int kedalaman) {
    address newNode = (address) malloc(sizeof(ElmtTree));
    newNode->state = *state; // Copy struct GameState
    newNode->langkah = langkah;
    newNode->parent = parent;
    newNode->kedalaman = kedalaman;
    newNode->jumlahAnak = 0;
    newNode->value = evaluateState(state);
    int i;
    for (i = 0; i < MAX_CHILDREN; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void expandNode(address node, GameTree* tree) {
    if (node->kedalaman >= tree->maxKedalaman) return;

    Move* langkahList = generateAllValidMoves(&node->state); // Kamu harus buat fungsi ini
    int i = 0;

    while (langkahList[i].bidak != NULL && node->jumlahAnak < MAX_CHILDREN) {
        GameState newState = node->state;
        applyMove(&newState, &langkahList[i]); // Kamu juga harus buat fungsi ini

        address child = createNode(&newState, langkahList[i], node, node->kedalaman + 1);
        node->children[node->jumlahAnak++] = child;
        tree->jumlahNodeSekarang++;

        expandNode(child, tree); // Rekursif bangun tree
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

int minimax(address node, int depth, boolean isMaximizingPlayer) {
    if (depth == 0 || node->jumlahAnak == 0) {
        return evaluateState(&node->state);
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        int i;
        for (i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth - 1, false);
            if (eval > maxEval) maxEval = eval;
        }
        node->value = maxEval;
        return maxEval;
    } else {
        int minEval = INT_MAX;
        int i;
        for (i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth - 1, true);
            if (eval < minEval) minEval = eval;
        }
        node->value = minEval;
        return minEval;
    }
}

Move getBestMove(GameTree* tree) {
    int bestValue = tree->isMaximizingPlayer ? INT_MIN : INT_MAX;
    Move bestMove = {0};
	
	int i;
    for (i = 0; i < tree->root->jumlahAnak; i++) {
        address child = tree->root->children[i];
        if ((tree->isMaximizingPlayer && child->value > bestValue) ||
            (!tree->isMaximizingPlayer && child->value < bestValue)) {
            bestValue = child->value;
            bestMove = child->langkah;
        }
    }

    return bestMove;
}


