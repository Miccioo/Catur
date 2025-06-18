#ifndef PRECOMPUTE_TABLE_H
#define PRECOMPUTE_TABLE_H

#include "chess_move.h"
#include "../game/papan.h"
#include "../game/player.h"

void generatePawnMoves(Position pos, Move* moves, int* count, Player* player, Papan* papan);
void generateBishopMoves(Position pos, Move* moves, int* count , Player* player, Papan* papan);
void generateKnightMoves(Position pos, Move* moves, int* count, Player* player, Papan* papan);
void generateRookMoves(Position pos, Move* moves, int* count, Player* player, Papan* papan);
void generateQueenMoves(Position pos, Move* moves, int* count, Player* player, Papan* papan);
void generateKingMoves(Position pos, Move* moves, int* count, Player* player, Papan* papan);

#endif
