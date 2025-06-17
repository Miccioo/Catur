#include "validator.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "..\game\bidak.h"
#define MAX_MOVES 256

boolean isPathClear(Papan papan, Position from, Position to) {
    // Calculate movement direction
    int dx = 0;
    if (to.col > from.col) dx = 1;
    else if (to.col < from.col) dx = -1;

    int dy = 0;
    if (to.row > from.row) dy = 1;
    else if (to.row < from.row) dy = -1;
    
    // Starting point for path checking (after the origin square)
    Position current = {from.row + dy, from.col + dx};
    
    // Loop as long as the target square has not been reached
    while (current.row != to.row || current.col != to.col) {
        // Check if there is a piece in the path
        Bidak bidakDiJalur = getBidakAt(papan, current.col, current.row);
        if (bidakDiJalur.id != -1) {
            return false; // Path is not clear
        }
        // Continue to the next square in the path
        current.row += dy;
        current.col += dx;
    }
    return true; // Path is clear
}

boolean isValidMove(Papan papan, Move* move, Player* currentPlayer) {
    // Get the piece at the starting position
    Bidak piece = getBidakAt(papan, move->from.col, move->from.row);

    // 1. Validate piece existence and ownership
    if (piece.id == -1 || piece.warna != currentPlayer->warna) {
        return false;
    }

    // Calculate row and column differences (delta x and delta y)
    int dx = move->to.col - move->from.col;
    int dy = move->to.row - move->from.row;
    
    // Get the piece at the target position
    Bidak target = getBidakAt(papan, move->to.col, move->to.row);
    // Determine if this is a capture move
    boolean isCapture = (target.id != -1 && target.warna != piece.warna);
    // Determine if the target square is occupied by own piece (illegal move)
    boolean isTargetOwnPiece = (target.id != -1 && target.warna == piece.warna);

    // Any move must not end on a square occupied by own piece
    if (isTargetOwnPiece) {
        return false;
    }

    // Validation logic based on piece type
    switch (piece.tipe) {
        case PION: {
            int direction = (piece.warna == PUTIH) ? -1 : 1; // White moves up (-row), Black moves down (+row)
            int startRow = (piece.warna == PUTIH) ? 6 : 1; // White pawn start row (index 6), Black (index 1)

            // Move 1 square forward
            if (dx == 0 && dy == direction && target.id == -1) {
                return true;
            }
            // Move 2 squares forward from starting position
            if (dx == 0 && dy == 2 * direction && move->from.row == startRow &&
                getBidakAt(papan, move->from.col, move->from.row + direction).id == -1 && // Ensure square in front is not blocked
                target.id == -1) {
                return true;
            }
            // Diagonal capture move
            if (abs(dx) == 1 && dy == direction && isCapture) {
                return true;
            }
            break; 
        }
        case BENTENG: // Rook
            if ((dx == 0 || dy == 0) && isPathClear(papan, move->from, move->to)) { // Moves horizontally or vertically
                return true;
            }
            break;
        case MENTERI: // Queen
            if ((abs(dx) == abs(dy) || dx == 0 || dy == 0) && isPathClear(papan, move->from, move->to)) { // Moves diagonally, horizontally, or vertically
                return true;
            }
            break;
        case GAJAH: // Bishop
            if (abs(dx) == abs(dy) && isPathClear(papan, move->from, move->to)) { // Moves diagonally
                return true;
            }
            break;
        case KUDA: // Knight
            // 'L' shape move
            if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) { // Knight does not require isPathClear
                return true;
            }
            break;
        case RAJA: // King
            if (abs(dx) <= 1 && abs(dy) <= 1) { // Moves one square in any direction
                return true;
            }
            break;
        case TIDAK_ADA:
            return false; // Should not validate TIDAK_ADA piece
    }

    return false;
}

Move* generateAllValidMoves(Papan papan, Player* currentPlayer) {
    Move* moves = (Move*)malloc(sizeof(Move) * MAX_MOVES);
    if (moves == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for moves in generateAllValidMoves.\n");
        return NULL;
    }
    
    int count = 0;
	
	int row, col, toRow, toCol;
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            // getBidakAt(board, column, row)
            Bidak piece = getBidakAt(papan, col, row); 

            if (piece.id == -1 || piece.warna != currentPlayer->warna) {
                continue; 
            }

            for (toRow = 0; toRow < 8; toRow++) {
                for (toCol = 0; toCol < 8; toCol++) {
                    Move currentMove;
                    createMove(&currentMove, 
                               (Position){row, col},
                               (Position){toRow, toCol},
                               piece.tipe);
                    
                    if (isValidMove(papan, &currentMove, currentPlayer)) {
                        moves[count++] = currentMove;
                        if (count >= MAX_MOVES - 1) {
                            moves[count].bidak = TIDAK_ADA; 
                            return moves;
                        }
                    }
                }
            }
        }
    }

    moves[count].bidak = TIDAK_ADA; 
    return moves;
}
