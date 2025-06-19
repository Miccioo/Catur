#include "validator.h" // Include validator.h FIRST
#include "..\game\Gamestate.h" // Then include Gamestate.h for the FULL definition of GameState

#include <stdlib.h>
#include <math.h>
#include <stdio.h> 
#include "..\game\bidak.h" 

// No need to redefine MAX_MOVES here, it's in validator.h

boolean isPathClear(Papan papan, Position from, Position to) {
    int dx = 0;
    if (to.col > from.col) dx = 1;
    else if (to.col < from.col) dx = -1;

    int dy = 0;
    if (to.row > from.row) dy = 1;
    else if (to.row < from.row) dy = -1;
    
    Position current = {from.row + dy, from.col + dx};
    
    while (current.row != to.row || current.col != to.col) {
        Bidak bidakDiJalur = getBidakAt(papan, current.col, current.row);
        if (bidakDiJalur.id != -1) {
            return false;
        }
        current.col += dx; 
        current.row += dy;
    }
    return true;
}

boolean isPseudoLegalMove(Papan papan, Move* move, Player* currentPlayer, Bidak* enPassantTargetPawn) {
    Bidak piece = getBidakAt(papan, move->from.col, move->from.row);

    if (piece.id == -1 || piece.warna != currentPlayer->warna) {
        return false;
    }

    int dx = move->to.col - move->from.col;
    int dy = move->to.row - move->from.row;
    
    Bidak target = getBidakAt(papan, move->to.col, move->to.row);
    boolean isTargetEmpty = (target.id == -1);
    boolean isTargetOpponentPiece = (target.id != -1 && target.warna != piece.warna);
    boolean isTargetOwnPiece = (target.id != -1 && target.warna == piece.warna);

    if (isTargetOwnPiece) {
        return false;
    }

    move->isEnPassant = false;
    move->isCastling = false;
    move->promotionTo = TIDAK_ADA;


    switch (piece.tipe) {
        case PION: {
            int direction = (piece.warna == PUTIH) ? -1 : 1; 
            int startRow = (piece.warna == PUTIH) ? 6 : 1; 

            if (dx == 0 && dy == direction && isTargetEmpty) {
                if ((piece.warna == PUTIH && move->to.row == 0) || (piece.warna == HITAM && move->to.row == 7)) {
                    move->promotionTo = MENTERI; 
                }
                return true;
            }
            if (dx == 0 && dy == 2 * direction && move->from.row == startRow &&
                getBidakAt(papan, move->from.col, move->from.row + direction).id == -1 && 
                isTargetEmpty) {
                return true;
            }
            if (abs(dx) == 1 && dy == direction && isTargetOpponentPiece) {
                return true;
            }
            if (abs(dx) == 1 && dy == direction && isTargetEmpty && enPassantTargetPawn != NULL &&
                enPassantTargetPawn->x == move->to.col && enPassantTargetPawn->y == move->from.row &&
                enPassantTargetPawn->tipe == PION && enPassantTargetPawn->warna != piece.warna) { 
                move->isEnPassant = true;
                return true;
            }
            break; 
        }
        case BENTENG:
            return (dx == 0 || dy == 0) && isPathClear(papan, move->from, move->to);
        case MENTERI:
            return (abs(dx) == abs(dy) || dx == 0 || dy == 0) && isPathClear(papan, move->from, move->to);
        case GAJAH:
            return abs(dx) == abs(dy) && isPathClear(papan, move->from, move->to);
        case KUDA:
            return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2);
        case RAJA: { 
            if (abs(dx) <= 1 && abs(dy) <= 1) {
                return true;
            }
            if (!piece.hasMoved && dy == 0 && abs(dx) == 2) {
                Bidak rook;
                int rookCol; 
                
                if (dx == 2 && piece.warna == PUTIH && move->from.col == 4 && move->from.row == 7) { 
                    rook = getBidakAt(papan, 7, 7); 
                    rookCol = 7;
                } else if (dx == -2 && piece.warna == PUTIH && move->from.col == 4 && move->from.row == 7) { 
                    rook = getBidakAt(papan, 0, 7); 
                    rookCol = 0;
                } else if (dx == 2 && piece.warna == HITAM && move->from.col == 4 && move->from.row == 0) { 
                    rook = getBidakAt(papan, 7, 0); 
                    rookCol = 7;
                } else if (dx == -2 && piece.warna == HITAM && move->from.col == 4 && move->from.row == 0) { 
                    rook = getBidakAt(papan, 0, 0); 
                    rookCol = 0;
                } else {
                    return false; 
                }

                if (rook.id != -1 && rook.tipe == BENTENG && rook.warna == piece.warna && !rook.hasMoved) {
                    Position intermediate1, intermediate2;
                    if (dx == 2) { // Kingside
                        intermediate1 = (Position){move->from.row, move->from.col + 1}; 
                        intermediate2 = (Position){move->from.row, move->from.col + 2}; 
                        if (!isEmptyBidak(&papan, intermediate1.col, intermediate1.row) || !isEmptyBidak(&papan, intermediate2.col, intermediate2.row)) {
                            return false; 
                        }
                    } else { // Queenside
                        intermediate1 = (Position){move->from.row, move->from.col - 1}; 
                        intermediate2 = (Position){move->from.row, move->from.col - 2}; 
                        Position intermediate3 = (Position){move->from.row, move->from.col - 3}; 
                        if (!isEmptyBidak(&papan, intermediate1.col, intermediate1.row) || !isEmptyBidak(&papan, intermediate2.col, intermediate2.row) || !isEmptyBidak(&papan, intermediate3.col, intermediate3.row)) {
                            return false; 
                        }
                    }
                    move->isCastling = true;
                    return true;
                }
            }
            break;
        } 
        
        // --- LOGIKA PERGERAKAN UNTUK BIDAK EVOLVE ---
        case PION_BERAT: { // Pion + Pion
            int direction = (piece.warna == PUTIH) ? -1 : 1;
            // Bergerak 1 langkah maju (pion normal)
            if (dx == 0 && dy == direction && isTargetEmpty) {
                return true;
            }
            // Menyerang diagonal (pion normal)
            if (abs(dx) == 1 && dy == direction && isTargetOpponentPiece) {
                return true;
            }
            // Pion Berat bisa bergerak 1 kotak ke samping (ortogonal)
            if ((abs(dx) == 1 && dy == 0) && (isTargetEmpty || isTargetOpponentPiece)) {
                return true;
            }
            break;
        }

        case KSATRIA_PIONIR: // Pion + Kuda
        case KUDA_BERPELINDUNG: { // Kuda + Pion
            int direction = (piece.warna == PUTIH) ? -1 : 1;
            // Gerakan Kuda
            if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
                return true;
            }
            // Gerakan Pion (maju 1 kotak)
            if (dx == 0 && dy == direction && isTargetEmpty) {
                return true;
            }
            // Gerakan Pion (serang diagonal)
            if (abs(dx) == 1 && dy == direction && isTargetOpponentPiece) {
                return true;
            }
            break;
        }

        case GAJAH_PENJAGA: // Pion + Gajah
        case GAJAH_BIJAKSANA: { // Gajah + Pion
            int direction = (piece.warna == PUTIH) ? -1 : 1;
            // Gerakan Gajah
            if (abs(dx) == abs(dy) && isPathClear(papan, move->from, move->to)) {
                return true;
            }
            // Gerakan Pion (maju 1 kotak)
            if (dx == 0 && dy == direction && isTargetEmpty) {
                return true;
            }
            // Gerakan Pion (serang diagonal)
            if (abs(dx) == 1 && dy == direction && isTargetOpponentPiece) {
                return true;
            }
            break;
        }

        case BENTENG_PENYERBU: // Pion + Benteng
        case BENTENG_PIONIR: { // Benteng + Pion
            int direction = (piece.warna == PUTIH) ? -1 : 1;
            // Gerakan Benteng
            if ((dx == 0 || dy == 0) && isPathClear(papan, move->from, move->to)) {
                return true;
            }
            // Gerakan Pion (maju 1 kotak)
            if (dx == 0 && dy == direction && isTargetEmpty) {
                return true;
            }
            // Gerakan Pion (serang diagonal)
            if (abs(dx) == 1 && dy == direction && isTargetOpponentPiece) {
                return true;
            }
            break;
        }
        
        case KSATRIA_GANDA: { // Kuda + Kuda
            // Gerakan Kuda
            if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
                return true;
            }
            // Tambahan: Gerakan 1 kotak lurus ke segala arah (ortogonal)
            if (((abs(dx) == 1 && dy == 0) || (dx == 0 && abs(dy) == 1)) && (isTargetEmpty || isTargetOpponentPiece)) {
                 return true;
            }
            break;
        }

        case KOMANDAN_MEDAN: // Kuda + Gajah
        case PENGUASA_GARIS_DEPAN: { // Gajah + Kuda
            // Gerakan Kuda
            if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
                return true;
            }
            // Gerakan Gajah
            if (abs(dx) == abs(dy) && isPathClear(papan, move->from, move->to)) {
                return true;
            }
            break;
        }

        case GAJAH_AGUNG: { // Gajah + Gajah
            // Gerakan Gajah
            if (abs(dx) == abs(dy) && isPathClear(papan, move->from, move->to)) {
                return true;
            }
            // Tambahan: Gerakan 1 kotak lurus ke segala arah (ortogonal)
            if (((abs(dx) == 1 && dy == 0) || (dx == 0 && abs(dy) == 1)) && (isTargetEmpty || isTargetOpponentPiece)) {
                 return true;
            }
            break;
        }

        // --- LOGIKA PERGERAKAN UNTUK BIDAK ZOMBIE --- (Belum ada, akan ditambah di masa depan)
        case ZOMBIE_PION:
        case ZOMBIE_BENTENG:
        case ZOMBIE_KUDA:
        case ZOMBIE_GAJAH:
        case ZOMBIE_MENTERI:
        case ZOMBIE_RAJA:
            // Untuk saat ini, bidak zombie tidak bisa bergerak atau memiliki gerakan sangat terbatas
            return false; // Contoh: bidak zombie tidak bisa bergerak
            break;

        case TIDAK_ADA: 
            return false; 
    }
    return false; // Mengembalikan false jika tidak ada aturan yang cocok.
}


