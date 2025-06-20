// ai_engine.c
#include "ai_engine.h"
#include "..\core\validator.h" // Include validator for generateAllValidMoves
#include <limits.h> // For INT_MIN, INT_MAX

//int evaluateState(GameState* state) {
//    int score = 0;
//    int x, y;
//    for (y = 0; y < 8; y++) {
//        for (x = 0; x < 8; x++) {
//            Bidak b = state->papan.grid[y][x];
//            if (b.id != -1) {
//                // Basic scoring:
//                switch(b.tipe) {
//                    case PION: score += (b.warna == PUTIH) ? 100 : -100; break;
//                    case KUDA: case GAJAH: score += (b.warna == PUTIH) ? 300 : -300; break;
//                    case BENTENG: score += (b.warna == PUTIH) ? 500 : -500; break;
//                    case MENTERI: score += (b.warna == PUTIH) ? 900 : -900; break;
//                    case RAJA: score += (b.warna == PUTIH) ? 10000 : -10000; break; // High value for King
//                    default: break;
//                }
//                // Add bonus for controlling center, piece mobility, etc. (more advanced)
//            }
//        }
//    }
//    // Adjust score for check/checkmate
//    if (state->skakmat) {
//        if (state->giliran->warna == PUTIH) { // If it's white's turn and checkmate, black wins
//            score += INT_MIN / 2; // Very bad for white
//        } else { // If it's black's turn and checkmate, white wins
//            score += INT_MAX / 2; // Very good for white
//        }
//    } else if (state->remis) {
//        score = 0; // Draw is neutral
//    }
//    return score;
//}

address createNode(GameState* state, Move langkah, address parent, int kedalaman) {
    address newNode = (address) malloc(sizeof(ElmtTree));
    // It's crucial that state is deep-copied if it contains dynamically allocated members (like Papan grid if it were pointer-based)
    // For Papan as a direct struct member, memcpy is a deep copy of the board itself.
    newNode->state = *state; // Copy struct GameState (Papan grid is copied here)
    newNode->langkah = langkah;
    newNode->parent = parent;
    newNode->kedalaman = kedalaman;
    newNode->jumlahAnak = 0;
    newNode->value = 0; // Value will be set by minimax
    int i;
    for (i = 0; i < MAX_CHILDREN; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void expandNode(address node, GameTree* tree) {
    if (node->kedalaman >= tree->maxKedalaman) return;

    // Generate legal moves for the current node's state
    // IMPORTANT: generateAllValidMoves needs the current GameState pointer for `isLegalMove`
    Move* langkahList = generateAllValidMoves(node->state.papan, node->state.giliran, &(node->state), node->state.enPassantTargetPawn); 
    
    if (langkahList == NULL) return; // Handle allocation failure

    int i = 0;
    while (langkahList[i].bidak != TIDAK_ADA && node->jumlahAnak < MAX_CHILDREN) {
        GameState newState;
        copyGameState(&newState, &(node->state)); // Deep copy the parent state for the child

        applyMove(&newState, &langkahList[i]); // Apply move to the new state

        address child = createNode(&newState, langkahList[i], node, node->kedalaman + 1);
        node->children[node->jumlahAnak++] = child;
        tree->jumlahNodeSekarang++;

        expandNode(child, tree); // Recursively build the tree
        i++;
    }
    free(langkahList); // Free the dynamically allocated move list
}

GameTree* createGameTree(GameState* rootState, int kedalamanMaks, boolean isMaximizing) {
    GameTree* tree = (GameTree*) malloc(sizeof(GameTree));
    if (tree == NULL) return NULL;
    tree->root = createNode(rootState, (Move){.bidak = TIDAK_ADA}, NULL, 0); // Initialize move with TIDAK_ADA
    if (tree->root == NULL) { free(tree); return NULL; }
    tree->maxKedalaman = kedalamanMaks;
    tree->jumlahNodeSekarang = 1;
    tree->isMaximizingPlayer = isMaximizing;

    expandNode(tree->root, tree);
    return tree;
}

// Function to free the entire game tree recursively
void freeGameTreeNodes(address node) {
    if (node == NULL) return;
    int i;
    for (i = 0; i < node->jumlahAnak; i++) {
        freeGameTreeNodes(node->children[i]);
    }
    free(node);
}

// Minimax with alpha beta pruning algorithm implementation
int minimax(address node, int depth, boolean isMaximizingPlayer, int alpha, int beta) {
    if (depth == 0 || node->jumlahAnak == 0 || node->state.skakmat || node->state.remis) {
        return evaluateState(&node->state);
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        int i;
        for (i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth - 1, false, alpha, beta);
            if (eval > maxEval) {
                maxEval = eval;
            }
            if (maxEval > alpha) {
                alpha = maxEval;
            }
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        node->value = maxEval;
        return maxEval;
    } else { // Minimizing player
        int minEval = INT_MAX;
        int i;
        for (i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth - 1, true, alpha, beta);
            if (eval < minEval) {
                minEval = eval;
            }
            if (minEval < beta) {
                beta = minEval;
            }
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        node->value = minEval;
        return minEval;
    }
}

Move getBestMove(GameTree* tree) {
    if (tree == NULL || tree->root == NULL || tree->root->jumlahAnak == 0) {
        return (Move){.bidak = TIDAK_ADA}; // Return empty move
    }

    int bestValue = tree->isMaximizingPlayer ? INT_MIN : INT_MAX;
    Move bestMove = {.bidak = TIDAK_ADA}; // Initialize with TIDAK_ADA

    int i;
    for (i = 0; i < tree->root->jumlahAnak; i++) {
        address child = tree->root->children[i];
        if (child == NULL) continue; // Should not happen if jumlahAnak is correct

        if ((tree->isMaximizingPlayer && child->value > bestValue) ||
            (!tree->isMaximizingPlayer && child->value < bestValue)) {
            bestValue = child->value;
            bestMove = child->langkah;
        }
    }

    // Free the entire tree after finding the best move
    freeGameTreeNodes(tree->root);
    free(tree);

    return bestMove;
}

// Dummy implementation for isEmptyTree (if needed by other files, though not used in this AI logic)
boolean isEmptyTree(GameTree X) {
    return X.root == NULL;
}
