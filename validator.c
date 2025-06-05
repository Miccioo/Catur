#include "validator.h"
#include <stdlib.h>
#include <math.h>

boolean isPathClear(Papan papan, Position from, Position to) {
    int dx = (to.col - from.col) == 0 ? 0 : (to.col - from.col) / abs(to.col - from.col);
    int dy = (to.row - from.row) == 0 ? 0 : (to.row - from.row) / abs(to.row - from.row);
    
    Position current = {from.row + dy, from.col + dx};
    
    while (current.row != to.row || current.col != to.col) {
        if (getBidakAt(papan, current.col, current.row).id != -1) {
            return false;
        }
        current.row += dy;
        current.col += dx;
    }
    return true;
}

boolean isValidMove(Papan papan, Move* move, Player* currentPlayer) {
    Bidak piece = getBidakAt(papan, move->from.col, move->from.row);
    if (piece.id != -1 || piece.warna != currentPlayer->warna) {
        return false;
    }

    int dx = move->to.col - move->from.col;
    int dy = move->to.row - move->from.row;
    Bidak target = getBidakAt(papan, move->to.col, move->to.row);
    boolean isCapture = target.id != -1 && target.warna != piece.warna;

    switch (piece.tipe) {
        case PION: {
            int direction = (piece.warna == PUTIH) ? -1 : 1;
            int startRow = (piece.warna == PUTIH) ? 6 : 1;
            Bidak bidakCurrent = getBidakAt(papan, move->from.col, move->from.row + direction);

            if (dx == 0 && dy == direction && target.id == -1) {
                return true;
            }
            if (dx == 0 && dy == 2 * direction && move->from.row == startRow && bidakCurrent.id == -1 && target.id == -1) {
                return true;
            }
            if (abs(dx) == 1 && dy == direction && isCapture) {
                return true;
            }
            break;
        }
        case BENTENG:
            return (dx == 0 || dy == 0) && isPathClear(papan, move->from, move->to) && 
                   (target.id == -1 || target.warna != piece.warna);
        case MENTERI:
            return (abs(dx) == abs(dy) || dx == 0 || dy == 0) &&
                   isPathClear(papan, move->from, move->to) && 
                   (target.id == -1 || target.warna != piece.warna);
        case GAJAH:
            return abs(dx) == abs(dy) &&
                   isPathClear(papan, move->from, move->to) && 
                   (target.id == -1 || target.warna != piece.warna);
        case KUDA:
            return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2);
        case RAJA:
            return abs(dx) <= 1 && abs(dy) <= 1 && 
                   (target.id == -1 || target.warna != piece.warna);
    }

    return false;
}

#define MAX_MOVES 256

Move* generateAllValidMoves(Papan papan, Player* currentPlayer) {
    Move* moves = (Move*)malloc(sizeof(Move) * MAX_MOVES);
    if (moves == NULL) return NULL;
    
    int count = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Bidak piece = getBidakAt(papan, col, row);
            if (piece.id == -1 || piece.warna != currentPlayer->warna) continue;

            for (int toRow = 0; toRow < 8; toRow++) {
                for (int toCol = 0; toCol < 8; toCol++) {
                    Move move = createMove(
                        (Position){row, col},
                        (Position){toRow, toCol},
                        piece.tipe
                    );
                    
                    if (isValidMove(papan, &move, currentPlayer)) {
                        moves[count++] = move;
                        if (count >= MAX_MOVES - 1) {
                            moves[count].bidak = '\0'; // Mark end of moves
                            return moves;
                        }
                    }
                }
            }
        }
    }

    moves[count].bidak = '\0'; // Mark end of moves
    return moves;
}

