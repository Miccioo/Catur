#include "evaluate_system.h"

int evaluateState(GameState* state) {
	
	if ()
	// Evaluasi dasar material
	int materialScore = evaluateMaterial(state);
	
	// Evaluasi posisi bidak
	int positionalScore = evaluatePositional(state);
	
	// Evaluasi keselamatan raja
    int kingSafetyScore = evaluateKingSafety(state);
    
    // Evaluasi mobilitas dan ruang
    int mobilityScore = evaluateMobility(state);
    
    // Evaluasi struktur pion
    int pawnStructureScore = evaluatePawnStructure(state);
    
    // Evaluasi fase permainan
    int gamePhase = determineGamePhase(state);
    int phaseWeight = (gamePhase == OPENING) ? 2 : (gamePhase == MIDGAME) ? 1 : 0;
    
    // Total score dengan bobot berbeda berdasarkan fase permainan
    int totalScore = materialScore 
                   + positionalScore
                   + (kingSafetyScore * phaseWeight)
                   + (mobilityScore * (gamePhase != ENDGAME ? 1 : 0))
                   + pawnStructureScore;
                   
    // Adjust untuk giliran
    return state->giliran->warna == PUTIH ? totalScore : -totalScore;
	
    int skor = 0;
    int x, y;
    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            Bidak b = state->papan.grid[y][x];
            
            switch (b.tipe) {
            	case BENTENG:
            		skor += (b.warna == PUTIH) ? 5 : -5;
            		break;
            		
            	case KUDA:
            		skor += (b.warna == PUTIH) ? 3 : -3;
            		break;
            		
            	case GAJAH:
            		skor += (b.warna == PUTIH) ? 3 : -3;
            		break;
            		
            	case MENTERI:
            		skor += (b.warna == PUTIH) ? 7 : -7;
            		break;
            		
            	case PION:
            		skor += (b.warna == PUTIH) ? 1 : -1;
            		break;
            		
            	case RAJA:
            		skor += (b.warna == PUTIH) ? 100 : -100;
            		break;
			}
        }
    }
    
    return skor;
}

int evaluateMaterial(GameState* state) {
    const int pieceValues[5] = {10, 30, 30, 50, 90}; // Pion, Kuda, Gajah, Benteng, Menteri
    
    int whiteMaterial = 0;
    int blackMaterial = 0;
    
    // Hitung material untuk setiap kotak
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                int value = pieceValues[piece.tipe];
                if (piece.warna == PUTIH) {
                    whiteMaterial += value;
                } else {
                    blackMaterial += value;
                }
            }
        }
    }

    return whiteMaterial - blackMaterial;
}

/**
 * Evaluasi posisi bidak berdasarkan tabel nilai posisi, 
   source tabel bidak : https://github.com/maksimKorzh/wukong/blob/master/src/wukong.c
 */
 
