#include "move_ordering.h"

int scoreMove(const Move* move, const GameState* state, const Move* killerMoves) {
	
    // 1. Prioritas: Langkah dari transposition table
    if (move->isFromTT) return 10000;

    // 2. Prioritas: Capture (MVV-LVA: Most Valuable Victim - Least Valuable Attacker)
    if (move->captured != TIDAK_ADA) {
        int victimValue = pieceValue[move->captured];
        int attackerValue = pieceValue[move->bidak];
        return 9000 + (victimValue * 10) - attackerValue;
    }

    // 3. Killer moves
//    if (moveEquals(move, &killerMoves)) 
//        return 8000;

    // 4. Quiet moves
    return 0;
}

void orderMoves(Move* moves, int count, GameState* state, Move* killerMoves) {
    for (int i = 0; i < count; i++) {
        moves[i].score = scoreMove(&moves[i], state, killerMoves);
    }
    qsort(moves, count, sizeof(Move), compareMoves);
}

int compareMoves(const void* a, const void* b) {
    return ((Move*)b)->score - ((Move*)a)->score; // Descending
}
