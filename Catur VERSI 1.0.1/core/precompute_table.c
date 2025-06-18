#include "precompute_table.h"

// Deklarasi Global
// Langkah - langkah legal yang dapat dijalankan oleh masing-masing bidak
const int kingMoves[10][2]  = { {1,0}, {-1,0}, { 0,1}, { 0,-1}, { 1, 1}, { 1,-1}, {-1,1}, {-1,-1}, {-2,0}, {2,0}};
const int queenMoves[8][2]  = { {1,0}, {-1,0}, { 0,1}, { 0,-1}, { 1, 1}, { 1,-1}, {-1,1}, {-1,-1} };
const int rookMoves[4][2]   = { {1,0}, {-1,0}, { 0,1}, { 0,-1} };
const int knightMoves[8][2] = { {2,1}, {1, 2}, {-1,2}, {-2, 1}, {-2,-1}, {-1,-2}, {1,-2}, { 2,-1} };
const int bishopMoves[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

void generatePawnMoves(Position pos, MoveList* list, Move* moves, Player* player, Papan* papan) {
    int langkah = (player->warna == PUTIH) ? 1 : -1;

    // Normal move
    Position target = {pos.col, pos.row + langkah};
    if (isEmptyBidak(papan, target.col, target.row)) {
        createMove(moves, pos, target, PION);

        // Langkah 2 kotak dari posisi awal
        if (pos.row == 1 || pos.row == 6) {
            Position doubleTarget = {pos.col, pos.row + 2*langkah};
            if (isEmptyBidak(papan, doubleTarget.col, doubleTarget.row)) {
            	createMove(moves, pos, target, PION);
                addMove(list, *(moves));
            }
        }
    
        // jika ada bidak yang bisa dimakan
        for (int i = -1; i <= 1; i += 2) {
            target.col = pos.col + i;
            target.row = pos.row + langkah;
            if (isEmptyBidak(papan, target.col, target.row)) {
                createMove(moves, pos, target, PION);
                addMove(list, *(moves));
            }
        }
    }
}

void generateBishopMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan) {
	
    for (int i = 0; i < 4; i++) {  // untuk 4 langkah legal bishop, (diagonal kiri kanan atas bawah)
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + bishopMoves[i][0] * step, 
                            pos.row + bishopMoves[i][1] * step};
            if (!isValidSquare(target))  continue;

            if (isEmptyBidak(papan, target.col, target.row)) {
                createMove(moves, pos, target, GAJAH);
                addMove(list, *(moves));
            } else {
                // Jika ada bidak yang bisa dimakan
                Bidak targetPiece = getBidakAt(*(papan), target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, pos, target, GAJAH);
                    addMove(list, *(moves));
                }
                break;
            }
        }
    }
}

void generateKnightMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan) {
		
    for (int i = 0; i < 8; i++) {
        Position target = {pos.col + knightMoves[i][0], 
                        pos.row + knightMoves[i][1]};
        if (!isValidSquare(target)) continue;
        if (isEmptyBidak(papan, target.col, target.row)) {
            createMove(moves, pos, target, KUDA);
            addMove(list, *(moves));
        } else {
            // Jika ada bidak yang bisa dimakan
            Bidak targetPiece = getBidakAt(*(papan), target.col, target.row);
            if (targetPiece.warna != player->warna) {
                createMove(moves, pos, target, KUDA);
                addMove(list, *(moves));
            }
        }

    }
}

void generateRookMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan) {
		
    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + rookMoves[i][0] * step, 
                            pos.row + rookMoves[i][1] * step};
            if (isValidSquare(target)) continue;
            if (isEmptyBidak(papan, target.col, target.row)) {
                createMove(moves, pos, target, BENTENG);
				addMove(list, *(moves));   
            } else {

                Bidak targetPiece = getBidakAt(*(papan), target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, pos, target, BENTENG);
                    addMove(list, *(moves));
                }
                break;
            }
        }
    }
}

void generateQueenMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan) {
		
    // Generate rook-like moves
    for (int i = 0; i < 4; i++) {
        for (int step = 1; step < 8; step++) {
            Position target = {pos.col + queenMoves[i][0] * step, 
                            pos.row + queenMoves[i][1] * step};
            if (isValidSquare(target)) continue;
            if (isEmptyBidak(papan, target.col, target.row)) {
                createMove(moves, pos, target, MENTERI);
                addMove(list, *(moves));
                
            } else {
                // If there's a piece, we can capture it
                Bidak targetPiece = getBidakAt(*(papan), target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, pos, target, MENTERI);
                    addMove(list, *(moves));
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
            if (isValidSquare(target)) continue;
            if (isEmptyBidak(papan, target.col, target.row)) {
                createMove(moves, pos, target, MENTERI);
                addMove(list, *(moves));
            } else {
                // If there's a piece, we can capture it
                Bidak targetPiece = getBidakAt(*(papan), target.col, target.row);
                if (targetPiece.warna != player->warna) {
                    createMove(moves, pos, target, MENTERI);
                    addMove(list, *(moves));
                }
                break; // Stop if we hit a piece
            }
        }
    }
}

void generateKingMoves(Position pos, MoveList *list, Move* moves, Player* player, Papan* papan) {
	
    for (int i = 0; i < 8; i++) {
        Position targetRaja = {pos.col + kingMoves[i][0], pos.row + kingMoves[i][1]};
        if (!isValidSquare(targetRaja)) continue; 
		if (isEmptyBidak(papan, pos.row, pos.col)){
            createMove(moves, pos, targetRaja, RAJA);
            addMove(list, *(moves));
        }
        
        Bidak raja = getBidakAt(*(papan), pos.row, pos.col);

        if (raja.hasMoved == 0) {
        	Bidak benteng1 = getBidakAt(*(papan), pos.col, pos.row - 4); // Benteng sebelah kiri
        	Bidak benteng2 = getBidakAt(*(papan), pos.col, pos.row + 3); // Benteng sebelah kanan
        	
        	if (benteng1.tipe == BENTENG && benteng1.hasMoved == 0) {
	        	targetRaja.col = pos.col + kingMoves[9][0];
	        	targetRaja.row = pos.row + kingMoves[9][1];
	        	
	        	Position targetBenteng = {pos.col + 3, pos.row};
	        	
	        	createMove(moves, pos, targetRaja, RAJA);
	        	createMove(moves, pos, targetBenteng, BENTENG);
	        	addMove(list, *(moves));
			}
			
			if (benteng2.tipe == BENTENG && benteng2.hasMoved == 0) {
	        	targetRaja.col = pos.col + kingMoves[10][0];
	        	targetRaja.row = pos.row + kingMoves[10][1];
	        	
	        	Position targetBenteng = {pos.col + 1, pos.row};
	        	
	        	createMove(moves, pos, targetRaja, RAJA);
	        	createMove(moves, pos, targetBenteng, BENTENG);
	        	addMove(list, *(moves));
			}

		}
    }
}