Move* generateAllPseudoLegalMoves(Papan papan, Player* currentPlayer, Bidak* enPassantTargetPawn) {
    Move* moves = (Move*)malloc(sizeof(Move) * MAX_MOVES);
    if (moves == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for pseudo-legal moves.\n");
        return NULL;
    }
    
    int count = 0;
    int row, col, toRow, toCol; 
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
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
                    
                    if (isPseudoLegalMove(papan, &currentMove, currentPlayer, enPassantTargetPawn)) {
                        moves[count++] = currentMove;
                        if (count >= MAX_MOVES - 1) { 
                            moves[count].bidak = TIDAK_ADA; // Tandai akhir array
                            return moves;
                        }
                    }
                }
            }
        }
    }

    moves[count].bidak = TIDAK_ADA; // Pastikan array selalu diakhiri dengan TIDAK_ADA
    return moves;
}

boolean isSquareAttacked(Papan papan, Position targetPos, WarnaBidak attackingColor) {
    int r, c; 
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            Bidak attackerPiece = getBidakAt(papan, c, r);

            if (attackerPiece.id != -1 && attackerPiece.warna == attackingColor) {
                Move dummyMove;
                createMove(&dummyMove, (Position){r, c}, targetPos, attackerPiece.tipe);

                int dx = dummyMove.to.col - dummyMove.from.col;
                int dy = dummyMove.to.row - dummyMove.from.row;
                
                switch (attackerPiece.tipe) {
                    case PION: {
                        int direction = (attackerPiece.warna == PUTIH) ? -1 : 1;
                        if (abs(dx) == 1 && dy == direction) {
                            return true;
                        }
                        break;
                    }
                    case BENTENG:
                        if ((dx == 0 || dy == 0) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true;
                        break;
                    case MENTERI:
                        if ((abs(dx) == abs(dy) || dx == 0 || dy == 0) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true;
                        break;
                    case GAJAH:
                        if (abs(dx) == abs(dy) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true;
                        break;
                    case KUDA:
                        if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) return true;
                        break;
                    case RAJA:
                        if (abs(dx) <= 1 && abs(dy) <= 1 && (abs(dx) > 0 || abs(dy) > 0)) return true;
                        break;
                    
                    // --- LOGIKA UNTUK BIDAK EVOLVE SEBAGAI PENYERANG ---
                    // Ini penting agar bidak evolve bisa men-skak raja
                    case PION_BERAT: {
                        int direction = (attackerPiece.warna == PUTIH) ? -1 : 1;
                        if (abs(dx) == 1 && dy == direction) return true; // Serang diagonal seperti pion
                        if ((abs(dx) == 1 && dy == 0)) return true; // Serang samping
                        break;
                    }
                    case KSATRIA_PIONIR:
                    case KUDA_BERPELINDUNG: {
                        int direction = (attackerPiece.warna == PUTIH) ? -1 : 1;
                        if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) return true; // Gerak Kuda
                        if (abs(dx) == 1 && dy == direction) return true; // Serang diagonal Pion
                        break;
                    }
                    case GAJAH_PENJAGA:
                    case GAJAH_BIJAKSANA: {
                        int direction = (attackerPiece.warna == PUTIH) ? -1 : 1;
                        if (abs(dx) == abs(dy) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true; // Gerak Gajah
                        if (abs(dx) == 1 && dy == direction) return true; // Serang diagonal Pion
                        break;
                    }
                    case BENTENG_PENYERBU:
                    case BENTENG_PIONIR: {
                        int direction = (attackerPiece.warna == PUTIH) ? -1 : 1;
                        if ((dx == 0 || dy == 0) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true; // Gerak Benteng
                        if (abs(dx) == 1 && dy == direction) return true; // Serang diagonal Pion
                        break;
                    }
                    case KSATRIA_GANDA: {
                        if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) return true; // Gerak Kuda
                        if (((abs(dx) == 1 && dy == 0) || (dx == 0 && abs(dy) == 1))) return true; // Gerak 1 kotak ortogonal
                        break;
                    }
                    case KOMANDAN_MEDAN:
                    case PENGUASA_GARIS_DEPAN: {
                        if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) return true; // Gerak Kuda
                        if (abs(dx) == abs(dy) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true; // Gerak Gajah
                        break;
                    }
                    case GAJAH_AGUNG: {
                        if (abs(dx) == abs(dy) && isPathClear(papan, dummyMove.from, dummyMove.to)) return true; // Gerak Gajah
                        if (((abs(dx) == 1 && dy == 0) || (dx == 0 && abs(dy) == 1))) return true; // Gerak 1 kotak ortogonal
                        break;
                    }
                    // --- BIDAK ZOMBIE SEBAGAI PENYERANG (jika ada aturannya) ---
                    // Saat ini mereka tidak bisa bergerak, jadi tidak akan ada di sini
                    case ZOMBIE_PION:
                    case ZOMBIE_BENTENG:
                    case ZOMBIE_KUDA:
                    case ZOMBIE_GAJAH:
                    case ZOMBIE_MENTERI:
                    case ZOMBIE_RAJA:
                        // Bidak zombie saat ini tidak punya aturan pergerakan, jadi tidak bisa menyerang
                        break;

                    case TIDAK_ADA: break; // Ini adalah case terakhir untuk switch (attackerPiece.tipe)
                } // Tutup switch (attackerPiece.tipe)
            } // Tutup if (attackerPiece.id != -1 && attackerPiece.warna == attackingColor)
        } // Tutup for (c = 0; c < 8; c++)
    } // Tutup for (r = 0; r < 8; r++)
    return false; // Kembali false jika tidak ada penyerang yang ditemukan
} // Tutup fungsi isSquareAttacked

