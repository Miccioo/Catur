#ifndef EVALUATE_SYSTEM_H
#define EVALUATE_SYSTEM_H

#include "../game/Gamestate.h" 
#include "../core/boolean.h"
#include <stdlib.h>
#include "../core/validator.h" 

typedef enum {
	OPENING,
	MIDGAME,
	ENDGAME
} GamePhase;

typedef struct {
	int doubled;
	int isolated;
	int passed;
} PionAnalyze;

extern const int pieceValue[7];

// Fungsi utama evaluasi
int evaluateState(GameState* state);

// Faktor-faktor evaluasi
int evaluateMaterial(GameState* state);
int evaluatePositional(GameState* state, GamePhase phase); // Tambahkan GamePhase sebagai parameter
int evaluateKingSafety(GameState* state);
int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor);
boolean isSquareAttacked(GameState* state, int x, int y, WarnaBidak attackerColor);

int evaluateMobility(GameState* state);
int evaluatePawnStructure(GameState* state);
int evaluatePawnShield(GameState* state, Position kingPos, WarnaBidak warna);
PionAnalyze analyzePion(GameState* state, WarnaBidak warna);

// Fungsi evaluasi baru
int evaluatePieceActivity(GameState* state);
int evaluateCenterControl(GameState* state);

GamePhase determineGamePhase(GameState* state);
Position findKingPosition(GameState* state, WarnaBidak warna); // Tetap di sini karena implementasinya di evaluate_system.c

#endif // EVALUATE_SYSTEM_H
