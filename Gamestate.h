#ifndef GAMESTATE_H
#define GAMESTATE_H

//#include "papan.h"
//#include "player.h"
//#include "move.h"

typedef struct MoveNode {
//    Move move;
    struct MoveNode* next;
} MoveNode;

typedef struct {
//    Papan papan;
//    Player* pemainPutih;
//    Player* pemainHitam;
//    Player* giliran;
//    MoveNode* history; // stack of moves
    int skakPutih;
    int skakHitam;
    int skakmat;
    int remis;
    int langkahTanpaGerak;
//    Bidak* bidakEnPassant;
} GameState;

#endif