boolean isKingInCheck(Papan papan, WarnaBidak kingColor, Position kingPos) {
    WarnaBidak opponentColor = (kingColor == PUTIH) ? HITAM : PUTIH;
    return isSquareAttacked(papan, kingPos, opponentColor);
}

// Implement isLegalMove using GameState* and casting
boolean isLegalMove(void* state_ptr, Move* move) {
    GameState* state = (GameState*)state_ptr; // Cast back to GameState*
    GameState tempState;
    copyGameState(&tempState, state); 

    Bidak pieceToMove = getBidakAt(tempState.papan, move->from.col, move->from.row);

    // Simpan tipe asli bidak untuk undo nanti jika promosi (jika diperlukan untuk undo)
    // TipeBidak originalPieceType = pieceToMove.tipe; 

    if (move->isEnPassant) {
        Bidak emptySquare; initBidak(&emptySquare, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
        tempState.papan.grid[move->from.row][move->to.col] = emptySquare; 
    } else {
        Bidak targetPiece = getBidakAt(tempState.papan, move->to.col, move->to.row);
        if (targetPiece.id != -1) {
            Bidak emptySquare; initBidak(&emptySquare, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
            tempState.papan.grid[move->to.row][move->to.col] = emptySquare;
        }
    }

    // Jika ada promosi, tipe bidak di tempState diubah
    if (move->promotionTo != TIDAK_ADA) {
        pieceToMove.tipe = move->promotionTo; 
    }

    // Pastikan koordinat bidak diperbarui sebelum ditempatkan di tempState
    pieceToMove.x = move->to.col;
    pieceToMove.y = move->to.row;

    tempState.papan.grid[move->to.row][move->to.col] = pieceToMove; 
    Bidak emptySquareFrom; initBidak(&emptySquareFrom, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
    tempState.papan.grid[move->from.row][move->from.col] = emptySquareFrom; 

    if (move->isCastling) {
        Bidak rook;
        Position rookFrom, rookTo;
        // Penentuan rookFrom dan rookTo harus konsisten dengan pindahkanBidak
        // Ambil rook dari papan ASLI (state->papan) karena tempState sudah dimodifikasi
        if (move->to.col == move->from.col + 2) { // Kingside
            rook = getBidakAt(state->papan, 7, move->from.row); 
            rookFrom = (Position){move->from.row, 7};
            rookTo = (Position){move->from.row, 5};
        } else { // Queenside
            rook = getBidakAt(state->papan, 0, move->from.row); 
            rookFrom = (Position){move->from.row, 0};
            rookTo = (Position){move->from.row, 3};
        }
        
        // Update koordinat rook untuk tempState
        rook.x = rookTo.col;
        rook.y = rookTo.row;

        tempState.papan.grid[rookTo.row][rookTo.col] = rook;
        Bidak emptyRookOriginal; initBidak(&emptyRookOriginal, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
        tempState.papan.grid[rookFrom.row][rookFrom.col] = emptyRookOriginal;
    }
    
    // Periksa apakah raja masih dalam skak setelah gerakan simulasi
    Position kingPosAfterMove = findKingPosition(tempState.papan, state->giliran->warna);

    return !isKingInCheck(tempState.papan, state->giliran->warna, kingPosAfterMove);
}


// Implement generateAllValidMoves using GameState* and casting
Move* generateAllValidMoves(Papan papan, Player* currentPlayer, void* currentState_ptr, Bidak* enPassantTargetPawn) {
    GameState* currentState = (GameState*)currentState_ptr; // Cast back to GameState*
    Move* pseudoLegalMoves = generateAllPseudoLegalMoves(papan, currentPlayer, enPassantTargetPawn);
    Move* legalMoves = (Move*)malloc(sizeof(Move) * MAX_MOVES);
    if (legalMoves == NULL) {
        if (pseudoLegalMoves != NULL) free(pseudoLegalMoves);
        fprintf(stderr, "Error: Failed to allocate memory for legal moves.\n");
        return NULL;
    }
    int legalCount = 0;

    if (pseudoLegalMoves == NULL) { 
        legalMoves[0].bidak = TIDAK_ADA; // Jika tidak ada pseudo-legal move, tandai kosong
        return legalMoves;
    }

    int i; 
    for (i = 0; pseudoLegalMoves[i].bidak != TIDAK_ADA; i++) {
        // isLegalMove akan membuat salinan GameState, jadi tidak perlu khawatir mengubah state asli.
        if (isLegalMove(currentState, &pseudoLegalMoves[i])) { 
            legalMoves[legalCount++] = pseudoLegalMoves[i];
        }
    }
    legalMoves[legalCount].bidak = TIDAK_ADA; // Pastikan array selalu diakhiri
    free(pseudoLegalMoves); // Bebaskan memori pseudo-legal moves
    return legalMoves;
}

// Implement isCheckmate using GameState* and casting
boolean isCheckmate(void* state_ptr) {
    GameState* state = (GameState*)state_ptr; // Cast back to GameState*
    Position currentKingPos = findKingPosition(state->papan, state->giliran->warna);
    
    // Pastikan raja sedang di-skak sebelum memeriksa skakmat
    if (!isKingInCheck(state->papan, state->giliran->warna, currentKingPos)) {
        return false; // Bukan skakmat jika raja tidak dalam skak
    }

    Move* allLegalMoves = generateAllValidMoves(state->papan, state->giliran, state, state->enPassantTargetPawn); 

    boolean noLegalMoves = true;
    if (allLegalMoves != NULL) {
        if (allLegalMoves[0].bidak != TIDAK_ADA) { // Periksa apakah ada langkah legal
            noLegalMoves = false; 
        }
        free(allLegalMoves); // Pastikan memori dibebaskan
    }
    
    if (noLegalMoves) {
        return true; // Jika raja di-skak dan tidak ada langkah legal, itu skakmat
    }

    return false; // Jika ada langkah legal, bukan skakmat
}
