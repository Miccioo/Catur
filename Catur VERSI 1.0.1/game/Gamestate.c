#include "Gamestate.h"

void initGameState(GameState* state, Player* putih, Player* hitam) {
    if (state == NULL || putih == NULL || hitam == NULL) return;

    // Inisialisasi papan
    initPapan(&state->papan);
    
    // Set pemain
    state->pemainPutih = putih;
    state->pemainHitam = hitam;
    state->giliran = putih;  // Putih mulai duluan
    
    // Inisialisasi history (stack of moves)
    state->history = NULL; // Menggunakan MoveNode* sebagai head of stack
    
    // Inisialisasi status game
    state->skakPutih = 0;
    state->skakHitam = 0;
    state->skakmat = 0;
    state->remis = 0;
    state->langkahTanpaGerak = 0;
    // Inisialisasi bidakEnPassant dengan posisi invalid
    // Jika Anda ingin melacak en passant, lebih baik simpan posisi bukan pointer ke bidak
    // karena bidak itu sendiri mungkin dipindahkan atau dihapus.
    // Misalnya, tambahkan `Position enPassantTargetSquare;` ke GameState
    // Untuk saat ini, diset ke NULL sesuai definisi asli.
    state->bidakEnPassant = NULL;
}

void copyGameState(GameState* dest, GameState* src) {
    if (dest == NULL || src == NULL) return;

    // Copy papan (deep copy karena Papan adalah struct)
    memcpy(&dest->papan, &src->papan, sizeof(Papan));
    
    // Copy pointer pemain (shallow copy karena pemain dialokasikan di luar)
    dest->pemainPutih = src->pemainPutih;
    dest->pemainHitam = src->pemainHitam;
    dest->giliran = src->giliran;
    
    // Copy history (deep copy linked list/stack)
    dest->history = NULL;
    MoveNode* current_src = src->history;
    MoveNode* prev_dest = NULL;
    
    while (current_src != NULL) {
        MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
        if (newNode == NULL) {
            // Handle memory allocation failure: clean up partially copied history
            MoveNode* temp = dest->history;
            while(temp != NULL) {
                MoveNode* next_temp = temp->next;
                free(temp);
                temp = next_temp;
            }
            dest->history = NULL;
            return;
        }
        
        newNode->move = current_src->move;
        newNode->next = NULL;
        
        if (prev_dest == NULL) {
            dest->history = newNode;
        } else {
            prev_dest->next = newNode;
        }
        
        prev_dest = newNode;
        current_src = current_src->next;
    }
    
    // Copy status game
    dest->skakPutih = src->skakPutih;
    dest->skakHitam = src->skakHitam;
    dest->skakmat = src->skakmat;
    dest->remis = src->remis;
    dest->langkahTanpaGerak = src->langkahTanpaGerak;
    dest->bidakEnPassant = src->bidakEnPassant; // Ini juga harus deep copy jika bidakEnPassant menunjuk ke sesuatu yang dialokasikan dinamis
                                                 // Jika bidakEnPassant menunjuk ke bidak di papan, ini oke.
}

boolean isGameOver(GameState* state) {
    if (state == NULL) return false;
    
    if (state->skakmat) return true;
    
    if (state->remis) return true;
    
    return false;
}

void switchTurn(GameState* state) {
    if (state == NULL) return;
    
    if (state->giliran == state->pemainPutih) {
        state->giliran = state->pemainHitam;
    } else {
        state->giliran = state->pemainPutih;
    }
    
    // Reset bidak en passant (jika ada) setelah giliran
    // Ini mungkin perlu logika yang lebih canggih jika en passant benar-benar diterapkan
    state->bidakEnPassant = NULL;
}

