#include "bidak.h"
#include <stdio.h>

void initBidak(Bidak *bidak, TipeBidak tipe, WarnaBidak warna, int x, int y, int id) {
    bidak->tipe = tipe;
    bidak->warna = warna;
    bidak->x = x;
    bidak->y = y;
    bidak->hasMoved = 0;
    bidak->id = id;
}

void printBidak(Bidak bidak) {
    const char *tipeStr[] = {"Pion", "Benteng", "Kuda", "Gajah", "Menteri", "Raja", "Kosong"};
    const char *warnaStr[] = {"Putih", "Hitam", "Tanpa Warna"};
    
    printf("Bidak %s %s di (%d,%d) ID: %d, Has Moved: %d\n", 
           warnaStr[bidak.warna], 
           tipeStr[bidak.tipe], 
           bidak.x, bidak.y, 
           bidak.id, 
           bidak.hasMoved);
}
