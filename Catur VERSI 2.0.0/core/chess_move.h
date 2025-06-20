#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H
#include <stdbool.h>
#include <stddef.h> // For NULL
#include "..\game\bidak.h"

// Struct representing position on the chess board
typedef struct {
    int row;    // Row (0-7)
    int col;    // Column (0-7)
} Position;

// Struct to represent a single move in a chess game
typedef struct {
    Position from;      // Starting position of the piece
    Position to;        // Target position of the piece
    TipeBidak bidak;    // Type of piece moved
    TipeBidak captured; // Type of captured piece (if any), TIDAK_ADA if none
    bool isCheck;       // Check status flag
    bool isCheckmate;   // Checkmate status flag
    bool isEnPassant;   // En Passant flag [New]
    bool isCastling;    // Castling flag [New]
    TipeBidak promotionTo; // What piece a pawn promotes to (if applicable) [New]
} Move;

// Node for Stack and Linked List
typedef struct MoveNode {
    Move move;
    struct MoveNode* next;
} MoveNode;

// Struct for stack (undo)
typedef struct {
    MoveNode* top;      // Pointer to the top element of the stack
    int size;           // Number of elements in the stack
} MoveStack;

// Struct for linked list to store all moves
typedef struct {
    MoveNode* head;     // Pointer to the first element
    MoveNode* tail;     // Pointer to the last element
    int size;           // Number of elements in the list
} MoveList;

// Functions for Stack
MoveStack* createMoveStack();
void pushMove(MoveStack* stack, Move move);
Move popMove(MoveStack* stack);
Move peekMove(MoveStack* stack);
bool isStackEmpty(MoveStack* stack);
void clearStack(MoveStack* stack);
void destroyStack(MoveStack* stack);

// Functions for Linked List
MoveList* createMoveList();
void addMove(MoveList* list, Move move);
Move getMoveAt(MoveList* list, int index);
void clearList(MoveList* list);
void destroyList(MoveList* list);

// Utility functions
void createMove(Move *move, Position from, Position to, TipeBidak piece);
void printMove(Move move);
bool isValidPosition(Position pos);

#endif // CHESS_MOVE_H
