#ifndef PRECOMPUTE_TABLE_H
#define PRECOMPUTE_TABLE_H

#include "chess_move.h"
#include "../game/papan.h"
#include "../game/player.h"

// Membuat definisi berikut menjadi tidak collasion saat dipakai di file .c yang lainnya
extern const int kingMoves[10][2];
extern const int queenMoves[8][2];
extern const int rookMoves[4][2];
extern const int knightMoves[8][2];
extern const int bishopMoves[4][2];

void generatePawnMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan);
void generateBishopMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan);
void generateKnightMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan);
void generateRookMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan);
void generateQueenMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan);
void generateKingMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan);

#endif
