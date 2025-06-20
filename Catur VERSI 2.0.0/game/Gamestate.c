// Gamestate.c
#include "Gamestate.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include "..\core\validator.h" 

// Helper Bidak Kosong untuk nilai default
// Ini adalah solusi sementara. Idealnya, Anda akan memiliki fungsi inisialisasi bidak kosong
// atau menggunakan TIDAK_ADA dari enum TipeBidak.
// Menggunakan sebuah variabel Bidak kosong yang sudah diinisialisasi
static const Bidak TIDAK_ADA_BIDAK_VALUE = {TIDAK_ADA, TANPA_WARNA, -1, -1, 0, -1};

void initGameState(GameState* state, Player* putih, Player* hitam) {
    if (state == NULL || putih == NULL || hitam == NULL) return;

    initPapan(&state->papan);
    
    state->pemainPutih = putih;
    state->pemainHitam = hitam;
    state->giliran = putih;  
    
    state->history = NULL;
    
    state->isWhiteKingInCheck = false; 
    state->isBlackKingInCheck = false; 
    state->skakmat = 0;
    state->remis = 0;
    state->langkahTanpaGerak = 0;
    state->enPassantTargetPawn = NULL; 
}

void copyGameState(GameState* dest, GameState* src) {
    if (dest == NULL || src == NULL) return;

    // Deep copy of the board
    memcpy(&dest->papan, &src->papan, sizeof(Papan));
    
    // Shallow copy of player pointers
    dest->pemainPutih = src->pemainPutih;
    dest->pemainHitam = src->pemainHitam;
    
    // Ensure giliran points to the correct player in the *destination* state's context
    if (src->giliran == src->pemainPutih) {
        dest->giliran = dest->pemainPutih;
    } else {
        dest->giliran = dest->pemainHitam;
    }
    
    // History: For deep copies in simulation (like in AI), usually you don't need a full history stack.
    // We'll clear history in the copied state for simulation purposes.
    dest->history = NULL; 
    
    dest->isWhiteKingInCheck = src->isWhiteKingInCheck; 
    dest->isBlackKingInCheck = src->isBlackKingInCheck; 
    dest->skakmat = src->skakmat;
    dest->remis = src->remis;
    dest->langkahTanpaGerak = src->langkahTanpaGerak;
    
    // En Passant Target Pawn: If the source points to a pawn on its board,
    // the destination should point to the *corresponding* pawn on its own board.
    // For simplicity in copy, we'll NULL it out and let simulation logic re-evaluate.
    dest->enPassantTargetPawn = NULL; 
}

boolean isGameOver(GameState* state) {
    if (state == NULL) return false;
    
    if (state->skakmat) return true;
    
    if (state->remis) return true;
    if (state->langkahTanpaGerak >= 100) { 
        state->remis = 1;
        return true;
    }

    // Pass `state` as the `currentState_ptr` parameter
    Position kingPos = findKingPosition(&state->papan, state->giliran->warna);
    if (!isKingInCheck(state->papan, state->giliran->warna, kingPos)) {
        Move* legalMoves = generateAllValidMoves(state->papan, state->giliran, state, state->enPassantTargetPawn);
        boolean hasNoLegalMoves = (legalMoves == NULL || legalMoves[0].bidak == TIDAK_ADA);
        if (legalMoves != NULL) free(legalMoves);
        if (hasNoLegalMoves) {
            state->remis = 1; 
            return true;
        }
    }
    
    return false;
}

void switchTurn(GameState* state) {
    if (state == NULL) return;
    
    if (state->giliran == state->pemainPutih) {
        state->giliran = state->pemainHitam;
    } else {
        state->giliran = state->pemainPutih;
    }
    
    // The en passant target is reset at the start of the new player's turn.
    // This is because en passant can only be captured immediately after the double-pawn move.
    state->enPassantTargetPawn = NULL; 
}

