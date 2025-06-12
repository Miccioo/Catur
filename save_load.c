#include "save_load.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMA_FILE_SAVE "save_catur.dat"
//SAVE GAME
boolean saveGameState(GameState* state, const char* filename) {
    if (state == NULL) {
        filename = NAMA_FILE_SAVE; 
    }
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return false;
    
    // Save papan
    fwrite(&state->papan, sizeof(Papan), 1, file);
    
    // Save informasi pemain
    fwrite(state->pemainPutih, sizeof(Player), 1, file);
    fwrite(state->pemainHitam, sizeof(Player), 1, file);
    
    // Save giliran saat ini (0 untuk putih, 1 untuk hitam)
    int currentTurn = (state->giliran == state->pemainPutih) ? 0 : 1;
    fwrite(&currentTurn, sizeof(int), 1, file);
    
    // Save history moves
    MoveNode* current = state->history;
    int moveCount = 0;
    while (current != NULL) {
        moveCount++;
        current = current->next;
    }
    
    fwrite(&moveCount, sizeof(int), 1, file);
    current = state->history;
    while (current != NULL) {
        fwrite(&current->move, sizeof(Move), 1, file);
        current = current->next;
    }
    
    // Save status permainan
    fwrite(&state->skakPutih, sizeof(int), 1, file);
    fwrite(&state->skakHitam, sizeof(int), 1, file);
    fwrite(&state->skakmat, sizeof(int), 1, file);
    fwrite(&state->remis, sizeof(int), 1, file);
    fwrite(&state->langkahTanpaGerak, sizeof(int), 1, file);
    
    // Save posisi bidak en passant jika ada
    int hasEnPassant = (state->bidakEnPassant != NULL) ? 1 : 0;
    fwrite(&hasEnPassant, sizeof(int), 1, file);
    if (hasEnPassant) {
        fwrite(state->bidakEnPassant, sizeof(Bidak), 1, file);
    }
    
    fclose(file);
    return true;
}
//LOAD GAME
boolean loadGameState(GameState* state, const char* filename) {
    if (state == NULL) {
        filename = NAMA_FILE_SIMPAN;  // Gunakan nama file default jika tidak ada nama file yang diberikan
    }
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return false;
    
    // Load papan
    fread(&state->papan, sizeof(Papan), 1, file);
    
    // Load informasi pemain
    state->pemainPutih = (Player*)malloc(sizeof(Player));
    state->pemainHitam = (Player*)malloc(sizeof(Player));
    if (state->pemainPutih == NULL || state->pemainHitam == NULL) {
        fclose(file);
        return false;
    }
    
    fread(state->pemainPutih, sizeof(Player), 1, file);
    fread(state->pemainHitam, sizeof(Player), 1, file);
    
    // Load giliran saat ini
    int currentTurn;
    fread(&currentTurn, sizeof(int), 1, file);
    state->giliran = (currentTurn == 0) ? state->pemainPutih : state->pemainHitam;
    
    // Load riwayat gerakan
    int moveCount;
    fread(&moveCount, sizeof(int), 1, file);
    
    state->history = NULL;
    for (int i = 0; i < moveCount; i++) {
        MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
        if (newNode == NULL) {
            // Bersihkan memori jika gagal
            while (state->history != NULL) {
                MoveNode* temp = state->history;
                state->history = state->history->next;
                free(temp);
            }
            fclose(file);
            return false;
        }
        
        fread(&newNode->move, sizeof(Move), 1, file);
        newNode->next = state->history;
        state->history = newNode;
    }
    
    // Load status permainan
    fread(&state->skakPutih, sizeof(int), 1, file);
    fread(&state->skakHitam, sizeof(int), 1, file);
    fread(&state->skakmat, sizeof(int), 1, file);
    fread(&state->remis, sizeof(int), 1, file);
    fread(&state->langkahTanpaGerak, sizeof(int), 1, file);
    
    // Load bidak en passant jika ada
    int hasEnPassant;
    fread(&hasEnPassant, sizeof(int), 1, file);
    if (hasEnPassant) {
        state->bidakEnPassant = (Bidak*)malloc(sizeof(Bidak));
        if (state->bidakEnPassant == NULL) {
            fclose(file);
            return false;
        }
        fread(state->bidakEnPassant, sizeof(Bidak), 1, file);
    } else {
        state->bidakEnPassant = NULL;
    }
    
    fclose(file);
    return true;
} 