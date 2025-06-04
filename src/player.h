#ifndef PLAYER_H
#define PLAYER_H

#include "bidak.h" // Tambahkan ini

#define MAX_NAMA 50

// Struct untuk pemain
typedef struct {
    char nama[MAX_NAMA];
    WarnaBidak warna;
    int giliran;  // 0 = bukan giliran, 1 = giliran
    int skor;
} Player;

// Fungsi untuk inisialisasi player
void initPlayer(Player *player, const char *nama, WarnaBidak warna);

// Fungsi untuk menampilkan info player
void printPlayer(Player player);

#endif
