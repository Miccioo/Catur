#include "move_ordering.h"

const int kingMoves[10][2]  = { {1,0}, {-1,0}, { 0,1}, { 0,-1}, { 1, 1}, { 1,-1}, {-1,1}, {-1,-1}, {-2,0}, {2,0}};
const int queenMoves[8][2]  = { {1,0}, {-1,0}, { 0,1}, { 0,-1}, { 1, 1}, { 1,-1}, {-1,1}, {-1,-1} };
const int rookMoves[4][2]   = { {1,0}, {-1,0}, { 0,1}, { 0,-1} };
const int knightMoves[8][2] = { {2,1}, {1, 2}, {-1,2}, {-2, 1}, {-2,-1}, {-1,-2}, {1,-2}, { 2,-1} };
const int bishopMoves[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

int scoreMove(const Move* move, const GameState* state, const Move* killerMoves) {
	// Tabel nilai untuk setiap jenis bidak
	const int pieceValue[7] = {
	    0,    // TIDAK_ADA or empty
	    10,  // Pawn
	    30,  // Knight
	    30,  // Bishop
	    50,  // Rook
	    90,  // Queen
	    1000 // King
	};
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
