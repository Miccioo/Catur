#include "evaluate_system.h"
#include <math.h> // Untuk fungsi abs() jika belum ada

// Nilai bidak standar (dalam centipawn, 100 centipawn = 1 pion)
const int pieceValue[7] = {
    0,    // TIDAK_ADA
    100,  // PION
    320,  // KUDA (sedikit lebih dari gajah karena fleksibilitas)
    330,  // GAJAH (seringkali lebih kuat dari kuda di open position)
    500,  // BENTENG
    900,  // MENTERI
    20000 // RAJA (sangat tinggi karena kalah = game over)
};

// Bobot untuk berbagai faktor evaluasi (nilai dapat disesuaikan)
typedef struct {
    int MATERIAL;
    int POSITIONAL;
    int KING_SAFETY;
    int MOBILITY;
    int PAWN_STRUCTURE;
    int PIECE_ACTIVITY; // New factor
    int CENTER_CONTROL; // New factor
} EvaluationWeights;

// Bobot yang berbeda untuk setiap fase permainan (contoh)
const EvaluationWeights OPENING_WEIGHTS = {
    .MATERIAL = 100, .POSITIONAL = 50, .KING_SAFETY = 70, 
    .MOBILITY = 30, .PAWN_STRUCTURE = 20, .PIECE_ACTIVITY = 40, .CENTER_CONTROL = 60
};
const EvaluationWeights MIDGAME_WEIGHTS = {
    .MATERIAL = 100, .POSITIONAL = 60, .KING_SAFETY = 80, 
    .MOBILITY = 40, .PAWN_STRUCTURE = 30, .PIECE_ACTIVITY = 50, .CENTER_CONTROL = 40
};
const EvaluationWeights ENDGAME_WEIGHTS = {
    .MATERIAL = 100, .POSITIONAL = 70, .KING_SAFETY = 20, // King safety less critical, sometimes central king is good
    .MOBILITY = 20, .PAWN_STRUCTURE = 50, .PIECE_ACTIVITY = 30, .CENTER_CONTROL = 10 // Pawn structure and king activity more important
};

// --- Helper Functions for Evaluation ---

// Helper untuk mendapatkan tabel nilai posisi (Piece-Square Tables)
// Nilai ini adalah contoh dan bisa disesuaikan
const int PawnPST[8][8] = {
    { 100, 100, 100, 100, 100, 100, 100, 100 },
    {  50,  50,  50,  50,  50,  50,  50,  50 },
    {  10,  10,  20,  30,  30,  20,  10,  10 },
    {   5,   5,  10,  25,  25,  10,   5,   5 },
    {   0,   0,   0,  20,  20,   0,   0,   0 },
    {   5,  -5, -10,   0,   0, -10,  -5,   5 },
    {   5,  10,  10, -20, -20,  10,  10,   5 },
    {   0,   0,   0,   0,   0,   0,   0,   0 }
};

const int KnightPST[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int BishopPST[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int RookPST[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 5, 10, 10, 10, 10, 10, 10,  5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    { 0,  0,  0,  5,  5,  0,  0,  0}
};

const int QueenPST[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};

// King PST for mid-game (staying safely behind pawns)
const int KingPST_Midgame[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20}
};

// King PST for endgame (more active in the center)
const int KingPST_Endgame[8][8] = {
    {-50,-40,-30,-20,-20,-30,-40,-50},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-50,-40,-30,-20,-20,-30,-40,-50}
};

// Fungsi utama evaluasi
int evaluateState(GameState* state) {
    int totalScore = 0;
    GamePhase phase = determineGamePhase(state);
    const EvaluationWeights *currentWeights;

    // Pilih bobot berdasarkan fase permainan
    if (phase == OPENING) {
        currentWeights = &OPENING_WEIGHTS;
    } else if (phase == MIDGAME) {
        currentWeights = &MIDGAME_WEIGHTS;
    } else { // ENDGAME
        currentWeights = &ENDGAME_WEIGHTS;
    }

    // 1. Evaluasi Material
    totalScore += evaluateMaterial(state) * currentWeights->MATERIAL;

    // 2. Evaluasi Posisi Bidak (Piece-Square Tables)
    totalScore += evaluatePositional(state, phase) * currentWeights->POSITIONAL;

    // 3. Evaluasi Keamanan Raja
    totalScore += evaluateKingSafety(state) * currentWeights->KING_SAFETY;

    // 4. Evaluasi Mobilitas (Jumlah langkah legal)
    totalScore += evaluateMobility(state) * currentWeights->MOBILITY;

    // 5. Evaluasi Struktur Pion
    totalScore += evaluatePawnStructure(state) * currentWeights->PAWN_STRUCTURE;

    // 6. Evaluasi Aktivitas Bidak (Bidak yang aktif atau di posisi strategis)
    totalScore += evaluatePieceActivity(state) * currentWeights->PIECE_ACTIVITY;

    // 7. Evaluasi Kontrol Pusat
    totalScore += evaluateCenterControl(state) * currentWeights->CENTER_CONTROL;

    // Adjust untuk giliran: Skor dihitung dari perspektif Putih.
    // Jika giliran Hitam, negasikan skor total.
    return state->giliran->warna == PUTIH ? totalScore : -totalScore;
}

