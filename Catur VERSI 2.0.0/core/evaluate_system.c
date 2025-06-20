#include "evaluate_system.h" // Include evaluate_system.h FIRSTq

// Piece values for material evaluation
const int pieceValues[6] = {
    0,   // TIDAK_ADA (Dummy value, not used for actual piece)
    10,  // PION
    30,  // KUDA
    30,  // GAJAH
    50,  // BENTENG
    90,  // MENTERI
};

// Tabel nilai posisi untuk setiap bidak
// Source: https://github.com/maksimKorzh/wukong/blob/master/src/wukong.c
// Adjusted tables (if any, ensure they are correct based on the source)
const int pawnTableWhite[8][8] = {
    { 90,  90,  90,  90,  90,  90,  90,  90 },
    { 30,  30,  30,  40,  40,  30,  30,  30 },
    { 20,  20,  20,  30,  30,  20,  20,  20 },
    { 10,  10,  10,  20,  20,  10,  10,  10 },
    {  5,   5,  10,  20,  20,   -5,   5,   5 },
    {  0,   0,   0,   5,   5,   0,   0,   0 },
    {  0,   0,   0, -10, -10,   0,   0,   0 },
    {  0,   0,   0,   0,   0,   0,   0,   0 }
};

const int knightTableWhite[8][8] = {
    { -5,   0,   0,   0,   0,   0,   0,  -5 },
    { -5,   0,   0,  10,  10,   0,   0,  -5 },
    { -5,   5,  20,  20,  20,  20,   5,  -5 },
    { -5,  10,  20,  30,  30,  20,  10,  -5 },
    { -5,  10,  20,  30,  30,  20,  10,  -5 },
    { -5,   5,  20,  10,  10,  20,   5,  -5 },
    { -5,   0,   0,   0,   0,   0,   0,  -5 },
    { -5, -10,   0,   0,   0,   0, -10,  -5 }
};

const int bishopTableWhite[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0 },
    {  0,   0,   0,   0,   0,   0,   0,   0 },
    {  0,   0,   0,  10,  10,   0,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,  10,   0,   0,   0,   0,  10,   0 },
    {  0,  30,   0,   0,   0,   0,  30,   0 },
    {  0,   0, -10,   0,   0, -10,   0,   0 }
};

const int rookTableWhite[8][8] = {
    { 50,  50,  50,  50,  50,  50,  50,  50 },
    { 50,  50,  50,  50,  50,  50,  50,  50 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,   0,  20,  20,   0,   0,   0 }
};

const int queenTableWhite[8][8] = {
    { -10, -5, -5, -2, -2, -5, -5, -10 },
    {  -5,  0,  0,  0,  0,  0,  0,  -5 },
    {  -5,  0,  2,  2,  2,  2,  0,  -5 },
    {  -2,  0,  2,  5,  5,  2,  0,  -2 },
    {  -2,  0,  2,  5,  5,  2,  0,  -2 },
    {  -5,  0,  2,  2,  2,  2,  0,  -5 },
    {  -5,  0,  0,  0,  0,  0,  0,  -5 },
    { -10, -5, -5, -2, -2, -5, -5, -10 }
};

const int kingTableWhite[8][8] = { // Opening/Midgame King Safety Table
    {  -40, -24, -24, -24, -24, -24, -24, -40 }, // This table is for general king position,
    {  -24, -24,   0,   0,   0,   0, -24, -24 }, // not exclusively for safety (handled by king shield/attack)
    {  -24,  -8,  16,  24,  24,  16,  -8, -24 },
    {  -24,  -8,  24,  32,  32,  24,  -8, -24 },
    {  -24,  -8,  24,  32,  32,  24,  -8, -24 },
    {  -24,  -8,  16,  24,  24,  16,  -8, -24 },
    {  -24, -16,  -8,   0,   0,  -8, -16, -24 },
    {  -40, -32,  30, -16,   0, -24,  30, -40 }
};

const int kingEndgameTableWhite[8][8] = { // Endgame King Positional Table
    { -50, -40, -30, -20, -20, -30, -40, -50 },
    { -40, -20,   0,   0,   0,   0, -20, -40 },
    { -30,   0,  20,  30,  30,  20,   0, -30 },
    { -20,   0,  30,  40,  40,  30,   0, -20 },
    { -20,   0,  30,  40,  40,  30,   0, -20 },
    { -30,   0,  20,  30,  30,  20,   0, -30 },
    { -40, -20,   0,   0,   0,   0, -20, -40 },
    { -50, -40, -30, -20, -20, -30, -40, -50 }
};

