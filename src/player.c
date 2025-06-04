#include "player.h"
#include <stdio.h>
#include <string.h>

void initPlayer(Player *player, const char *nama, WarnaBidak warna) {
    strncpy(player->nama, nama, MAX_NAMA - 1);
    player->nama[MAX_NAMA - 1] = '\0';
    player->warna = warna;
    player->giliran = (warna == PUTIH) ? 1 : 0; // Putih mulai duluan
    player->skor = 0;
}

void printPlayer(Player player) {
    const char *warnaStr[] = {"Putih", "Hitam"};
    printf("Pemain: %s (%s), Skor: %d, Giliran: %s\n", 
           player.nama, 
           warnaStr[player.warna],
           player.skor,
           player.giliran ? "Ya" : "Tidak");
}