int evaluateMaterial(GameState* state) {
    int whiteMaterial = 0;
    int blackMaterial = 0;
    
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

// Menggunakan Piece-Square Tables (PST) yang lebih detail
int evaluatePositional(GameState* state, GamePhase phase) {
    int positionalScore = 0;
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                int score = 0;
                int boardY = (piece.warna == PUTIH) ? y : (7 - y); // Mirror board for Black
                
                switch (piece.tipe) {
                    case PION:      score = PawnPST[boardY][x]; break;
                    case KUDA:      score = KnightPST[boardY][x]; break;
                    case GAJAH:     score = BishopPST[boardY][x]; break;
                    case BENTENG:   score = RookPST[boardY][x]; break;
                    case MENTERI:   score = QueenPST[boardY][x]; break;
                    case RAJA:      
                        score = (phase == ENDGAME) ? KingPST_Endgame[boardY][x] : KingPST_Midgame[boardY][x]; 
                        break;
                    default: break;
                }
                
                if (piece.warna == PUTIH) {
                    positionalScore += score;
                } else {
                    positionalScore -= score;
                }
            }
        }
    }
    return positionalScore;
}

int evaluateKingSafety(GameState* state) {
    int whiteKingSafety = 0;
    int blackKingSafety = 0;

    Position whiteKingPos = findKingPosition(state, PUTIH);
    Position blackKingPos = findKingPosition(state, HITAM);
    
    if (whiteKingPos.row != -1) {
        whiteKingSafety += evaluatePawnShield(state, whiteKingPos, PUTIH);
        whiteKingSafety -= evaluateKingAttack(state, whiteKingPos, HITAM);
    }
    if (blackKingPos.row != -1) {
        blackKingSafety += evaluatePawnShield(state, blackKingPos, HITAM);
        blackKingSafety -= evaluateKingAttack(state, blackKingPos, PUTIH);
    }
    
    return whiteKingSafety - blackKingSafety;
}

Position findKingPosition(GameState* state, WarnaBidak warna) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == RAJA && piece.warna == warna) return (Position){x, y};
        }
    }
    return (Position){-1, -1}; // Not found
}

int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor) {
    int attackScore = 0;
    // Semakin banyak bidak lawan mengancam sekitar raja, semakin buruk
    // Bobot serangan bisa berbeda untuk setiap tipe bidak penyerang
    
    // Cek 8 kotak di sekitar raja
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            
            int x = kingPos.col + dx;
            int y = kingPos.row + dy;
            if (x < 0 || x >= 8 || y < 0 || y >= 8) continue;

            if (isSquareAttacked(state, x, y, attackerColor)) {
                // Berikan penalti lebih tinggi untuk serangan langsung ke raja, 
                // atau serangan dari bidak-bidak kuat.
                attackScore += 20; // Penalti dasar
                // TODO: Tambahkan logika untuk bobot serangan berdasarkan tipe bidak penyerang
                // (e.g., Menteri menyerang > Pion menyerang)
            }
        }
    }
    return attackScore;
}

