#include "chess_move.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementasi function stack
MoveStack* createMoveStack() {
    MoveStack* stack = (MoveStack*)malloc(sizeof(MoveStack));
    if (stack == NULL) {
        printf("Error: Gagal mengalokasi memori untuk stack\n");
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
        printf("Error: Gagal mengalokasi memori untuk node baru\n");
        return;
    }

    newNode->move = move;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

Move popMove(MoveStack* stack) {
    Move emptyMove = {{-1, -1}, {-1, -1}, '\0', '\0', false, false};
    
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
    Move emptyMove = {{-1, -1}, {-1, -1}, '\0', '\0', false, false};
    
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

// Implementasi function Linked List
MoveList* createMoveList() {
    MoveList* list = (MoveList*)malloc(sizeof(MoveList));
    if (list == NULL) {
        printf("Error: Gagal mengalokasi memori untuk list\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// buat nambahin move ke linked list
void addMove(MoveList* list, Move move) {
    if (list == NULL) return;

    MoveNode* newNode = (MoveNode*)malloc(sizeof(MoveNode));
    if (newNode == NULL) {
        printf("Error: Gagal mengalokasi memori untuk node baru\n");
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
    Move emptyMove = {{-1, -1}, {-1, -1}, '\0', '\0', false, false};
    
    if (list == NULL || index < 0 || index >= list->size) {
        return emptyMove;
    }

    MoveNode* current = list->head;
    for (int i = 0; i < index; i++) {
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

// Implementasi function utilitas
Move createMove(Position from, Position to, TipeBidak piece) {
    Move move;
    move.from = from; 
    move.to = to;
    move.bidak = piece;
    move.captured = '\0';
    move.isCheck = false;
    move.isCheckmate = false;
    return move;
}

void printMove(Move move) {
    // Konversi koordinat ke notasi catur (a1-h8)
    char fromCol = 'a' + move.from.col;
    char toCol = 'a' + move.to.col;
    
    printf("%c%d -> %c%d", 
           fromCol, 8 - move.from.row,
           toCol, 8 - move.to.row);
    
    if (move.captured != '\0') {
        printf(" (menangkap %c)", move.captured);
    }
    if (move.isCheck) {
        printf(" +"); //bebas bang untuk menandai skak
    }
    if (move.isCheckmate) {
        printf(" ++"); //bebas bang untuk menandai skakmat
    }
    printf("\n");
}

bool isValidPosition(Position pos) {
    return (pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8);
} 