const int pawnTableBlack[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0 },
    {  0,   0,   0, -10, -10,   0,   0,   0 },
    {  0,   0,   0,   5,   5,   0,   0,   0 },
    {  5,   5,  10,  20,  20,  -5,   5,   5 },
    { 10,  10,  10,  20,  20,  10,  10,  10 },
    { 20,  20,  20,  30,  30,  20,  20,  20 },
    { 30,  30,  30,  40,  40,  30,  30,  30 },
    { 90,  90,  90,  90,  90,  90,  90,  90 }
};


const int knightTableBlack[8][8] = {
    { -5, -10,   0,   0,   0,   0, -10,  -5 },
    { -5,   0,   0,   0,   0,   0,   0,  -5 },
    { -5,   5,  20,  10,  10,  20,   5,  -5 },
    { -5,  10,  20,  30,  30,  20,  10,  -5 },
    { -5,  10,  20,  30,  30,  20,  10,  -5 },
    { -5,   5,  20,  20,  20,  20,   5,  -5 },
    { -5,   0,   0,  10,  10,   0,   0,  -5 },
    { -5,   0,   0,   0,   0,   0,   0,  -5 }
};

const int bishopTableBlack[8][8] = {
    {  0,   0, -10,   0,   0, -10,   0,   0 },
    {  0,  30,   0,   0,   0,   0,  30,   0 },
    {  0,  10,   0,   0,   0,   0,  10,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,   0,  10,  10,   0,   0,   0 },
    {  0,   0,   0,   0,   0,   0,   0,   0 },
    {  0,   0,   0,   0,   0,   0,   0,   0 }
};

