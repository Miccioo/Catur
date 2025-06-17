#include "Gamestate.h"

void initGameState(GameState* state, Player* putih, Player* hitam) {
    if (state == NULL || putih == NULL || hitam == NULL) return;

    // Inisialisasi papan
    initPapan(&state->papan);
    
    // Set pemain
    state->pemainPutih = putih;
    state->pemainHitam = hitam;
    state->giliran = putih;  // Putih mulai duluan
    
    // Inisialisasi history
    state->history = NULL;
    
    // Inisialisasi status game
    state->skakPutih = 0;
    state->skakHitam = 0;
    state->skakmat = 0;
    state->remis = 0;
    state->langkahTanpaGerak = 0;
    state->bidakEnPassant = NULL;
}

void copyGameState(GameState* dest, GameState* src) {
    if (dest == NULL || src == NULL) return;

    // Copy papan
    memcpy(&dest->papan, &src->papan, sizeof(Papan));
    
    // Copy pointer pemain
    dest->pemainPutih = src->pemainPutih;
    dest->pemainHitam = src->pemainHitam;
    dest->giliran = src->giliran;
    
    // Copy history (deep copy)
    dest->history = NULL;
    MoveNode* current = src->history;
    MoveNode* prev = NULL;
    
    while (current != NULL) {
        MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
        if (newNode == NULL) return;
        
        newNode->move = current->move;
        newNode->next = NULL;
        
        if (prev == NULL) {
            dest->history = newNode;
        } else {
            prev->next = newNode;
        }
        
        prev = newNode;
        current = current->next;
    }
    
    // Copy status game
    dest->skakPutih = src->skakPutih;
    dest->skakHitam = src->skakHitam;
    dest->skakmat = src->skakmat;
    dest->remis = src->remis;
    dest->langkahTanpaGerak = src->langkahTanpaGerak;
    dest->bidakEnPassant = src->bidakEnPassant;
}

boolean isGameOver(GameState* state) {
    if (state == NULL) return false;
    
    // Cek skakmat
    if (state->skakmat) return true;
    
    // Cek remis
    if (state->remis) return true;
    
    return false;
}

void switchTurn(GameState* state) {
    if (state == NULL) return;
    
    // Tukar giliran
    if (state->giliran == state->pemainPutih) {
        state->giliran = state->pemainHitam;
    } else {
        state->giliran = state->pemainPutih;
    }
    
    // Reset bidak en passant
    state->bidakEnPassant = NULL;
}

void applyMove(GameState* state, Move* move) {
    if (state == NULL || move == NULL) return;
    
    // Buat node baru untuk history
    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
    if (newNode == NULL) return;
    
    // Copy move ke node
    newNode->move = *move;
    newNode->next = state->history;
    state->history = newNode;
    
    pindahkanBidak(&state->papan, move);
   
    // Simpan bidak yang mungkin dimakan
    Bidak captured = getBidakAt(state->papan, move->to.col, move->to.row);
    move->captured = (captured.id != -1) ? captured.tipe : '\0';

//    Bidak kosong;
//    initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
//    
//    // Pindahkan bidak
//    setBidakAt(&state->papan, piece, move->to.col, move->to.row);
//    setBidakAt(&state->papan, kosong, move->from.col, move->from.row);
    
    // Update status game
    if (captured.id != -1) {
        state->langkahTanpaGerak = 0;
    } else {
        state->langkahTanpaGerak++;
    }
    
    // Tukar giliran
    switchTurn(state);
}

void undoMove(GameState* state) {
    if (state == NULL || state->history == NULL) return;
    
    // Ambil move terakhir
    MoveNode* lastMove = state->history;
    Move move = lastMove->move;
    
    // Ambil bidak yang dipindahkan
    Bidak piece = getBidakAt(state->papan, move.to.col, move.to.row);
    if (piece.id == -1) return;
    
    // Kembalikan bidak ke posisi awal
    setBidakAt(&state->papan, piece, move.from.col, move.from.row);
    
    // Kosongkan posisi tujuan
    Bidak kosong;
    initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
    setBidakAt(&state->papan, kosong, move.to.col, move.to.row);
    
    // Kembalikan bidak yang dimakan (jika ada)
    if (move.captured != '\0') {
        Bidak captured;
        initBidak(&captured, move.captured, (state->giliran == state->pemainPutih) ? HITAM : PUTIH, move.to.col, move.to.row, -1);
        setBidakAt(&state->papan, captured, move.to.col, move.to.row);
    }
    
    // Update history
    state->history = lastMove->next;
    free(lastMove);
    
    // Update status game
    state->langkahTanpaGerak--;
    
    // Tukar giliran kembali
    switchTurn(state);
}
