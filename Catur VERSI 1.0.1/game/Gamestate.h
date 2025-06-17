#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "..\game\papan.h"
#include "..\game\player.h"
#include "..\io\menu.h"
#include "..\core\chess_move.h"
#include "..\core\boolean.h"

// IMPORTANT: Do NOT include "..\core\validator.h" here.
// This is the key to breaking the circular dependency.

typedef struct {
    Papan papan;
    Player* pemainPutih;
    Player* pemainHitam;
    Player* giliran;
    MoveNode* history; // stack of moves
    boolean isWhiteKingInCheck;
    boolean isBlackKingInCheck;
    int skakmat; 
    int remis;   
    int langkahTanpaGerak; 
    Bidak* enPassantTargetPawn; 
} GameState;

// Function prototypes... (no changes to these)
void initGameState(GameState* state, Player* putih, Player* hitam);
void copyGameState(GameState* dest, GameState* src);
boolean isGameOver(GameState* state);
void switchTurn(GameState* state);
void applyMove(GameState* state, Move* move);
void undoMove(GameState* state);
void updateGameStatus(GameState* state);

#endif