const int rookTableBlack[8][8] = {
    {  0,   0,   0,  20,  20,   0,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    {  0,   0,  10,  20,  20,  10,   0,   0 },
    { 50,  50,  50,  50,  50,  50,  50,  50 },
    { 50,  50,  50,  50,  50,  50,  50,  50 }
};

const int queenTableBlack[8][8] = {
    { -10, -5, -5, -2, -2, -5, -5, -10 },
    {  -5,  0,  0,  0,  0,  0,  0,  -5 },
    {  -5,  0,  2,  2,  2,  2,  0,  -5 },
    {  -2,  0,  2,  5,  5,  2,  0,  -2 },
    {  -2,  0,  2,  5,  5,  2,  0,  -2 },
    {  -5,  0,  2,  2,  2,  2,  0,  -5 },
    {  -5,  0,  0,  0,  0,  0,  0,  -5 },
    { -10, -5, -5, -2, -2, -5, -5, -10 }
};

const int kingTableBlack[8][8] = {
    { -40, -32,  30, -16,   0, -24,  30, -40 },
    { -24, -16,  -8,   0,   0,  -8, -16, -24 },
    { -24,  -8,  16,  24,  24,  16,  -8, -24 },
    { -24,  -8,  24,  32,  32,  24,  -8, -24 },
    { -24,  -8,  24,  32,  32,  24,  -8, -24 },
    { -24,  -8,  16,  24,  24,  16,  -8, -24 },
    { -24, -24,   0,   0,   0,   0, -24, -24 },
    { -40, -24, -24, -24, -24, -24, -24, -40 }
};

const int kingEndgameTableBlack[8][8] = {
    { -50, -40, -30, -20, -20, -30, -40, -50 },
    { -40, -20,   0,   0,   0,   0, -20, -40 },
    { -30,   0,  20,  30,  30,  20,   0, -30 },
    { -20,   0,  30,  40,  40,  30,   0, -20 },
    { -20,   0,  30,  40,  40,  30,   0, -20 },
    { -30,   0,  20,  30,  30,  20,   0, -30 },
    { -40, -20,   0,   0,   0,   0, -20, -40 },
    { -50, -40, -30, -20, -20, -30, -40, -50 }
};


/**
 * Evaluasi keadaan untuk dihitung dalam algoritma minimax
 */
int evaluateState(GameState* state) {
    // Determine game phase first
    int gamePhase = determineGamePhase(state);

    // Evaluate basic material
    int materialScore = evaluateMaterial(state);

    // Evaluate piece positions
    int positionalScore = evaluatePositional(state);

    // Evaluate king safety
    int kingSafetyScore = evaluateKingSafety(state);

    // Evaluate mobility and space control
    int mobilityScore = evaluateMobility(state);

    // Evaluate pawn structure
    int pawnStructureScore = evaluatePawnStructure(state);

    // Apply phase-dependent weights
    int totalScore = 0;

    totalScore += materialScore * MATERIAL_WEIGHT;
    totalScore += positionalScore * POSITIONAL_WEIGHT;
    totalScore += pawnStructureScore * PAWN_STRUCTURE_WEIGHT;
    totalScore += mobilityScore * MOBILITY_WEIGHT;

    // King safety is more important in opening/midgame
    if (gamePhase == OPENING || gamePhase == MIDGAME) {
        totalScore += kingSafetyScore * KING_SAFETY_WEIGHT_OPENING_MIDGAME;
    } else { // Endgame
        totalScore += kingSafetyScore * KING_SAFETY_WEIGHT_ENDGAME;
        // In endgame, king position often becomes more active, which is handled by positional table
        // But the king safety score from attacks/pawn shield is still relevant, just less weighted.
    }

    // Adjust score based on current turn
    // If it's White's turn, a positive score is good for White.
    // If it's Black's turn, a positive score is good for Black, so we negate if for minimax perspective.
    return (state->giliran->warna == PUTIH) ? totalScore : -totalScore;
}

/**
 * Evaluasi material yang ada di papan
 */
int evaluateMaterial(GameState* state) {
    // The `pieceValues` array is already defined globally to avoid re-creation
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
    int positionalScore = 0;
    int gamePhase = determineGamePhase(state);
    
    int (*pawnTable)[8];
	int (*knightTable)[8];
	int (*rookTable)[8];
	int (*bishopTable)[8];
	int (*queenTable)[8];
	int (*kingEndgameTable)[8];
	int (*kingTable)[8];
	
	if (state->giliran->warna == PUTIH) {
	    pawnTable = &pawnTableWhite[0];
	    knightTable = &knightTableWhite[0];
	    rookTable = &rookTableWhite[0];
	    bishopTable = &bishopTableWhite[0];
	    queenTable = &queenTableWhite[0];
	    kingEndgameTable = &kingEndgameTableWhite[0];
	    kingTable = &kingTableWhite[0];
	} else {
	    pawnTable = &pawnTableBlack[0];
	    knightTable = &knightTableBlack[0];
	    rookTable = &rookTableBlack[0];
	    bishopTable = &bishopTableBlack[0];
	    queenTable = &queenTableBlack[0];
	    kingEndgameTable = &kingEndgameTableBlack[0];
	    kingTable = &kingTableBlack[0];
	}

    
    

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                int sign = (piece.warna == PUTIH) ? 1 : -1;
                int row = (piece.warna == PUTIH) ? y : 7 - y; // Flip row for Black pieces
				
                switch (piece.tipe) {
                    case PION:
                        positionalScore += sign * pawnTable[row][x];
                        break;
                    case BENTENG:
                        positionalScore += sign * rookTable[row][x];
                        break;
                    case KUDA:
                        positionalScore += sign * knightTable[row][x];
                        break;
                    case GAJAH:
                        positionalScore += sign * bishopTable[row][x];
                        break;
                    case MENTERI:
                        positionalScore += sign * queenTable[row][x];
                        break;
                    case RAJA:
                        // King positional table depends on game phase
                        if (gamePhase == ENDGAME) {
                            positionalScore += sign * kingEndgameTable[row][x];
                        } else {
                            positionalScore += sign * kingTable[row][x];
                        }
                        break;
                    case TIDAK_ADA:
                        break; // Should not happen if piece.tipe != TIDAK_ADA
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
    Position whiteKingPos = findKingPosition(&state->papan, PUTIH);
    Position blackKingPos = findKingPosition(&state->papan, HITAM);

    // Hitung pawn shield
    whiteKingSafety += evaluatePawnShield(state, whiteKingPos, PUTIH);
    blackKingSafety += evaluatePawnShield(state, blackKingPos, HITAM);

    // Hitung serangan ke kotak di sekitar raja
    // Penalties for being attacked around the king
    whiteKingSafety -= evaluateKingAttack(state, whiteKingPos, HITAM);
    blackKingSafety -= evaluateKingAttack(state, blackKingPos, PUTIH);

    return whiteKingSafety - blackKingSafety;
}

int evaluateKingAttack(GameState* state, Position kingPos, WarnaBidak attackerColor) {
    int attackScore = 0;
    // Values for attacking pieces, can be tuned
    const int pawnAttackValue = 10;
    const int knightAttackValue = 15;
    const int bishopAttackValue = 15;
    const int rookAttackValue = 20;
    const int queenAttackValue = 25;

    // Iterate through all squares on the board to find potential attackers
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Bidak attackerPiece = getBidakAt(state->papan, c, r);

            // If it's an opponent's piece
            if (attackerPiece.id != -1 && attackerPiece.warna == attackerColor) {
                // Check if this piece attacks any square around the king, including the king's square itself
                // We need to loop through the 3x3 square around the king
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        // Target square around the king
                        Position targetSquare = {kingPos.row + dy, kingPos.col + dx};

                        // Check if the target square is within bounds
                        if (targetSquare.row >= 0 && targetSquare.row < 8 &&
                            targetSquare.col >= 0 && targetSquare.col < 8) {

                            // Create a dummy move from attacker to target square
                            Move dummyMove;
                            // The 'from' position for isPseudoLegalMove is the attacker's actual position
                            createMove(&dummyMove, (Position){r, c}, targetSquare, attackerPiece.tipe);

                            // isPseudoLegalMove needs the current player, which is the attacker's color in this context
                            // and enPassantTargetPawn, which can be NULL for this check.
                            Player dummyPlayer; // Create a dummy player for isPseudoLegalMove call
                            dummyPlayer.warna = attackerColor;

                            // Check if the attacker pseudo-legally attacks the target square
                            // We use isPseudoLegalMove because isSquareAttacked (which would be more direct)
                            // is already defined using the piece type logic directly within its body.
                            // To avoid code duplication and rely on existing validation, we simulate a check.
                            // Note: isPseudoLegalMove considers capture rules, which is correct for attack.
                            if (isPseudoLegalMove(state->papan, &dummyMove, &dummyPlayer, state->enPassantTargetPawn)) {
                                // Add penalty based on attacking piece type
                                switch (attackerPiece.tipe) {
                                    case PION:
                                        // Pawns only attack diagonally, so if target is diagonal, it's a real threat
                                        // This check is implicitly handled by isPseudoLegalMove for pawns.
                                        if (abs(c - targetSquare.col) == 1 && abs(r - targetSquare.row) == 1) {
                                            attackScore += pawnAttackValue;
                                        }
                                        break;
                                    case KUDA:
                                        attackScore += knightAttackValue;
                                        break;
                                    case GAJAH:
                                        attackScore += bishopAttackValue;
                                        break;
                                    case BENTENG:
                                        attackScore += rookAttackValue;
                                        break;
                                    case MENTERI:
                                        attackScore += queenAttackValue;
                                        break;
                                    case RAJA:
                                        // King attacking is usually for opposition and doesn't directly contribute to safety score in this context
                                        // unless it's a very specific endgame scenario of zugzwang. For now, we omit it.
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return attackScore;
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
            if (piece.id != -1) { // Check if there's a piece
                // Temporarily set the current player to the piece's color to correctly count legal moves for that piece
                Player tempPlayer;
                tempPlayer.warna = piece.warna;

                // Use a temporary GameState for generateAllValidMoves or countLegalMoves
                // This is crucial because isLegalMove (called by countLegalMoves) needs a GameState pointer.
                GameState tempStateForMobility = *state; // Copy current state for modification
                tempStateForMobility.giliran = &tempPlayer; // Set the current player for this specific piece's turn

                // countLegalMoves returns the number of legal moves for a specific piece
                int moves = countLegalMoves(state->papan, x, y, &tempPlayer);

                // Assign mobility score based on piece type (can be weighted)
                // Example: Queen moves might be worth more than pawn moves
                int mobilityValue = moves; // Base mobility score
                switch (piece.tipe) {
                    case PION: mobilityValue *= 1; break; // Pawns are generally low mobility
                    case KUDA: mobilityValue *= 2; break; // Knights have good mobility
                    case GAJAH: mobilityValue *= 2; break; // Bishops have good mobility
                    case BENTENG: mobilityValue *= 3; break; // Rooks have high mobility
                    case MENTERI: mobilityValue *= 4; break; // Queen has highest mobility
                    case RAJA: mobilityValue *= 0.5; break; // King mobility is less about offense, more about safety
                    case TIDAK_ADA: break;
                }

                if (piece.warna == PUTIH) {
                    whiteMobility += mobilityValue;
                } else {
                    blackMobility += mobilityValue;
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

    // Penalties for doubled and isolated pawns, bonus for passed pawns
    // These values are examples and can be fine-tuned.
    whitePawnStructure += (-10 * white.doubled) + (-15 * white.isolated) + (20 * white.passed);
    blackPawnStructure += (-10 * black.doubled) + (-15 * black.isolated) + (20 * black.passed);

    return whitePawnStructure - blackPawnStructure;
}

/**
 * Evaluasi pion yang melindungi raja
 */
int evaluatePawnShield(GameState* state, Position kingPos, WarnaBidak warna) {
    int shieldScore = 0;
    int forward = (warna == PUTIH) ? -1 : 1; // Direction for pawn shield relative to king

    // Check pawns in adjacent columns and one rank forward
    for (int dx = -1; dx <= 1; dx++) {
        int x = kingPos.col + dx;
        // Ensure the column is within board bounds
        if (x < 0 || x >= 8) continue;

        int y = kingPos.row + forward;
        // Ensure the row is within board bounds
        if (y < 0 || y >= 8) continue;

        Bidak piece = state->papan.grid[y][x];
        if (piece.tipe == PION && piece.warna == warna) {
            shieldScore += 15; // Bonus for each protecting pawn
        }
    }
    return shieldScore;
}

PionAnalyze analyzePion(GameState* state, WarnaBidak warna) {
    PionAnalyze result = {0, 0, 0}; // Initialize to zeros
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
                // A pawn is isolated if it has no friendly pawns on adjacent files
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
                } else { // HITAM
                    startRow = y + 1;
                    endRow = 8;
                    step = 1;
                }

                for (int row = startRow; row != endRow; row += step) {
                    for (int col = x - 1; col <= x + 1; col++) {
                        if (col >= 0 && col < 8) {
                            Bidak frontPiece = state->papan.grid[row][col];
                            // A passed pawn has no opposing pawns on its own file or adjacent files ahead of it.
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
    int totalQueenCount = 0;
    int totalRookCount = 0;
    int totalMinorPieceCount = 0; // Knights and Bishops

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe == MENTERI) totalQueenCount++;
            if (piece.tipe == BENTENG) totalRookCount++;
            if (piece.tipe == KUDA || piece.tipe == GAJAH) totalMinorPieceCount++;
        }
    }

    // Heuristic for game phase
    // Opening: Both queens are still on board, and most minor pieces.
    if (totalQueenCount == 2 && (totalMinorPieceCount >= 3 || totalRookCount >= 2)) return OPENING;
    // Endgame: Few pieces left, usually one queen or no queens and few minor pieces/rooks.
    if (totalQueenCount <= 1 && totalRookCount <= 1 && totalMinorPieceCount <= 2) return ENDGAME;
    // Otherwise, Midgame
    return MIDGAME;
}

//#include "evaluate_system.h"
//
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
//    const int pieceValues[5] = {10, 30, 30, 50, 90}; // Pion, Kuda, Gajah, Benteng, Menteri
//    
//    int whiteMaterial = 0;
//    int blackMaterial = 0;
//    
//    // Hitung material untuk setiap kotak
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
//            Bidak piece = state->papan.grid[y][x];
//            if (piece.tipe != TIDAK_ADA) {
//                int value = pieceValues[piece.tipe];
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
//    int positionalScore = 0;
//    
//    for (int y = 0; y < 8; y++) {
//        for (int x = 0; x < 8; x++) {
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
//    Position whiteKingPos = findKingPosition(&state->papan, PUTIH);
//    Position blackKingPos = findKingPosition(&state->papan, HITAM);
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
//            Position pos;
//            pos.col = x;
//            pos.row = y;
//
//            if (isSquareAttacked(state->papan, pos, attackerColor)) {
//                attackScore += 10; // Penalty untuk setiap ancaman
//            }
//        }
//    }
//    return attackScore;
//}
//
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
//            	Position pos;
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
//    // Cek pion di kolom adjasen (?1 dari kolom raja)
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

