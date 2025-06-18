#include "evaluate_system.h"


int evaluateState(GameState* state) {

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
	
	const int pieceValue[] = {
	    0,    // TIDAK_ADA or empty
	    10,  // Pawn
	    30,  // Knight
	    30,  // Bishop
	    50,  // Rook
	    90,  // Queen
	    1000 // King
	};
    
    int whiteMaterial = 0;
    int blackMaterial = 0;
    
    // Hitung material untuk setiap kotak
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                int value = pieceValue[piece.tipe];
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
	};
	
	const int bishopTable[8][8] = {
		{  0,   0,   0,   0,   0,   0,   0,   0 },
        {  0,   0,   0,   0,   0,   0,   0,   0 },
        {  0,   0,   0,  10,  10,   0,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,  10,   0,   0,   0,   0,  10,   0 },
        {  0,  30,   0,   0,   0,   0,  30,   0 },
        {  0,   0, -10,   0,   0, -10,   0,   0 }
	};

    const int rookTable[8][8] = {
		{ 50,  50,  50,  50,  50,  50,  50,  50 },
        { 50,  50,  50,  50,  50,  50,  50,  50 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,   0,  20,  20,   0,   0,   0 }
	};
    
	const int queenTable[8][8] = {
		{ 50,  50,  50,  50,  50,  50,  50,  50 },
        { 50,  50,  50,  50,  50,  50,  50,  50 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,  10,  20,  20,  10,   0,   0 },
        {  0,   0,   0,  20,  20,   0,   0,   0 }
	};
    
	const int kingTable[8][8] = {
		{  0,   0,   0,   0,   0,   0,   0,   0 },
        {  0,   0,   5,   5,   5,   5,   0,   0 },
        {  0,   5,   5,  10,  10,   5,   5,   0 },
        {  0,   5,  10,  20,  20,  10,   5,   0 },
        {  0,   5,  10,  20,  20,  10,   5,   0 },
        {  0,   0,   5,  10,  10,   5,   0,   0 },
        {  0,  -5,  -5,  -5,  -5,  -5,  -5,   0 },
        {  0,   0,   5,   0, -15,   0,  10,   0 }
	};
    
    int positionalScore = 0;
    
    for (int y = 8; y > 0; y--) {
        for (int x = 0; x > 0; x++) {
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
                    	positionalScore += sign * bishopTable[piece.warna == PUTIH ? y : 7-y][x];
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

Position findKingPosition(GameState* state, WarnaBidak warna) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == RAJA && piece.warna == warna) return (Position){x, y};
        }
    }
    
    return (Position){-1, -1}; // Tidak ditemukan 
}

int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor) {
    int attackScore = 0;
    
    // Cek semua bidak lawan yang mengancam kotak sekitar raja
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; // Lewati posisi raja sendiri
            
            int x = kingPos.col + dx;
            int y = kingPos.row + dy;
            if (x < 0 || x >= 8 || y < 0 || y >= 8) continue;

            if (isSquareAttacked(state, x, y, attackerColor)) {
                attackScore += 10; // Penalty untuk setiap ancaman
            }
        }
    }
    return attackScore;
}

/**
 * Mengecek apakah kotak (x,y) diserang oleh bidak lawan dengan warna tertentu
 * @return true jika kotak dalam keadaan diserang, false jika tidak
 */
