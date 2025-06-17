#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H
#include <stdbool.h>
#include <stddef.h> // Tambahkan ini untuk NULL jika belum ada
#include "..\game\bidak.h"

// Struct representasi posisi pada papan catur
typedef struct {
    int row;    // Baris (0-7)
    int col;    // Kolom (0-7)
} Position;

// Struktur untuk merepresentasikan satu langkah dalam permainan catur
typedef struct {
    Position from;      // Posisi awal bidak
    Position to;        // Posisi tujuan bidak
    TipeBidak bidak;         // Jenis bidak yang gerak
    TipeBidak captured;      // Bidak yang dimakan (jika ada) - Pastikan ini juga TipeBidak, bukan char
    bool isCheck;       // cek status skak
    bool isCheckmate;   // cek status skakmat
} Move;

// Node untuk Stack dan Linked List
typedef struct MoveNode {
    Move move;
    struct MoveNode* next;
} MoveNode;

// Struct untuk stack (undo)
typedef struct {
    MoveNode* top;      // Pointer ke elemen teratas stack
    int size;           // Jumlah elemen dalam stack
} MoveStack;

// Struct linked list nyimpan semua langkah
typedef struct {
    MoveNode* head;     // Pointer ke elemen pertama
    MoveNode* tail;     // Pointer ke elemen terakhir
    int size;           // Jumlah elemen dalam list
} MoveList;

// Function untuk Stack
MoveStack* createMoveStack();
void pushMove(MoveStack* stack, Move move);
Move popMove(MoveStack* stack);
Move peekMove(MoveStack* stack);
bool isStackEmpty(MoveStack* stack);
void clearStack(MoveStack* stack);
void destroyStack(MoveStack* stack);

// Function untuk Linked List
MoveList* createMoveList();
void addMove(MoveList* list, Move move);
Move getMoveAt(MoveList* list, int index);
void clearList(MoveList* list);
void destroyList(MoveList* list);

// Function untuk utilitas
// Pastikan move->captured diatur ke TIDAK_ADA atau nilai default yang valid
void createMove(Move *move, Position from, Position to, TipeBidak piece);
void printMove(Move move);
bool isValidPosition(Position pos);

#endif // CHESS_MOVE_H
