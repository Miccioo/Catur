#include "chess_move.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Stack function implementation
MoveStack* createMoveStack() {
    MoveStack* stack = (MoveStack*)malloc(sizeof(MoveStack));
    if (stack == NULL) {
        printf("Error: Failed to allocate memory for stack\n");
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void pushMove(MoveStack* stack, Move move) {
    if (stack == NULL) return;

    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
    if (newNode == NULL) {
        printf("Error: Failed to allocate memory for new node\n");
        return;
    }

    newNode->move = move;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

Move popMove(MoveStack* stack) {
    Move emptyMove = {{-1, -1}, {-1, -1}, TIDAK_ADA, TIDAK_ADA, false, false, false, false, TIDAK_ADA}; // Use TIDAK_ADA
    
    if (stack == NULL || stack->top == NULL) {
        return emptyMove;
    }

    MoveNode* temp = stack->top;
    Move move = temp->move;
    
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    
    return move;
}

Move peekMove(MoveStack* stack) {
    Move emptyMove = {{-1, -1}, {-1, -1}, TIDAK_ADA, TIDAK_ADA, false, false, false, false, TIDAK_ADA}; // Use TIDAK_ADA
    
    if (stack == NULL || stack->top == NULL) {
        return emptyMove;
    }
    
    return stack->top->move;
}

bool isStackEmpty(MoveStack* stack) {
    return (stack == NULL || stack->top == NULL);
}

void clearStack(MoveStack* stack) {
    if (stack == NULL) return;
    
    while (!isStackEmpty(stack)) {
        popMove(stack);
    }
}

void destroyStack(MoveStack* stack) {
    if (stack == NULL) return;
    
    clearStack(stack);
    free(stack);
}

// Linked List function implementation
MoveList* createMoveList() {
    MoveList* list = (MoveList*)malloc(sizeof(MoveList));
    if (list == NULL) {
        printf("Error: Failed to allocate memory for list\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Function to add a move to the linked list
void addMove(MoveList* list, Move move) {
    if (list == NULL) return;

    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
    if (newNode == NULL) {
        printf("Error: Failed to allocate memory for new node\n");
        return;
    }

    newNode->move = move;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

Move getMoveAt(MoveList* list, int index) {
    Move emptyMove = {{-1, -1}, {-1, -1}, TIDAK_ADA, TIDAK_ADA, false, false, false, false, TIDAK_ADA}; // Use TIDAK_ADA
    
    if (list == NULL || index < 0 || index >= list->size) {
        return emptyMove;
    }

    MoveNode* current = list->head;
    int i;
    for (i = 0; i < index; i++) {
        current = current->next;
    }
    
    return current->move;
}

void clearList(MoveList* list) {
    if (list == NULL) return;
    
    MoveNode* current = list->head;
    while (current != NULL) {
        MoveNode* temp = current;
        current = current->next;
        free(temp);
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void destroyList(MoveList* list) {
    if (list == NULL) return;
    
    clearList(list);
    free(list);
}

// Utility function implementation
void createMove(Move* move, Position from, Position to, TipeBidak piece) {
    move->from = from; 
    move->to = to;
    move->bidak = piece;
    move->captured = TIDAK_ADA; // Initialize captured piece to TIDAK_ADA
    move->isCheck = false;
    move->isCheckmate = false;
    move->isEnPassant = false;  // Initialize new flags
    move->isCastling = false;   // Initialize new flags
    move->promotionTo = TIDAK_ADA; // Initialize promotion type
}

void printMove(Move move) {
    // Convert coordinates to chess notation (a1-h8)
    char fromCol = 'a' + move.from.col;
    char toCol = 'a' + move.to.col;
    
    printf("%c%d -> %c%d", 
           fromCol, 8 - move.from.row,
           toCol, 8 - move.to.row);
    
    if (move.captured != TIDAK_ADA) { // Check against TIDAK_ADA
        printf(" (captures %d)", move.captured); // Print captured piece type
    }
    if (move.isCheck) {
        printf(" +"); // Mark check
    }
    if (move.isCheckmate) {
        printf(" ++"); // Mark checkmate
    }
    if (move.isEnPassant) {
        printf(" (en passant)");
    }
    if (move.isCastling) {
        printf(" (castling)");
    }
    if (move.promotionTo != TIDAK_ADA) {
        printf(" (promotes to %d)", move.promotionTo);
    }
    printf("\n");
}

bool isValidPosition(Position pos) {
    return (pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8);
}
