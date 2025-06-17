#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <stdint.h>
#include "../game/Gamestate.h"

#define TT_SIZE 1048576    // 1 juta entry (ukuran hash table)
uint64_t zobristKeys[12][64];  // 12 jenis bidak × 64 kotak

typedef struct {
    uint64_t key;          // Zobrist key posisi
    int depth;             // Kedalaman pencarian
    int eval;              // Nilai evaluasi
    Move bestMove;         // Langkah terbaik untuk posisi ini
    int flag;              // Jenis node: EXACT, LOWERBOUND, UPPERBOUND
} TTEntry;


typedef struct {
    TTEntry* entries;      // Array dinamis
    int size;              // Ukuran tabel
} TranspositionTable;

// Jenis flag
enum { EXACT, LOWERBOUND, UPPERBOUND };

#endif