boolean isSquareAttacked(GameState* state, int x, int y, WarnaBidak attackerColor) {
    // Fungsi ini mengecek apakah kotak (x,y) diserang oleh bidak lawan.
    // Implementasi yang ada sudah cukup baik.
    // (Kode isSquareAttacked Anda sebelumnya di sini)
    // Cek serangan dari pion
    int pawnDir = (attackerColor == PUTIH) ? 1 : -1; // Pawn moves forward for its color
    Position pawnAttacks[2] = {
        {x-1, y + pawnDir},  // Diagonal left capture
        {x+1, y + pawnDir}   // Diagonal right capture
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

    // Cek serangan dari kuda
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
                break;  
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
                break;  
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

int evaluateMobility(GameState* state) {
    int whiteMobility = 0;
    int blackMobility = 0;
    
    // Alokasi MoveList sementara untuk menghitung langkah legal
    // Ini bisa mahal, pertimbangkan caching atau hanya menghitung "potential moves"
    // daripada "legal moves" yang melibatkan check-checking.
    // Namun, untuk akurasi, menghitung legal moves adalah yang terbaik.
    MoveList *tempMoveList = createMoveList();
    if (tempMoveList == NULL) return 0; // Handle allocation failure

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                Player tempPlayer; 
                initPlayer(&tempPlayer, "Temp", piece.warna); 

                // countLegalMoves akan memanggil generateAllValidMoves
                // yang pada gilirannya memanggil isMoveLegalAfterCheck
                int moves = countLegalMoves(state->papan, y, x, &tempPlayer); // Passed row, col for consistency
                
                if (piece.warna == PUTIH) {
                    whiteMobility += moves;
                } else {
                    blackMobility += moves;
                }
            }
        }
    }
    destroyList(tempMoveList); // Bebaskan MoveList sementara
    return whiteMobility - blackMobility;
}

int evaluatePawnStructure(GameState* state) {
    int whitePawnStructure = 0;
    int blackPawnStructure = 0;
    
    PionAnalyze white = analyzePion(state, PUTIH);
	PionAnalyze black = analyzePion(state, HITAM);
	
	// Penalti lebih tinggi untuk pion terisolasi atau ganda
	whitePawnStructure += (-25 * white.doubled) + (-30 * white.isolated) + (50 * white.passed);
	blackPawnStructure -= ((-25 * black.doubled) + (-30 * black.isolated) + (50 * black.passed));

    return whitePawnStructure - blackPawnStructure;
}

int evaluatePawnShield(GameState* state, Position kingPos, WarnaBidak warna) {
	int shieldScore = 0;
    int forward = (warna == PUTIH) ? -1 : 1; 
    
    // Check pawns in adjacent columns (±1 from king's column) on the pawn shield rank
    for (int dx = -1; dx <= 1; dx++) {
        int x = kingPos.col + dx;
        if (x < 0 || x >= 8) continue;
        
        int y = kingPos.row + forward; // Rank in front of the king
        if (y < 0 || y >= 8) continue;
        
        Bidak piece = state->papan.grid[y][x];
        if (piece.tipe == PION && piece.warna == warna) {
            shieldScore += 20; // Bonus for each protecting pawn
        }
    }
    return shieldScore;
}

PionAnalyze analyzePion(GameState* state, WarnaBidak warna) {
	PionAnalyze result = {0, 0, 0}; 
	int pawnCountPerFile[8] = {0};
	
	// Hitung doubled pawns
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
    
    // Hitung isolated & passed pawns
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == PION && piece.warna == warna) {
                // Check isolated pawn
                boolean leftFileHasPawn = false;
                if (x > 0) { 
                    for (int r = 0; r < 8; r++) {
                        if (state->papan.grid[r][x-1].tipe == PION && state->papan.grid[r][x-1].warna == warna) {
                            leftFileHasPawn = true;
                            break;
                        }
                    }
                }

                boolean rightFileHasPawn = false;
                if (x < 7) { 
                    for (int r = 0; r < 8; r++) {
                        if (state->papan.grid[r][x+1].tipe == PION && state->papan.grid[r][x+1].warna == warna) {
                            rightFileHasPawn = true;
                            break;
                        }
                    }
                }
                
                if (!leftFileHasPawn && !rightFileHasPawn) {
                    result.isolated++;
                }

                // Check passed pawn
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
                    for (int col_offset = -1; col_offset <= 1; col_offset++) { 
                        int col = x + col_offset;
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

GamePhase determineGamePhase(GameState* state) {
    int queenCount = 0;
    int minorPieceCount = 0; 
    int rookCount = 0;
    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == MENTERI) queenCount++;
            if (piece.tipe == KUDA || piece.tipe == GAJAH) minorPieceCount++;
            if (piece.tipe == BENTENG) rookCount++;
        }
    }
    
    // Perhitungan fase permainan yang lebih detail
    int totalPieceValue = (queenCount * pieceValue[MENTERI]) + 
                          (minorPieceCount * pieceValue[KUDA]) + // Asumsi kuda dan gajah punya nilai rata-rata
                          (rookCount * pieceValue[BENTENG]);

    // Threshold ini bisa disesuaikan
    const int OPENING_THRESHOLD = 2500; // Contoh: Nilai material tinggi
    const int ENDGAME_THRESHOLD = 1000;  // Contoh: Nilai material rendah

    if (totalPieceValue > OPENING_THRESHOLD) return OPENING;
    if (totalPieceValue < ENDGAME_THRESHOLD) return ENDGAME;
    return MIDGAME;
}

