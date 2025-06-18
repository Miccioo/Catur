#ifndef move_ordering_h
#define move_ordering_h

#include "evaluate_system.h"

typedef enum {
    TT_MOVE,      // Langkah dari transposition table
    CAPTURE,      // Langkah menangkap
    KILLER_MOVE,  // Killer heuristic
    QUIET         // Langkah biasa
} MoveCategory;

extern const int kingMoves[10][2];
extern const int queenMoves[8][2];
extern const int rookMoves[4][2];
extern const int knightMoves[8][2];
extern const int bishopMoves[4][2];

int scoreMove(const Move* move, const GameState* state, const Move* killerMoves);
void orderMoves(Move* moves, int count, GameState* state, Move* killerMoves);
int compareMoves(const void* a, const void* b);

#endif
