#ifndef PRECOMPUTE_TABLE_H
#define PRECOMPUTE_TABLE_H

#include "chess_move.h"
#include "../game/papan.h"
#include "../game/player.h"

const int bishopMoves[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
const int knightMoves[8][2] = {{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
const int rookMoves[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
const int queenMoves[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
const int kingMoves[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};

void generatePawnMoves(Position pos, Move* moves, int* count, Player* player, Papan papan);
void generateBishopMoves(Position pos, Move* moves, int* count , Player* player, Papan papan);
void generateKnightMoves(Position pos, Move* moves, int* count, Player* player, Papan papan);
void generateRookMoves(Position pos, Move* moves, int* count, Player* player, Papan papan);
void generateQueenMoves(Position pos, Move* moves, int* count, Player* player, Papan papan);
void generateKingMoves(Position pos, Move* moves, int* count);

#endif
