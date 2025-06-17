#include "precompute_table.h"

void generatePawnMoves(Position pos, Move* moves, int* count, Player* player, Papan papan) {
    int forward = (player->giliran == 1) ? 1 : -1;

    // Normal move
    Position target = {pos.col, pos.row + 1};
    if (isEmptyBidak(&papan, target.col, target.row)) {
        createMove(moves, count, pos, target, PION);

        // Langkah 2 kotak dari posisi awal
        if (pos.row == 1 || pos.row == 6) {
            Position doubleTarget = {pos.col, pos.row + 2*forward};
            if (isEmptyBidak(&papan, doubleTarget.col, doubleTarget.row)) {
                createMove(moves, count, pos, doubleTarget, PION);
            }
        }
    
        // Capture moves
        for (int i = -1; i <= 1; i += 2) {
            target.col = pos.col + i;
            target.row = pos.row + 1;
            if (isEmptyBidak(&papan, target.col, target.row)) {
                createMove(moves, count, pos, target, PION);
            }
        }
    }
}

void generateBishopMoves(Position pos, Move* moves, int* count, Player* player, Papan papan) {
    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + bishopMoves[i][0] * step, 
                            pos.row + bishopMoves[i][1] * step};
            if (!isValidSquare(target))  break;
            
            if (isEmptyBidak(&papan, target.col, target.row)) {
                createMove(moves, count, pos, target, GAJAH);
            } else {
                // If there's a piece, we can capture it
                Bidak targetPiece = getBidakAt(papan, target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, count, pos, target, GAJAH);
                }
                break; // Stop if we hit a piece
            }
        }
    }
}

void generateKnightMoves(Position pos, Move* moves, int* count, Player* player, Papan papan) {
    for (int i = 0; i < 8; i++) {
        Position target = {pos.col + knightMoves[i][0], 
                        pos.row + knightMoves[i][1]};
        if (!isValidSquare(target)) break;
        if (isEmptyBidak(&papan, target.col, target.row)) {
            createMove(moves, count, pos, target, KUDA);
        } else {
            // If there's a piece, we can capture it
            Bidak targetPiece = getBidakAt(papan, target.col, target.row);
            if (targetPiece.warna != player->warna) {
                createMove(moves, count, pos, target, KUDA);
            }
        }

    }
}

void generateRookMoves(Position pos, Move* moves, int* count, Player* player, Papan papan) {
    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + rookMoves[i][0] * step, 
                            pos.row + rookMoves[i][1] * step};
            if (isValidSquare(target)) break;
            if (isEmptyBidak(&papan, target.col, target.row)) {
                createMove(moves, count, pos, target, BENTENG);          
            } else {
                // If there's a piece, we can capture it
                Bidak targetPiece = getBidakAt(papan, target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, count, pos, target, BENTENG); 
                }
                break; // Stop if we hit a piece
            }
        }
    }
}

void generateQueenMoves(Position pos, Move* moves, int* count, Player* player, Papan papan) {
    // Generate rook-like moves
    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + queenMoves[i][0] * step, 
                            pos.row + queenMoves[i][1] * step};
            if (isValidSquare(target)) break;
            if (isEmptyBidak(&papan, target.col, target.row)) {
                createMove(moves, count, pos, target, MENTERI);
                
            } else {
                // If there's a piece, we can capture it
                Bidak targetPiece = getBidakAt(papan, target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, count, pos, target, MENTERI);
                }
                break; // Stop if we hit a piece
            }
        }
    }
    
    // Generate bishop-like moves
    for (int i = 4; i < 8; i++) {
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + queenMoves[i][0] * step, 
                            pos.row + queenMoves[i][1] * step};
            if (isValidSquare(target)) break;
            if (isEmptyBidak(&papan, target.col, target.row)) {
                createMove(moves, count, pos, target, MENTERI);
            } else {
                // If there's a piece, we can capture it
                Bidak targetPiece = getBidakAt(papan, target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, count, pos, target, MENTERI);
                }
                break; // Stop if we hit a piece
            }
        }
    }
}

void generateKingMoves(Position pos, Move* moves, int* count) {
    for (int i = 0; i < 8; i++) {
        Position target = {pos.col + kingMoves[i][0], 
                        pos.row + kingMoves[i][1]};
        if (isValidSquare(target)) {
            createMove(moves, count, pos, target, RAJA);
            
        }
    }
}