void applyMove(GameState* state, Move* move) {
    if (state == NULL || move == NULL) return;
    
    // Ambil bidak yang dipindahkan
    Bidak pieceToMove = getBidakAt(state->papan, move->from.col, move->from.row);
    
    // Simpan bidak yang mungkin dimakan (sebelum memindahkan)
    Bidak capturedPiece = getBidakAt(state->papan, move->to.col, move->to.row);
    move->captured = (capturedPiece.tipe != TIDAK_ADA) ? capturedPiece.tipe : TIDAK_ADA; // Menggunakan TIDAK_ADA
    
    // Pindahkan bidak
    pindahkanBidak(&state->papan, move); // Ini akan mengosongkan from dan mengisi to
    
    // Implementasi khusus untuk Castling (jika move->bidak adalah Raja dan perpindahannya 2 kotak horisontal)
    if (pieceToMove.tipe == RAJA && abs(move->to.col - move->from.col) == 2) {
        // Ini adalah castling. Pindahkan juga benteng.
        int rookFromCol, rookToCol;
        if (move->to.col > move->from.col) { // Kingside Castling
            rookFromCol = 7;
            rookToCol = move->to.col - 1; // f1/f8
        } else { // Queenside Castling
            rookFromCol = 0;
            rookToCol = move->to.col + 1; // d1/d8
        }
        Move rookMove;
        createMove(&rookMove, (Position){move->from.row, rookFromCol}, (Position){move->from.row, rookToCol}, BENTENG);
        pindahkanBidak(&state->papan, &rookMove); // Pindahkan benteng
    }
    
    // Buat node baru untuk history dan push ke stack
    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
    if (newNode == NULL) {
        // Handle memory allocation failure (critical)
        fprintf(stderr, "Error: Failed to allocate memory for move history node.\n");
        return;
    }
    newNode->move = *move;
    newNode->next = state->history;
    state->history = newNode;
    
    // Update status game
    if (move->captured != TIDAK_ADA || pieceToMove.tipe == PION) { // Jika ada bidak dimakan atau pion bergerak
        state->langkahTanpaGerak = 0;
    } else {
        state->langkahTanpaGerak++;
    }
    
    // Tukar giliran
    switchTurn(state);
    // Perbarui status skakmat/skak/remis setelah move diterapkan
    // Ini harus dilakukan dengan memanggil fungsi isCheck(), isCheckmate(), isStalemate()
    // yang belum terlihat di file-file ini.
}

void undoMove(GameState* state) {
    if (state == NULL || state->history == NULL) return;
    
    // Ambil move terakhir dari stack
    MoveNode* lastMoveNode = state->history;
    Move move = lastMoveNode->move;
    
    // Pindahkan bidak kembali ke posisi asal
    Bidak piece = getBidakAt(state->papan, move.to.col, move.to.row);
    // Penting: Kembalikan hasMoved ke 0 jika ini adalah langkah pertama bidak tersebut
    // ini membutuhkan pelacakan yang lebih canggih (misal, menyimpan hasMoved di Move struct)
    // Untuk demo, kita abaikan dulu `hasMoved` di `undoMove` karena `initBidak` mengesetnya.
    
    // Kembalikan bidak ke posisi awal
    setBidakAt(&state->papan, piece, move.from.col, move.from.row);
    
    // Kosongkan posisi tujuan
    Bidak kosong;
    initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, move.to.col, move.to.row, -1);
    setBidakAt(&state->papan, kosong, move.to.col, move.to.row);
    
    // Kembalikan bidak yang dimakan (jika ada)
    if (move.captured != TIDAK_ADA) { // Menggunakan TIDAK_ADA
        Bidak captured;
        // Warna bidak yang dimakan adalah warna lawan dari pemain yang tadi giliran (sebelum undo)
        // Jadi, warna lawan dari state->giliran saat ini (setelah switchTurn)
        WarnaBidak capturedColor = (state->giliran == state->pemainPutih) ? HITAM : PUTIH;
        initBidak(&captured, move.captured, capturedColor, move.to.col, move.to.row, -1); // ID bidak yang dimakan hilang
        setBidakAt(&state->papan, captured, move.to.col, move.to.row);
    }

    // Implementasi khusus untuk Castling (undo benteng)
    if (piece.tipe == RAJA && abs(move.to.col - move.from.col) == 2) {
        // Ini adalah undo castling. Kembalikan juga benteng.
        int rookFromCol, rookToCol;
        if (move.to.col > move.from.col) { // Kingside Castling
            rookFromCol = move.to.col - 1; // f1/f8
            rookToCol = 7;                 // h1/h8
        } else { // Queenside Castling
            rookFromCol = move.to.col + 1; // d1/d8
            rookToCol = 0;                 // a1/a8
        }
        Bidak rook = getBidakAt(state->papan, rookFromCol, move.from.row);
        setBidakAt(&state->papan, rook, rookToCol, move.from.row);
        initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, rookFromCol, move.from.row, -1);
        setBidakAt(&state->papan, kosong, rookFromCol, move.from.row);
    }
    
    // Update history: pop move dari stack
    state->history = lastMoveNode->next;
    free(lastMoveNode);
    
    // Update status game
    state->langkahTanpaGerak--;
    // Tukar giliran kembali
    switchTurn(state);
}

