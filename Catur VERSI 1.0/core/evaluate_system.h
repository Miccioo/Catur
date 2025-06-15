#ifndef EVALUATE_SYSTEM_H
#define EVALUATE_SYSTEM_H

#include "../game/Gamestate.h" 
#include "../core/chess_move.h"
#include "../core/boolean.h"
#include "stdlib.h"

/**
 * Phase Game dalam catur
 */ 
typedef enum {
	OPENING,
	MIDGAME,
	ENDGAME
} GamePhase;

/** 
 * PionAnalyze digunakan untuk menyimpan info tentang pion 
 * seperti doubled, isolated dan passed sehingga tidak usah
 * membuat fungsi untuk menghitung ketiga info ini
 */

typedef struct {
	int doubled;
	int isolated;
	int passed;
} PionAnalyze;

/** 
 * Evaluasi keadaan untuk dihitung dalam algoritma minimax
 */
int evaluateState(GameState* state);

/** 
 * Evaluasi material yang ada di papan
 */
int evaluateMaterial(GameState* state);

/**
 * Evaluasi posisi bidak berdasarkan tabel nilai posisi, 
 * source tabel bidak : https://github.com/maksimKorzh/wukong/blob/master/src/wukong.c
 */
int evaluatePositional(GameState* state);

/**
 * Evaluasi keselamatan raja berdasarkan proteksi dan ancaman
 */
int evaluateKingSafety(GameState* state);

Position findKingPosition(GameState* state, WarnaBidak warna);

int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor);

boolean isSquareAttacked(GameState* state, int x, int y, WarnaBidak attackerColor);

/**
 * Evaluasi mobilitas bidak dan kontrol ruang
 */
int evaluateMobility(GameState* state);

/**
 * Evaluasi struktur pion (doubled, isolated, passed pawns)
 */
int evaluatePawnStructure(GameState* state);

/** 
 * Evaluasi pion yang melindungi raja
 */
int evaluatePawnShield(GameState* state, Position kingPos, WarnaBidak warna);

/** 
 * Evaluasi pion yang melindungi raja
 */
PionAnalyze analyzePion(GameState* state, WarnaBidak warna);

/**
 * Menentukan fase permainan berdasarkan material
 */
GamePhase determineGamePhase(GameState* state);


#endif