boolean isSquareAttacked(GameState* state, int x, int y, WarnaBidak attackerColor) {
    // Cek serangan dari pion
    int pawnDir = (attackerColor == PUTIH) ? 1 : -1;
    Position pawnAttacks[2] = {
        {x-1, y + pawnDir},  // Serangan diagonal kiri
        {x+1, y + pawnDir}   // Serangan diagonal kanan
    };
    
    for (int i = 0; i < 2; i++) {
        Position pos = pawnAttacks[i];
        if (pos.col >= 0 && pos.col < 8 && pos.row >= 0 && pos.row < 8) {
            Bidak piece = getBidakAt(state->papan, pos.col, pos.row);
            if (piece.tipe == PION && piece.warna == attackerColor) {
                return true;
            }
        }
    }

    // Cek serangan dari kuda (knight)
    Position knightMoves[8] = {
        {x-2, y-1}, {x-1, y-2}, {x+1, y-2}, {x+2, y-1},
        {x+2, y+1}, {x+1, y+2}, {x-1, y+2}, {x-2, y+1}
    };
    
    for (int i = 0; i < 8; i++) {
        Position pos = knightMoves[i];
        if (pos.col >= 0 && pos.col < 8 && pos.row >= 0 && pos.row < 8) {
            Bidak piece = getBidakAt(state->papan, pos.col, pos.row);
            if (piece.tipe == KUDA && piece.warna == attackerColor) {
                return true;
            }
        }
    }

    // Cek serangan garis lurus (benteng/ratu)
    Position straightDirs[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (int i = 0; i < 4; i++) {
        Position dir = straightDirs[i];
        for (int step = 1; step < 8; step++) {
            int cx = x + dir.col * step;
            int cy = y + dir.row * step;
            
            if (cx < 0 || cx >= 8 || cy < 0 || cy >= 8) break;
            
            Bidak piece = getBidakAt(state->papan, cx, cy);
            if (piece.tipe != TIDAK_ADA) {
                if (piece.warna == attackerColor && 
                    (piece.tipe == BENTENG || piece.tipe == MENTERI)) {
                    return true;
                }
                break;  // Terhalang bidak lain
            }
        }
    }

    // Cek serangan diagonal (gajah/ratu)
    Position diagonalDirs[4] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for (int i = 0; i < 4; i++) {
        Position dir = diagonalDirs[i];
        for (int step = 1; step < 8; step++) {
            int cx = x + dir.col * step;
            int cy = y + dir.row * step;
            
            if (cx < 0 || cx >= 8 || cy < 0 || cy >= 8) break;
            
            Bidak piece = getBidakAt(state->papan, cx, cy);
            if (piece.tipe != TIDAK_ADA) {
                if (piece.warna == attackerColor && 
                    (piece.tipe == GAJAH || piece.tipe == MENTERI)) {
                    return true;
                }
                break;  // Terhalang bidak lain
            }
        }
    }

    // Cek serangan raja (jarak 1 kotak)
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            
            int cx = x + dx;
            int cy = y + dy;
            if (cx >= 0 && cx < 8 && cy >= 0 && cy < 8) {
                Bidak piece = getBidakAt(state->papan, cx, cy);
                if (piece.tipe == RAJA && piece.warna == attackerColor) {
                    return true;
                }
            }
        }
    }

    return false;
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
                int moves = countLegalMoves(state->papan, x, y, state->giliran);

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
    
    PionAnalyze white = analyzePion(state, PUTIH);
	PionAnalyze black = analyzePion(state, HITAM);
	
	whitePawnStructure += -10 * white.doubled - 15 * white.isolated + 20 * white.passed;
	blackPawnStructure -= -10 * black.doubled - 15 * black.isolated + 20 * black.passed;
    
    return whitePawnStructure - blackPawnStructure;
}

/** 
 * Evaluasi pion yang melindungi raja
 */
int evaluatePawnShield(GameState* state, Position kingPos, WarnaBidak warna) {
	int shieldScore = 0;
    int forward = (warna == PUTIH) ? 1 : -1;
    
    // Cek pion di kolom adjasen (�1 dari kolom raja)
    for (int dx = -1; dx <= 1; dx++) {
        int x = kingPos.col + dx;
        if (x < 0 || x >= 8) continue;
        
        int y = kingPos.row + forward;
        if (y < 0 || y >= 8) continue;
        
        Bidak piece = state->papan.grid[y][x];
        if (piece.tipe == PION && piece.warna == warna) {
            shieldScore += 15; // Bonus untuk setiap pion pelindung
        }
    }
    
    return shieldScore;
}

PionAnalyze analyzePion(GameState* state, WarnaBidak warna) {
	PionAnalyze result;
	int pawnCountPerFile[8] = {0};
	
	// Hitung doubled: hitung pion di tiap file
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == PION && piece.warna == warna) {
                pawnCountPerFile[x]++;
            }
        }
    }
    
    for (int x = 0; x < 8; x++) {
        if (pawnCountPerFile[x] > 1) {
            result.doubled += pawnCountPerFile[x] - 1;
        }
    }
    
    // Hitung isolated & passed
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == PION && piece.warna == warna) {
                // Cek isolated
                boolean leftFileHasPawn = (x > 0) && pawnCountPerFile[x - 1] > 0;
                boolean rightFileHasPawn = (x < 7) && pawnCountPerFile[x + 1] > 0;
                if (!leftFileHasPawn && !rightFileHasPawn) {
                    result.isolated++;
                }

                // Cek passed
                boolean isPassed = true;
                int startRow, endRow, step;
                if (warna == PUTIH) {
                    startRow = y - 1;
                    endRow = -1;
                    step = -1;
                } else {
                    startRow = y + 1;
                    endRow = 8;
                    step = 1;
                }

                for (int row = startRow; row != endRow; row += step) {
                    for (int col = x - 1; col <= x + 1; col++) {
                        if (col >= 0 && col < 8) {
                            Bidak frontPiece = state->papan.grid[row][col];
                            if (frontPiece.tipe == PION && frontPiece.warna != warna) {
                                isPassed = false;
                                break;
                            }
                        }
                    }
                    if (!isPassed) break;
                }

                if (isPassed) {
                    result.passed++;
                }
            }
        }
    }

    return result;

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
            if (piece.tipe == MENTERI) queenCount++;
            if (piece.tipe == KUDA || piece.tipe == GAJAH) minorPieceCount++;
        }
    }
    
    if (queenCount == 2 && minorPieceCount >= 4) return OPENING;
    if (queenCount >= 1) return MIDGAME;
    return ENDGAME;
}


