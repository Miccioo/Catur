#include "transposition.h"

void initTT(TranspositionTable* tt, int size) {
    tt->size = size;
    tt->entries = (TTEntry*)calloc(size, sizeof(TTEntry));
}

void clearTT(TranspositionTable* tt) {
    memset(tt->entries, 0, tt->size * sizeof(TTEntry));
}

void freeTT(TranspositionTable* tt) {
    free(tt->entries);
}

void initZobrist() {
    for (int i = 0; i < 12; i++)
        for (int j = 0; j < 64; j++)
            zobristKeys[i][j] = random64(); // Fungsi pembangkit random 64-bit
}

uint64_t computeKey(const GameState* state) {
    uint64_t key = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Bidak piece = state->papan.grid[y][x];
            if (piece.tipe != TIDAK_ADA) {
                int pieceType = piece.tipe + 6 * piece.warna; // Indeks 0-11
                key ^= zobristKeys[pieceType][y * 8 + x];
            }
        }
    }
    return key;
}

TTEntry* probeTT(TranspositionTable* tt, uint64_t key) {
    int index = key % tt->size;
    return &tt->entries[index];
}

void storeTT(TranspositionTable* tt, uint64_t key, int depth, int eval, Move bestMove, int flag) {
    int index = key % tt->size;
    tt->entries[index] = (TTEntry){key, depth, eval, bestMove, flag};
}