#include "player.h"
#include <stdio.h>
#include <string.h>

void initPlayer(Player *player, char nama[MAX_NAMA], WarnaBidak warna) {
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

int getScorePutih(Player *putih, Player *hitam) {
	// According to FIDE 
	// Skor akan bertambah kurang lebih 40 jika player baru main dan belum menang 30 kali
	// Skor akan bertambah kurang lebih 20 jika rating masih dibawah rating 2400
	// Skor akan bertambah kurang lebih 10 jika sudah mencapai rating 2400
	
	int scorePutih;
	
	int selisihScore = putih->skor - hitam->skor;
	float scaleWeight = 1.1;

	if (selisihScore < 0) {
		for (int i = 0; i > selisihScore; i -= 30) {
			scaleWeight = scaleWeight - 0.1;
		}
	} 
	
	else if (selisihScore == 0) {
		scaleWeight = 1;
	} 
	
	else {
		for (int i = 0; i > selisihScore; i += 30) {
			scaleWeight = scaleWeight + 0.1;
		}
	}
	
	if (putih->skor < 2400) {
		scorePutih = putih->skor + 20 * scaleWeight;
	} 
	else if (putih->skor > 2400) {
		scorePutih = putih->skor + 10 *scaleWeight;
	}
	
	return scorePutih;
}

int getScoreHitam(Player *putih, Player *hitam) {
	// According to FIDE 
	// Skor akan bertambah kurang lebih 40 jika player baru main dan belum menang 30 kali
	// Skor akan bertambah kurang lebih 20 jika rating masih dibawah rating 2400
	// Skor akan bertambah kurang lebih 10 jika sudah mencapai rating 2400
	
	int scoreHitam;
	
	int selisihScore = putih->skor - hitam->skor;
	float scaleWeight = 1.1;

	if (selisihScore < 0) {
		for (int i = 0; i > selisihScore; i -= 30) {
			scaleWeight = scaleWeight - 0.1;
		}
	} 
	
	else if (selisihScore == 0) {
		scaleWeight = 1;
	} 
	
	else {
		for (int i = 0; i > selisihScore; i += 30) {
			scaleWeight = scaleWeight + 0.1;
		}
	}
	
	if (hitam->skor < 2400) {
		scoreHitam = hitam->skor + 20 * scaleWeight;
	}
	else if (hitam->skor > 2400) {
		scoreHitam = hitam->skor + 10 * scaleWeight;
	}
	
	return scoreHitam;
}