//#include "Gamestate.h"
//
//void initGameState(GameState* state, Player* putih, Player* hitam) {
//    if (state == NULL || putih == NULL || hitam == NULL) return;
//
//    // Inisialisasi papan
//    initPapan(&state->papan);
//    
//    // Set pemain
//    state->pemainPutih = putih;
//    state->pemainHitam = hitam;
//    state->giliran = putih;  // Putih mulai duluan
//    
//    // Inisialisasi history
//    state->history = NULL;
//    
//    // Inisialisasi status game
//    state->skakPutih = 0;
//    state->skakHitam = 0;
//    state->skakmat = 0;
//    state->remis = 0;
//    state->langkahTanpaGerak = 0;
//    state->bidakEnPassant = NULL;
//}
//
//void copyGameState(GameState* dest, GameState* src) {
//    if (dest == NULL || src == NULL) return;
//
//    // Copy papan
//    memcpy(&dest->papan, &src->papan, sizeof(Papan));
//    
//    // Copy pointer pemain
//    dest->pemainPutih = src->pemainPutih;
//    dest->pemainHitam = src->pemainHitam;
//    dest->giliran = src->giliran;
//    
//    // Copy history (deep copy)
//    dest->history = NULL;
//    MoveNode* current = src->history;
//    MoveNode* prev = NULL;
//    
//    while (current != NULL) {
//        MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
//        if (newNode == NULL) return;
//        
//        newNode->move = current->move;
//        newNode->next = NULL;
//        
//        if (prev == NULL) {
//            dest->history = newNode;
//        } else {
//            prev->next = newNode;
//        }
//        
//        prev = newNode;
//        current = current->next;
//    }
//    
//    // Copy status game
//    dest->skakPutih = src->skakPutih;
//    dest->skakHitam = src->skakHitam;
//    dest->skakmat = src->skakmat;
//    dest->remis = src->remis;
//    dest->langkahTanpaGerak = src->langkahTanpaGerak;
//    dest->bidakEnPassant = src->bidakEnPassant;
//}
//
//boolean isGameOver(GameState* state) {
//    if (state == NULL) return false;
//    
//    // Cek skakmat
//    if (state->skakmat) return true;
//    
//    // Cek remis
//    if (state->remis) return true;
//    
//    return false;
//}
//
//void switchTurn(GameState* state) {
//    if (state == NULL) return;
//    
//    // Tukar giliran
//    if (state->giliran == state->pemainPutih) {
//        state->giliran = state->pemainHitam;
//    } else {
//        state->giliran = state->pemainPutih;
//    }
//    
//    // Reset bidak en passant
//    state->bidakEnPassant = NULL;
//}
//
//void applyMove(GameState* state, Move* move) {
//    if (state == NULL || move == NULL) return;
//    
//    // Buat node baru untuk history
//    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
//    if (newNode == NULL) return;
//    
//    // Copy move ke node
//    newNode->move = *move;
//    newNode->next = state->history;
//    state->history = newNode;
//    
//   
//    // Simpan bidak yang mungkin dimakan
//    Bidak captured = getBidakAt(state->papan, move->to.col, move->to.row);
//    move->captured = (captured.id != -1) ? captured.tipe : '\0';
//
//    pindahkanBidak(&state->papan, move);
//    
////    Bidak kosong;
////    initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
////    
////    // Pindahkan bidak
////    setBidakAt(&state->papan, piece, move->to.col, move->to.row);
////    setBidakAt(&state->papan, kosong, move->from.col, move->from.row);
//    
//    // Update status game
//    if (captured.id != -1) {
//        state->langkahTanpaGerak = 0;
//    } else {
//        state->langkahTanpaGerak++;
//    }
//    
//    // Tukar giliran
//    switchTurn(state);
//}
//
//void undoMove(GameState* state) {
//    if (state == NULL || state->history == NULL) return;
//    
//    // Ambil move terakhir
//    MoveNode* lastMove = state->history;
//    Move move = lastMove->move;
//    
//    // Ambil bidak yang dipindahkan
//    Bidak piece = getBidakAt(state->papan, move.to.col, move.to.row);
//    if (piece.id == -1) return;
//    
//    // Kembalikan bidak ke posisi awal
//    setBidakAt(&state->papan, piece, move.from.col, move.from.row);
//    
//    // Kosongkan posisi tujuan
//    Bidak kosong;
//    initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
//    setBidakAt(&state->papan, kosong, move.to.col, move.to.row);
//    
//    // Kembalikan bidak yang dimakan (jika ada)
//    if (move.captured != '\0') {
//        Bidak captured;
//        initBidak(&captured, move.captured, (state->giliran == state->pemainPutih) ? HITAM : PUTIH, move.to.col, move.to.row, -1);
//        setBidakAt(&state->papan, captured, move.to.col, move.to.row);
//    }
//    
//    // Update history
//    state->history = lastMove->next;
//    free(lastMove);
//    
//    // Update status game
//    state->langkahTanpaGerak--;
//    
//    // Tukar giliran kembali
//    switchTurn(state);
//}