int evaluatePositional(GameState* state) {
    // Tabel nilai posisi untuk setiap bidak
    const int pawnTable[8][8] = {
        { 90,  90,  90,  90,  90,  90,  90,  90 },
        { 30,  30,  30,  40,  40,  30,  30,  30 },
        { 20,  20,  20,  30,  30,  20,  20,  20 },
        { 10,  10,  10,  20,  20,  10,  10,  10 },
        {  5,   5,  10,  20,  20,   5,   5,   5 },
        {  0,   0,   0,   5,   5,   0,   0,   0 },
        {  0,   0,   0, -10, -10,   0,   0,   0 },
        {  0,   0,   0,   0,   0,   0,   0,   0 }
    };
    
	const int knightTable[8][8] = {
		{ -5,   0,   0,   0,   0,   0,   0,  -5 },
        { -5,   0,   0,  10,  10,   0,   0,  -5 },
        { -5,   5,  20,  20,  20,  20,   5,  -5 },
        { -5,  10,  20,  30,  30,  20,  10,  -5 },
        { -5,  10,  20,  30,  30,  20,  10,  -5 },
        { -5,   5,  20,  10,  10,  20,   5,  -5 },
        { -5,   0,   0,   0,   0,   0,   0,  -5 },
        { -5, -10,   0,   0,   0,   0, -10,  -5 }
	}
	
	const int BioshopTable[8][8] = {
		{  0,   0,   0,   0,   0,   0,   0,   0 },
        {  0,   0,   0,   0,   0,   0,   0,   0 },
        {  0,   0,   0,  10,  10,   0,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,  10,   0,   0,   0,   0,  10,   0 },
        {  0,  30,   0,   0,   0,   0,  30,   0 },
        {  0,   0, -10,   0,   0, -10,   0,   0 }
	}
	
    const int RookTable[8][8] = {
		{ 50,  50,  50,  50,  50,  50,  50,  50 },
        { 50,  50,  50,  50,  50,  50,  50,  50 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,   0,  20,  20,   0,   0,   0 }
	};
    
	const int QueenTable[8][8] = {
		{ 50,  50,  50,  50,  50,  50,  50,  50 },
        { 50,  50,  50,  50,  50,  50,  50,  50 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,   0,  20,  20,   0,   0,   0 }
	}
    
	const int KingTable[8][8] = {
		{ 50,  50,  50,  50,  50,  50,  50,  50 },
        { 50,  50,  50,  50,  50,  50,  50,  50 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,   0,  20,  20,   0,   0,   0 }
	}
    
    int positionalScore = 0;
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                int sign = (piece.warna == PUTIH) ? 1 : -1;
                
                switch (piece.tipe) {
                    case PION:
                        positionalScore += sign * pawnTable[piece.warna == PUTIH ? y : 7-y][x];
                        break;
                        
                    case BENTENG:
                    	positionalScore += sign * rookTable[piece.warna == PUTIH ? y : 7-y][x];
                    	break;
  
                    case KUDA:
                    	positionalScore += sign * knightTable[piece.warna == PUTIH ? y : 7-y][x];
                    	break;

                    case GAJAH:
                    	positionalScore += sign * bioshopTable[piece.warna == PUTIH ? y : 7-y][x];
                    	break;

                    case MENTERI:
                    	positionalScore += sign * queenTable[piece.warna == PUTIH ? y : 7-y][x];
                    	break;

                    case RAJA:
                    	positionalScore += sign * kingTable[piece.warna == PUTIH ? y : 7-y][x];
                    	break;
                }
            }
        }
    }
    
    return positionalScore;
}

/**
 * Evaluasi keselamatan raja berdasarkan proteksi dan ancaman
 */
int evaluateKingSafety(GameState* state) {
    int whiteKingSafety = 0;
    int blackKingSafety = 0;

    // Temukan posisi raja
    Position whiteKingPos = findKingPosition(state, PUTIH);
    Position blackKingPos = findKingPosition(state, HITAM);
    
    // Hitung pawn shield
    whiteKingSafety += evaluatePawnShield(state, whiteKingPos, PUTIH);
    blackKingSafety += evaluatePawnShield(state, blackKingPos, HITAM);
    
    // Hitung serangan ke kotak di sekitar raja
    whiteKingSafety -= evaluateKingAttack(state, whiteKingPos, HITAM);
    blackKingSafety -= evaluateKingAttack(state, blackKingPos, PUTIH);
    
    return whiteKingSafety - blackKingSafety;
}

/**
 * Evaluasi mobilitas bidak dan kontrol ruang
 */
int evaluateMobility(GameState* state) {
    int whiteMobility = 0;
    int blackMobility = 0;
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                // Hitung jumlah langkah legal untuk bidak ini
                int moves = countLegalMoves(state, x, y);
                
                if (piece.warna == PUTIH) {
                    whiteMobility += moves;
                } else {
                    blackMobility += moves;
                }
            }
        }
    }
    
    return whiteMobility - blackMobility;
}

/**
 * Evaluasi struktur pion (doubled, isolated, passed pawns)
 */
int evaluatePawnStructure(GameState* state) {
    int whitePawnStructure = 0;
    int blackPawnStructure = 0;
    
    // Hitung doubled pawns
    whitePawnStructure -= 10 * countDoubledPawns(state, PUTIH);
    blackPawnStructure -= 10 * countDoubledPawns(state, HITAM);
    
    // Hitung isolated pawns
    whitePawnStructure -= 15 * countIsolatedPawns(state, PUTIH);
    blackPawnStructure -= 15 * countIsolatedPawns(state, HITAM);
    
    // Hitung passed pawns
    whitePawnStructure += 20 * countPassedPawns(state, PUTIH);
    blackPawnStructure += 20 * countPassedPawns(state, HITAM);
    
    return whitePawnStructure - blackPawnStructure;
}

/**
 * Menentukan fase permainan berdasarkan material
 */
GamePhase determineGamePhase(GameState* state) {
    int queenCount = 0;
    int minorPieceCount = 0;
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == QUEEN) queenCount++;
            if (piece.tipe == KNIGHT || piece.tipe == BISHOP) minorPieceCount++;
        }
    }
    
    if (queenCount == 2 && minorPieceCount >= 4) return OPENING;
    if (queenCount >= 1) return MIDGAME;
    return ENDGAME;
}