// --- NEW EVALUATION FACTORS ---

int evaluatePieceActivity(GameState* state) {
    int whiteActivity = 0;
    int blackActivity = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == TIDAK_ADA || piece.tipe == RAJA) continue; // Raja aktivitasnya di KingSafety

            int activityScore = 0;
            // Faktor aktivitas: bidak di pusat, bidak yang menyerang/melindungi
            
            // Centralization for minor pieces (knights and bishops)
            if (piece.tipe == KUDA || piece.tipe == GAJAH) {
                // Berikan bonus untuk bidak di 4 kotak pusat (d4, e4, d5, e5) atau di sekitar pusat
                if ((x >= 2 && x <= 5) && (y >= 2 && y <= 5)) { // Inner 4x4 square
                    activityScore += 10;
                }
            }

            // Rooks on open/half-open files
            if (piece.tipe == BENTENG) {
                boolean isOpenFile = true;
                boolean isHalfOpenFile = false;
                for (int r = 0; r < 8; r++) {
                    Bidak p = state->papan.grid[r][x];
                    if (p.tipe == PION && p.warna == piece.warna) { // Pion sendiri menghalangi
                        isOpenFile = false;
                        break;
                    }
                    if (p.tipe == PION && p.warna != piece.warna) { // Pion lawan di file yang sama
                        isHalfOpenFile = true;
                    }
                }
                if (isOpenFile) activityScore += 20;
                else if (isHalfOpenFile) activityScore += 10;

                // Rooks on 7th rank (for white, rank 2; for black, rank 7)
                if ((piece.warna == PUTIH && y == 1) || (piece.warna == HITAM && y == 6)) {
                    activityScore += 15;
                }
            }

            // Bishops on long diagonals
            if (piece.tipe == GAJAH) {
                // Berikan bonus jika gajah berada di diagonal yang panjang dan terbuka
                // Contoh: Diagonal a1-h8 atau h1-a8
                if (abs(x - y) == 0 || abs(x - y) == 7) activityScore += 10; // Main diagonals
            }

            if (piece.warna == PUTIH) {
                whiteActivity += activityScore;
            } else {
                blackActivity += activityScore;
            }
        }
    }
    return whiteActivity - blackActivity;
}

int evaluateCenterControl(GameState* state) {
    int whiteCenterControl = 0;
    int blackCenterControl = 0;

    // Squares d4, e4, d5, e5 are central squares.
    // Squares c3, d3, e3, f3, c4, f4, c5, f5, c6, d6, e6, f6 are extended center.
    // Give points for occupying, attacking, and defending these squares.

    // Center squares (d4, e4, d5, e5)
    int centerSquares[4][2] = {{3,3}, {3,4}, {4,3}, {4,4}}; // col, row

    for (int i = 0; i < 4; i++) {
        int cx = centerSquares[i][0];
        int cy = centerSquares[i][1];
        
        Bidak piece = state->papan.grid[cy][cx]; // Get piece at center square

        if (piece.tipe != TIDAK_ADA) {
            if (piece.warna == PUTIH) whiteCenterControl += 20; // Occupying center
            else blackCenterControl += 20;
        }

        // Check who attacks this center square
        // Note: This can be expensive. A precomputed attack map would be better.
        // For simplicity, using isSquareAttacked which iterates through all pieces.
        if (isSquareAttacked(state, cx, cy, PUTIH)) {
            whiteCenterControl += 10; // Attacking center
        }
        if (isSquareAttacked(state, cx, cy, HITAM)) {
            blackCenterControl += 10;
        }
    }
    
    return whiteCenterControl - blackCenterControl;
}

