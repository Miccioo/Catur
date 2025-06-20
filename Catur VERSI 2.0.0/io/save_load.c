#include "save_load.h"
#include <stdio.h>
#include <string.h>

// Helper: simpan papan
static void saveBoard(FILE* f, const Papan* papan) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            const Bidak* b = &papan->grid[y][x];
            fprintf(f, "%d %d %d %d %d %d ", b->tipe, b->warna, b->x, b->y, b->hasMoved, b->id);
        }
        fprintf(f, "\n");
    }
}

// Helper: load papan
static void loadBoard(FILE* f, Papan* papan) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int tipe, warna, bx, by, hasMoved, id;
            fscanf(f, "%d %d %d %d %d %d ", &tipe, &warna, &bx, &by, &hasMoved, &id);
            papan->grid[y][x].tipe = tipe;
            papan->grid[y][x].warna = warna;
            papan->grid[y][x].x = bx;
            papan->grid[y][x].y = by;
            papan->grid[y][x].hasMoved = hasMoved;
            papan->grid[y][x].id = id;
        }
    }
}

// Helper: simpan player
static void savePlayer(FILE* f, const Player* p) {
    fprintf(f, "%s\n%d %d %d\n", p->nama, p->warna, p->giliran, p->skor);
}

// Helper: load player
static void loadPlayer(FILE* f, Player* p) {
    fgets(p->nama, MAX_NAMA, f);
    size_t len = strlen(p->nama);
    if (len > 0 && p->nama[len-1] == '\n') p->nama[len-1] = '\0';
    fscanf(f, "%d %d %d\n", (int*)&p->warna, &p->giliran, &p->skor);
}

int saveGame(const char* filename, const GameState* state) {
    FILE* f = fopen(filename, "w");
    if (!f) return 0;
    saveBoard(f, &state->papan);
    savePlayer(f, state->pemainPutih);
    savePlayer(f, state->pemainHitam);
    // Giliran: 0=putih, 1=hitam
    int giliran = (state->giliran == state->pemainPutih) ? 0 : 1;
    fprintf(f, "%d\n", giliran);
    fprintf(f, "%d %d %d\n", state->isWhiteKingInCheck, state->isBlackKingInCheck, state->skakmat);
    fprintf(f, "%d %d %d\n", state->remis, state->langkahTanpaGerak, -1); // -1: abaikan enPassantTargetPawn
    fclose(f);
    return 1;
}

int loadGame(const char* filename, GameState* state) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;
    loadBoard(f, &state->papan);
    static Player putih, hitam;
    loadPlayer(f, &putih);
    loadPlayer(f, &hitam);
    state->pemainPutih = &putih;
    state->pemainHitam = &hitam;
    int giliran;
    fscanf(f, "%d\n", &giliran);
    state->giliran = (giliran == 0) ? state->pemainPutih : state->pemainHitam;
    fscanf(f, "%d %d %d\n", (int*)&state->isWhiteKingInCheck, (int*)&state->isBlackKingInCheck, &state->skakmat);
    fscanf(f, "%d %d %*d\n", &state->remis, &state->langkahTanpaGerak);
    state->history = NULL;
    state->enPassantTargetPawn = NULL;
    fclose(f);
    return 1;
}