void applyMove(GameState* state, Move* move) {
    if (state == NULL || move == NULL) return;
    
    // Store captured piece type for undo
    Bidak capturedBeforeMove = TIDAK_ADA_BIDAK_VALUE; // Use the static const Bidak value
    if (move->isEnPassant) {
        capturedBeforeMove = getBidakAt(state->papan, move->to.col, move->from.row);
    } else {
        capturedBeforeMove = getBidakAt(state->papan, move->to.col, move->to.row);
    }
    move->captured = (capturedBeforeMove.id != -1) ? capturedBeforeMove.tipe : TIDAK_ADA; 

    // Perform the main piece movement (and rook for castling, if applicable)
    pindahkanBidak(&state->papan, move); // FIXED TYPO HERE

    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
    if (newNode == NULL) return;
    *newNode = (MoveNode){ .move = *move, .next = state->history }; 
    state->history = newNode;
    
    Bidak movedPieceActual = getBidakAt(state->papan, move->to.col, move->to.row); 
    if (movedPieceActual.tipe == PION && abs(move->from.row - move->to.row) == 2) {
        state->enPassantTargetPawn = &(state->papan.grid[move->to.row][move->to.col]);
    } else {
        state->enPassantTargetPawn = NULL; 
    }

    if (move->bidak == PION || move->captured != TIDAK_ADA) { 
        state->langkahTanpaGerak = 0;
    } else {
        state->langkahTanpaGerak++;
    }
    
    switchTurn(state);

    updateGameStatus(state);
}

void undoMove(GameState* state) {
    if (state == NULL || state->history == NULL) return;
    
    MoveNode* lastMoveNode = state->history;
    Move move = lastMoveNode->move;
    
    switchTurn(state); 

    Bidak pieceToUndo = getBidakAt(state->papan, move.to.col, move.to.row);
    if (pieceToUndo.id == -1) { 
        fprintf(stderr, "Error: Piece not found at target position for undo.\n");
        return; 
    }

    if (move.promotionTo != TIDAK_ADA) {
        pieceToUndo.tipe = PION; 
    }

    setBidakAt(&state->papan, pieceToUndo, move.from.col, move.from.row);
    
    Bidak emptySquareTo;
    initBidak(&emptySquareTo, TIDAK_ADA, TANPA_WARNA, move.to.col, move.to.row, -1);
    setBidakAt(&state->papan, emptySquareTo, move.to.col, move.to.row);
    
    if (move.captured != TIDAK_ADA) {
        Bidak capturedPiece;
        WarnaBidak capturedColor = (state->giliran->warna == PUTIH) ? HITAM : PUTIH; 

        if (move.isEnPassant) {
            initBidak(&capturedPiece, move.captured, capturedColor, move.to.col, move.from.row, -1); 
            setBidakAt(&state->papan, capturedPiece, move.to.col, move.from.row);
        } else {
            initBidak(&capturedPiece, move.captured, capturedColor, move.to.col, move.to.row, -1); 
            setBidakAt(&state->papan, capturedPiece, move.to.col, move.to.row);
        }
    }

    if (move.isCastling) {
        Bidak rookToUndo;
        Position rookOriginalPos, rookMovedPos;

        if (move.to.col == move.from.col + 2) { 
            rookToUndo = getBidakAt(state->papan, move.to.col - 1, move.to.row); 
            rookOriginalPos = (Position){move.from.row, 7}; 
            rookMovedPos = (Position){move.to.row, move.to.col - 1}; 
        } else { 
            rookToUndo = getBidakAt(state->papan, move.to.col + 1, move.to.row); 
            rookOriginalPos = (Position){move.from.row, 0}; 
            rookMovedPos = (Position){move.to.row, move.to.col + 1}; 
        }
        
        setBidakAt(&state->papan, rookToUndo, rookOriginalPos.col, rookOriginalPos.row);
        Bidak emptyRookMovedSquare;
        initBidak(&emptyRookMovedSquare, TIDAK_ADA, TANPA_WARNA, rookMovedPos.col, rookMovedPos.row, -1);
        setBidakAt(&state->papan, emptyRookMovedSquare, rookMovedPos.col, rookMovedPos.row);
        
        state->papan.grid[move.from.row][move.from.col].hasMoved = 0; 
        state->papan.grid[rookOriginalPos.row][rookOriginalPos.col].hasMoved = 0;   
    }
    
    state->enPassantTargetPawn = NULL; 

    state->history = lastMoveNode->next;
    free(lastMoveNode);
    
    state->langkahTanpaGerak--; 

    updateGameStatus(state);
}

void updateGameStatus(GameState* state) {
    if (state == NULL) return;

    Position currentKingPos = findKingPosition(&state->papan, state->giliran->warna);

    if (isKingInCheck(state->papan, state->giliran->warna, currentKingPos)) {
        if (state->giliran->warna == PUTIH) state->isWhiteKingInCheck = true;
        else state->isBlackKingInCheck = true;
        
        if (isCheckmate(state)) {
            state->skakmat = 1;
        }
    } else {
        if (state->giliran->warna == PUTIH) state->isWhiteKingInCheck = false;
        else state->isBlackKingInCheck = false;
        
        if (isCheckmate(state)) { 
            state->remis = 1;
        }
    }
}
