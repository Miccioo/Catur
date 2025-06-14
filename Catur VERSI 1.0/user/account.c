#include "account.h"

boolean login(Account* acc) {
	char username[MAX_USERNAME], password[MAX_PASSWORD];
	int wins, losses, Elo;
	
	FILE *fp = fopen("../data/akun.txt", "r");
	
	if (fp == NULL) {
	    printf("Gagal membuka file!\n");
	    return;
	}
	
	
}

boolean registerAccount(Account* acc) {
	
}

void saveAccount(const Account* acc) {
	
}

void loadAccount(Account* acc, const char* username) {
	
}

void printProfile(const Account* acc) {
	
}

