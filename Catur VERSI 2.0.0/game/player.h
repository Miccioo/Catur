#ifndef PLAYER_H
#define PLAYER_H

#include "bidak.h"

#define MAX_NAMA 50

// Struct for player
typedef struct {
    char nama[MAX_NAMA];
    WarnaBidak warna;
    int giliran;  // 0 = not turn, 1 = turn
    int skor;
} Player;

// Function to initialize player
void initPlayer(Player *player, char nama[MAX_NAMA], WarnaBidak warna);

// Function to display player info
void printPlayer(Player player);

// Function to get score from player
int getScorePutih(Player *putih, Player *hitam);
int getScoreHitam(Player *putih, Player *hitam);

#endif
