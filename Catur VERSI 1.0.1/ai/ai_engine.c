#include "ai_engine.h"
#include <limits.h> // Untuk INT_MIN dan INT_MAX

void freeTree(address node) {
    if (!node) return;
    for (int i = 0; i < node->jumlahAnak; i++) {
        freeTree(node->children[i]);
    }
    free(node);
}

// Fungsi isEmptyTree tidak diimplementasikan di .c file yang diberikan.
// boolean isEmptyTree(GameTree X) {
//     return X.root == NULL;
// }

address createNode(GameState* state, Move langkah, address parent, int kedalaman) {
    address newNode = (address) malloc(sizeof(ElmtTree));
    if (newNode == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    newNode->state = *state; // Copy struct GameState
    newNode->langkah = langkah;
    newNode->parent = parent;
    newNode->kedalaman = kedalaman;
    newNode->jumlahAnak = 0;
	newNode->value = evaluateState(state);
    for (int i = 0; i < MAX_CHILDREN; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void expandNode(MoveList *list, address node, GameTree* tree, Move* killerMoves) {
    if (node->kedalaman >= tree->maxKedalaman) return;
    
    // Panggil generateAllValidMoves untuk mengisi list
    generateAllValidMoves(list, &node->state.papan, node->state.giliran);

    // Iterasi melalui moves di MoveList (linked list)
    MoveNode* currentMoveNode = list->head;
    while (currentMoveNode != NULL) {
        // Buat salinan GameState untuk anak agar tidak mempengaruhi state node saat ini
	    GameState newState = node->state; 
        
        // Simulasikan langkah pada newState
        applyMove(&newState, &currentMoveNode->move);
	
        // Buat node anak
	    address child = createNode(&newState, currentMoveNode->move, node, node->kedalaman + 1);
	    if (child == NULL) {
            currentMoveNode = currentMoveNode->next; // Lanjut ke node berikutnya meskipun pembuatan gagal
            continue;
        }
	
        // Tambahkan anak ke node saat ini jika masih ada ruang
	    if (node->jumlahAnak < MAX_CHILDREN) {
	        node->children[node->jumlahAnak++] = child;
	        tree->jumlahNodeSekarang++;
	
	        // Rekursif panggil expandNode untuk anak
	        expandNode(list, child, tree, killerMoves);
	    } else {
            // Jika sudah mencapai MAX_CHILDREN, free child yang baru saja dialokasikan
            // agar tidak terjadi kebocoran memori (ini penting jika MAX_CHILDREN tercapai)
            freeTree(child); // Free the child node if it cannot be added
        }
        currentMoveNode = currentMoveNode->next; // Lanjut ke langkah berikutnya
	}
    // Tidak perlu membebaskan `langkahList` karena tidak lagi dialokasikan di sini.
    // `list` akan dibersihkan di chessController setelah tree digunakan.
}

GameTree* createGameTree(GameState* rootState, int kedalamanMaks, boolean isMaximizing) {
    GameTree* tree = (GameTree*) malloc(sizeof(GameTree));
    if (tree == NULL) {
        return NULL; // Handle allocation failure
    }
    tree->root = createNode(rootState, (Move){0}, NULL, 0); // Langkah awal (dummy)
    if (tree->root == NULL) {
        free(tree);
        return NULL; // Handle allocation failure
    }
    tree->maxKedalaman = kedalamanMaks;
    tree->jumlahNodeSekarang = 1;
    tree->isMaximizingPlayer = isMaximizing;
    
    return tree;
}

int minimax(address node, int depth, int alpha, int beta, boolean isMaximizingPlayer, Move* killerMoves) {
    if (depth == 0 || node->jumlahAnak == 0) { // Base case: leaf node or max depth reached
        return evaluateState(&node->state);
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (int i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth-1, alpha, beta, false, killerMoves);
            
            // Update killer moves (logika ini bisa dioptimalkan, tapi dipertahankan sesuai aslinya)
            if (eval > alpha && node->children[i]->langkah.captured == TIDAK_ADA) {
                // Killer move should be updated if it causes a beta cutoff,
                // or if it's just a good quiet move. The current logic seems
                // to update it if the eval improves alpha.
                killerMoves[node->kedalaman] = node->children[i]->langkah;
            }
            
            if (eval > maxEval) { // Update maxEval
                maxEval = eval;
            }
            if (eval > alpha) { // Update alpha
                alpha = eval;
            }
            if (beta <= alpha) { // Alpha-beta pruning
                break;
            }
        }
        node->value = maxEval; // Simpan nilai terbaik di node
        return maxEval;
    } else { // Minimizing player
        int minEval = INT_MAX;
        for (int i = 0; i < node->jumlahAnak; i++) {
            int eval = minimax(node->children[i], depth-1, alpha, beta, true, killerMoves);
            
            // Update killer moves (logika ini bisa dioptimalkan)
            if (eval < beta && node->children[i]->langkah.captured == TIDAK_ADA) { // Similar logic for minimizing player
                killerMoves[node->kedalaman] = node->children[i]->langkah;
            }

            if (eval < minEval) { // Update minEval
                minEval = eval;
            }
            if (eval < beta) { // Update beta
                beta = eval;
            }
            if (beta <= alpha) { // Alpha-beta pruning
                break;
            }
        }
        node->value = minEval; // Simpan nilai terbaik di node
        return minEval;
    }
}

Move getBestMove(GameTree* tree) {
	if (tree == NULL || tree->root == NULL || tree->root->jumlahAnak == 0) {
        // Handle case where tree is empty or root has no children
        return (Move){0}; // Return a default/empty move
    }
	
    int bestValue = tree->isMaximizingPlayer ? INT_MIN : INT_MAX;
    Move bestMove = {0}; // Initialize with a default move

    for (int i = 0; i < tree->root->jumlahAnak; i++) {
        address child = tree->root->children[i];
        if (child == NULL) continue; // Safety check
        
        if ((tree->isMaximizingPlayer && child->value > bestValue) ||
            (!tree->isMaximizingPlayer && child->value < bestValue)) {
            bestValue = child->value;
            bestMove = child->langkah;
        }
    }

    return bestMove;
}


//#include "ai_engine.h"
//
//void freeTree(address node) {
//    if (!node) return;
//    for (int i = 0; i < node->jumlahAnak; i++) {
//        freeTree(node->children[i]);
//    }
//    free(node);
//}
//
//address createNode(GameState* state, Move langkah, address parent, int kedalaman) {
//    address newNode = (address) malloc(sizeof(ElmtTree));
//    newNode->state = *state; // Copy struct GameState
//    newNode->langkah = langkah;
//    newNode->parent = parent;
//    newNode->kedalaman = kedalaman;
//    newNode->jumlahAnak = 0;
//	newNode->value = evaluateState(state);
//    int i;
//    for (i = 0; i < MAX_CHILDREN; i++) {
//        newNode->children[i] = NULL;
//    }
//    return newNode;
//}
//
//void expandNode(MoveList *list, address node, GameTree* tree, Move* killerMoves) {
//    if (node->kedalaman >= tree->maxKedalaman) return;
//    
//    int numValidMoves;
//
//    Move* langkahList = generateAllValidMoves(list, &node->state.papan, node->state.giliran);
////    orderMoves(langkahList, numValidMoves, &node->state, killerMoves);
//
//    for (int i = 0; i < MAX_MOVES && langkahList[i].from.row != -1; i++) {
//	    GameState newState = node->state;
//	    applyMove(&newState, &langkahList[i]);
//	
//	    address child = createNode(&newState, langkahList[i], node, node->kedalaman + 1);
//	    if (child == NULL) continue;
//	
//	    if (node->jumlahAnak < MAX_CHILDREN) {
//	        node->children[node->jumlahAnak++] = child;
//	        tree->jumlahNodeSekarang++;
//	
//	        expandNode(list, child, tree, killerMoves);
//	    }
//	}
//	free(langkahList);
//
//}
//
//GameTree* createGameTree(GameState* rootState, int kedalamanMaks, boolean isMaximizing) {
//    GameTree* tree = (GameTree*) malloc(sizeof(GameTree));
//    tree->root = createNode(rootState, (Move){0}, NULL, 0);
//    tree->maxKedalaman = kedalamanMaks;
//    tree->jumlahNodeSekarang = 1;
//    tree->isMaximizingPlayer = isMaximizing;
//    
//    return tree;
//}
//
//int minimax(address node, int depth, int alpha, int beta, boolean isMaximizingPlayer, Move* killerMoves) {
//    if (depth == 0 || node->jumlahAnak == 0) {
//        return evaluateState(&node->state);
//    }
//
//    if (isMaximizingPlayer) {
//        int maxEval = INT_MIN;
//        for (int i = 0; i < node->jumlahAnak; i++) {
//        int eval = minimax(node->children[i], depth-1, alpha, beta, false, killerMoves);
//        
//        // Update killer moves
//        if (eval > alpha && node->children[i]->langkah.captured == TIDAK_ADA) {
//            killerMoves[node->kedalaman] = node->children[i]->langkah;
//        }
//    }
//        node->value = maxEval;
//        return maxEval;
//    } else {
//        int minEval = INT_MAX;
//        int i;
//        for (int i = 0; i < node->jumlahAnak; i++) {
//        int eval = minimax(node->children[i], depth-1, alpha, beta, true, killerMoves);
//        
//        // Update killer moves
//        if (eval > alpha && node->children[i]->langkah.captured == TIDAK_ADA) {
//            killerMoves[node->kedalaman] = node->children[i]->langkah;
//        }
//    }
//        node->value = minEval;
//        return minEval;
//    }
//
//}
//
//Move getBestMove(GameTree* tree) {
//	
//	if (tree->jumlahNodeSekarang == 0) return (Move){0};
//	
//    int bestValue = tree->isMaximizingPlayer ? INT_MIN : INT_MAX;
//    Move bestMove = {0};
//
//    for (int i = 0; i < tree->root->jumlahAnak; i++) {
//        address child = tree->root->children[i];
//        if ((tree->isMaximizingPlayer && child->value > bestValue) ||
//            (!tree->isMaximizingPlayer && child->value < bestValue)) {
//            bestValue = child->value;
//            bestMove = child->langkah;
//        }
//    }
//
//    return bestMove;
//}