//#include "evaluate_system.h"
//
//const int pieceValue[7] = {
//    0,    // TIDAK_ADA or empty
//    10,  // Pawn
//    30,  // Knight
//    30,  // Bishop
//    50,  // Rook
//    90,  // Queen
//    1000 // King
//};
//
//int evaluateState(GameState* state) {
//
//	// Evaluasi dasar material
//	int materialScore = evaluateMaterial(state);
//
//	// Evaluasi posisi bidak
//	int positionalScore = evaluatePositional(state);
//
//	// Evaluasi keselamatan raja
//    int kingSafetyScore = evaluateKingSafety(state);
//
//    // Evaluasi mobilitas dan ruang
//    int mobilityScore = evaluateMobility(state);
//
//    // Evaluasi struktur pion
//    int pawnStructureScore = evaluatePawnStructure(state);
//
//    // Evaluasi fase permainan
//    int gamePhase = determineGamePhase(state);
//    int phaseWeight = (gamePhase == OPENING) ? 2 : (gamePhase == MIDGAME) ? 1 : 0;
//
//    // Total score dengan bobot berbeda berdasarkan fase permainan
//    int totalScore = materialScore 
//                   + positionalScore
//                   + (kingSafetyScore * phaseWeight)
//                   + (mobilityScore * (gamePhase != ENDGAME ? 1 : 0))
//                   + pawnStructureScore;
//
//    // Adjust untuk giliran
//    return state->giliran->warna == PUTIH ? totalScore : -totalScore;
//
//    int skor = 0;
//    int x, y;
//    for (y = 0; y < 8; y++) {
//        for (x = 0; x < 8; x++) {
//            Bidak b = state->papan.grid[y][x];
//            
//            switch (b.tipe) {
//            	case BENTENG:
//            		skor += (b.warna == PUTIH) ? 5 : -5;
//            		break;
//            		
//            	case KUDA:
//            		skor += (b.warna == PUTIH) ? 3 : -3;
//            		break;
//            		
//            	case GAJAH:
//            		skor += (b.warna == PUTIH) ? 3 : -3;
//            		break;
//            		
//            	case MENTERI:
//            		skor += (b.warna == PUTIH) ? 7 : -7;
//            		break;
//            		
//            	case PION:
//            		skor += (b.warna == PUTIH) ? 1 : -1;
//            		break;
//            		
//            	case RAJA:
//            		skor += (b.warna == PUTIH) ? 100 : -100;
//            		break;
//			}
//        }
//    }
//    
//    return skor;
//}
//
//int evaluateMaterial(GameState* state) {
//    
//    int whiteMaterial = 0;
//    int blackMaterial = 0;
//    
//    // Hitung material untuk setiap kotak
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe != TIDAK_ADA) {
//                int value = pieceValue[piece.tipe];
//                if (piece.warna == PUTIH) {
//                    whiteMaterial += value;
//                } else {
//                    blackMaterial += value;
//                }
//            }
//        }
//    }
//
//    return whiteMaterial - blackMaterial;
//}
//
///**
// * Evaluasi posisi bidak berdasarkan tabel nilai posisi, 
//   source tabel bidak : https://github.com/maksimKorzh/wukong/blob/master/src/wukong.c
// */
// 
//int evaluatePositional(GameState* state) {
//    // Tabel nilai posisi untuk setiap bidak
//    
//    const int pawnTable[8][8] = {
//        { 90,  90,  90,  90,  90,  90,  90,  90 },
//        { 30,  30,  30,  40,  40,  30,  30,  30 },
//        { 20,  20,  20,  30,  30,  20,  20,  20 },
//        { 10,  10,  10,  20,  20,  10,  10,  10 },
//        {  5,   5,  10,  20,  20,   5,   5,   5 },
//        {  0,   0,   0,   5,   5,   0,   0,   0 },
//        {  0,   0,   0, -10, -10,   0,   0,   0 },
//        {  0,   0,   0,   0,   0,   0,   0,   0 }
//    };
//    
//	const int knightTable[8][8] = {
//		{ -5,   0,   0,   0,   0,   0,   0,  -5 },
//        { -5,   0,   0,  10,  10,   0,   0,  -5 },
//        { -5,   5,  20,  20,  20,  20,   5,  -5 },
//        { -5,  10,  20,  30,  30,  20,  10,  -5 },
//        { -5,  10,  20,  30,  30,  20,  10,  -5 },
//        { -5,   5,  20,  10,  10,  20,   5,  -5 },
//        { -5,   0,   0,   0,   0,   0,   0,  -5 },
//        { -5, -10,   0,   0,   0,   0, -10,  -5 }
//	};
//	
//	const int bishopTable[8][8] = {
//		{  0,   0,   0,   0,   0,   0,   0,   0 },
//        {  0,   0,   0,   0,   0,   0,   0,   0 },
//        {  0,   0,   0,  10,  10,   0,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,  10,   0,   0,   0,   0,  10,   0 },
//        {  0,  30,   0,   0,   0,   0,  30,   0 },
//        {  0,   0, -10,   0,   0, -10,   0,   0 }
//	};
//
//    const int rookTable[8][8] = {
//		{ 50,  50,  50,  50,  50,  50,  50,  50 },
//        { 50,  50,  50,  50,  50,  50,  50,  50 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,   0,  20,  20,   0,   0,   0 }
//	};
//    
//	const int queenTable[8][8] = {
//		{ 50,  50,  50,  50,  50,  50,  50,  50 },
//        { 50,  50,  50,  50,  50,  50,  50,  50 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,  10,  20,  20,  10,   0,   0 },
//        {  0,   0,   0,  20,  20,   0,   0,   0 }
//	};
//    
//	const int kingTable[8][8] = {
//		{  0,   0,   0,   0,   0,   0,   0,   0 },
//        {  0,   0,   5,   5,   5,   5,   0,   0 },
//        {  0,   5,   5,  10,  10,   5,   5,   0 },
//        {  0,   5,  10,  20,  20,  10,   5,   0 },
//        {  0,   5,  10,  20,  20,  10,   5,   0 },
//        {  0,   0,   5,  10,  10,   5,   0,   0 },
//        {  0,  -5,  -5,  -5,  -5,  -5,  -5,   0 },
//        {  0,   0,   5,   0, -15,   0,  10,   0 }
//	};
//    
//    int positionalScore = 0;
//    
//    for (int y = 8; y > 0; y--) {
//        for (int x = 0; x > 0; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe != TIDAK_ADA) {
//                int sign = (piece.warna == PUTIH) ? 1 : -1;
//                
//                switch (piece.tipe) {
//                    case PION:
//                        positionalScore += sign * pawnTable[piece.warna == PUTIH ? y : 7-y][x];
//                        break;
//                        
//                    case BENTENG:
//                    	positionalScore += sign * rookTable[piece.warna == PUTIH ? y : 7-y][x];
//                    	break;
//  
//                    case KUDA:
//                    	positionalScore += sign * knightTable[piece.warna == PUTIH ? y : 7-y][x];
//                    	break;
//
//                    case GAJAH:
//                    	positionalScore += sign * bishopTable[piece.warna == PUTIH ? y : 7-y][x];
//                    	break;
//
//                    case MENTERI:
//                    	positionalScore += sign * queenTable[piece.warna == PUTIH ? y : 7-y][x];
//                    	break;
//
//                    case RAJA:
//                    	positionalScore += sign * kingTable[piece.warna == PUTIH ? y : 7-y][x];
//                    	break;
//                }
//            }
//        }
//    }
//    
//    return positionalScore;
//}
//
///**
// * Evaluasi keselamatan raja berdasarkan proteksi dan ancaman
// */
//int evaluateKingSafety(GameState* state) {
//    int whiteKingSafety = 0;
//    int blackKingSafety = 0;
//
//    // Temukan posisi raja
//    Position whiteKingPos = findKingPosition(state, PUTIH);
//    Position blackKingPos = findKingPosition(state, HITAM);
//    
//    // Hitung pawn shield
//    whiteKingSafety += evaluatePawnShield(state, whiteKingPos, PUTIH);
//    blackKingSafety += evaluatePawnShield(state, blackKingPos, HITAM);
//    
//    // Hitung serangan ke kotak di sekitar raja
//    whiteKingSafety -= evaluateKingAttack(state, whiteKingPos, HITAM);
//    blackKingSafety -= evaluateKingAttack(state, blackKingPos, PUTIH);
//    
//    return whiteKingSafety - blackKingSafety;
//}
//
//Position findKingPosition(GameState* state, WarnaBidak warna) {
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe == RAJA && piece.warna == warna) return (Position){x, y};
//        }
//    }
//    
//    return (Position){-1, -1}; // Tidak ditemukan 
//}
//
//int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor) {
//    int attackScore = 0;
//    
//    // Cek semua bidak lawan yang mengancam kotak sekitar raja
//    for (int dy = -1; dy <= 1; dy++) {
//        for (int dx = -1; dx <= 1; dx++) {
//            if (dx == 0 && dy == 0) continue; // Lewati posisi raja sendiri
//            
//            int x = kingPos.col + dx;
//            int y = kingPos.row + dy;
//            if (x < 0 || x >= 8 || y < 0 || y >= 8) continue;
//
//            if (isSquareAttacked(state, x, y, attackerColor)) {
//                attackScore += 10; // Penalty untuk setiap ancaman
//            }
//        }
//    }
//    return attackScore;
//}
//
///**
// * Mengecek apakah kotak (x,y) diserang oleh bidak lawan dengan warna tertentu
// * @return true jika kotak dalam keadaan diserang, false jika tidak
// */
//boolean isSquareAttacked(GameState* state, int x, int y, WarnaBidak attackerColor) {
//    // Cek serangan dari pion
//    int pawnDir = (attackerColor == PUTIH) ? 1 : -1;
//    Position pawnAttacks[2] = {
//        {x-1, y + pawnDir},  // Serangan diagonal kiri
//        {x+1, y + pawnDir}   // Serangan diagonal kanan
//    };
//    
//    for (int i = 0; i < 2; i++) {
//        Position pos = pawnAttacks[i];
//        if (pos.col >= 0 && pos.col < 8 && pos.row >= 0 && pos.row < 8) {
//            Bidak piece = getBidakAt(state->papan, pos.col, pos.row);
//            if (piece.tipe == PION && piece.warna == attackerColor) {
//                return true;
//            }
//        }
//    }
//
//    // Cek serangan dari kuda (knight)
//    Position knightMoves[8] = {
//        {x-2, y-1}, {x-1, y-2}, {x+1, y-2}, {x+2, y-1},
//        {x+2, y+1}, {x+1, y+2}, {x-1, y+2}, {x-2, y+1}
//    };
//    
//    for (int i = 0; i < 8; i++) {
//        Position pos = knightMoves[i];
//        if (pos.col >= 0 && pos.col < 8 && pos.row >= 0 && pos.row < 8) {
//            Bidak piece = getBidakAt(state->papan, pos.col, pos.row);
//            if (piece.tipe == KUDA && piece.warna == attackerColor) {
//                return true;
//            }
//        }
//    }
//
//    // Cek serangan garis lurus (benteng/ratu)
//    Position straightDirs[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
//    for (int i = 0; i < 4; i++) {
//        Position dir = straightDirs[i];
//        for (int step = 1; step < 8; step++) {
//            int cx = x + dir.col * step;
//            int cy = y + dir.row * step;
//            
//            if (cx < 0 || cx >= 8 || cy < 0 || cy >= 8) break;
//            
//            Bidak piece = getBidakAt(state->papan, cx, cy);
//            if (piece.tipe != TIDAK_ADA) {
//                if (piece.warna == attackerColor && 
//                    (piece.tipe == BENTENG || piece.tipe == MENTERI)) {
//                    return true;
//                }
//                break;  // Terhalang bidak lain
//            }
//        }
//    }
//
//    // Cek serangan diagonal (gajah/ratu)
//    Position diagonalDirs[4] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
//    for (int i = 0; i < 4; i++) {
//        Position dir = diagonalDirs[i];
//        for (int step = 1; step < 8; step++) {
//            int cx = x + dir.col * step;
//            int cy = y + dir.row * step;
//            
//            if (cx < 0 || cx >= 8 || cy < 0 || cy >= 8) break;
//            
//            Bidak piece = getBidakAt(state->papan, cx, cy);
//            if (piece.tipe != TIDAK_ADA) {
//                if (piece.warna == attackerColor && 
//                    (piece.tipe == GAJAH || piece.tipe == MENTERI)) {
//                    return true;
//                }
//                break;  // Terhalang bidak lain
//            }
//        }
//    }
//
//    // Cek serangan raja (jarak 1 kotak)
//    for (int dy = -1; dy <= 1; dy++) {
//        for (int dx = -1; dx <= 1; dx++) {
//            if (dx == 0 && dy == 0) continue;
//            
//            int cx = x + dx;
//            int cy = y + dy;
//            if (cx >= 0 && cx < 8 && cy >= 0 && cy < 8) {
//                Bidak piece = getBidakAt(state->papan, cx, cy);
//                if (piece.tipe == RAJA && piece.warna == attackerColor) {
//                    return true;
//                }
//            }
//        }
//    }
//
//    return false;
//}
///**
// * Evaluasi mobilitas bidak dan kontrol ruang
// */
//int evaluateMobility(GameState* state) {
//    int whiteMobility = 0;
//    int blackMobility = 0;
//    
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe != TIDAK_ADA) {
//                // Hitung jumlah langkah legal untuk bidak ini
//                int moves = countLegalMoves(state->papan, x, y, state->giliran);
//
//                if (piece.warna == PUTIH) {
//                    whiteMobility += moves;
//                } else {
//                    blackMobility += moves;
//                }
//            }
//        }
//    }
//    
//    return whiteMobility - blackMobility;
//}
//
///**
// * Evaluasi struktur pion (doubled, isolated, passed pawns)
// */
//int evaluatePawnStructure(GameState* state) {
//    int whitePawnStructure = 0;
//    int blackPawnStructure = 0;
//    
//    PionAnalyze white = analyzePion(state, PUTIH);
//	PionAnalyze black = analyzePion(state, HITAM);
//	
//	whitePawnStructure += -10 * white.doubled - 15 * white.isolated + 20 * white.passed;
//	blackPawnStructure -= -10 * black.doubled - 15 * black.isolated + 20 * black.passed;
//    
//    return whitePawnStructure - blackPawnStructure;
//}
//
///** 
// * Evaluasi pion yang melindungi raja
// */
//int evaluatePawnShield(GameState* state, Position kingPos, WarnaBidak warna) {
//	int shieldScore = 0;
//    int forward = (warna == PUTIH) ? 1 : -1;
//    
//    // Cek pion di kolom adjasen (ï¿½1 dari kolom raja)
//    for (int dx = -1; dx <= 1; dx++) {
//        int x = kingPos.col + dx;
//        if (x < 0 || x >= 8) continue;
//        
//        int y = kingPos.row + forward;
//        if (y < 0 || y >= 8) continue;
//        
//        Bidak piece = state->papan.grid[y][x];
//        if (piece.tipe == PION && piece.warna == warna) {
//            shieldScore += 15; // Bonus untuk setiap pion pelindung
//        }
//    }
//    
//    return shieldScore;
//}
//
//PionAnalyze analyzePion(GameState* state, WarnaBidak warna) {
//	PionAnalyze result;
//	int pawnCountPerFile[8] = {0};
//	
//	// Hitung doubled: hitung pion di tiap file
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe == PION && piece.warna == warna) {
//                pawnCountPerFile[x]++;
//            }
//        }
//    }
//    
//    for (int x = 0; x < 8; x++) {
//        if (pawnCountPerFile[x] > 1) {
//            result.doubled += pawnCountPerFile[x] - 1;
//        }
//    }
//    
//    // Hitung isolated & passed
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe == PION && piece.warna == warna) {
//                // Cek isolated
//                boolean leftFileHasPawn = (x > 0) && pawnCountPerFile[x - 1] > 0;
//                boolean rightFileHasPawn = (x < 7) && pawnCountPerFile[x + 1] > 0;
//                if (!leftFileHasPawn && !rightFileHasPawn) {
//                    result.isolated++;
//                }
//
//                // Cek passed
//                boolean isPassed = true;
//                int startRow, endRow, step;
//                if (warna == PUTIH) {
//                    startRow = y - 1;
//                    endRow = -1;
//                    step = -1;
//                } else {
//                    startRow = y + 1;
//                    endRow = 8;
//                    step = 1;
//                }
//
//                for (int row = startRow; row != endRow; row += step) {
//                    for (int col = x - 1; col <= x + 1; col++) {
//                        if (col >= 0 && col < 8) {
//                            Bidak frontPiece = state->papan.grid[row][col];
//                            if (frontPiece.tipe == PION && frontPiece.warna != warna) {
//                                isPassed = false;
//                                break;
//                            }
//                        }
//                    }
//                    if (!isPassed) break;
//                }
//
//                if (isPassed) {
//                    result.passed++;
//                }
//            }
//        }
//    }
//
//    return result;
//
//}
//
///**
// * Menentukan fase permainan berdasarkan material
// */
//GamePhase determineGamePhase(GameState* state) {
//    int queenCount = 0;
//    int minorPieceCount = 0;
//    
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe == MENTERI) queenCount++;
//            if (piece.tipe == KUDA || piece.tipe == GAJAH) minorPieceCount++;
//        }
//    }
//    
//    if (queenCount == 2 && minorPieceCount >= 4) return OPENING;
//    if (queenCount >= 1) return MIDGAME;
//    return ENDGAME;
//}
//

