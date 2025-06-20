#ifndef EVALUATE_SYSTEM_H
#define EVALUATE_SYSTEM_H

#include "../game/Gamestate.h" 
#include "../core/chess_move.h"
#include "../core/boolean.h"
#include "validator.h"
#include "stdlib.h"

#define MATERIAL_WEIGHT 100 // Material is generally the most important
#define POSITIONAL_WEIGHT 10
#define KING_SAFETY_WEIGHT_OPENING_MIDGAME 15 // King safety is crucial early/mid
#define KING_SAFETY_WEIGHT_ENDGAME 5        // Less crucial in endgame
#define MOBILITY_WEIGHT 5
#define PAWN_STRUCTURE_WEIGHT 10

extern const int pawnTableWhite[8][8];
extern const int bishopTableWhite[8][8];
extern const int rookTableWhite[8][8];
extern const int queenTableWhite[8][8];
extern const int knightTableWhite[8][8];
extern const int kingTableWhite[8][8];
extern const int kingEndgameTableWhite[8][8];

extern const int pawnTableBlack[8][8];
extern const int bishopTableBlack[8][8];
extern const int rookTableBlack[8][8];
extern const int queenTableBlack[8][8];
extern const int knightTableBlack[8][8];
extern const int kingTableBlack[8][8];
extern const int kingEndgameTableBlack[8][8];

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

int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor);

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

