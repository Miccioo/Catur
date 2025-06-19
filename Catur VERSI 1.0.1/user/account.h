#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "../core/boolean.h"
#include <stdio.h>
#include <string.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50

typedef struct {
    char username[MAX_USERNAME];
    int totalGames;
    int wins;
    int losses;
    int Elo;
} Account;

// Deklarasi variabel global untuk menyimpan akun yang sedang login
extern Account* currentLoggedInAccount; // Pointer ke akun yang sedang login

// Deklarasi helper clearInputBuffer
void clearInputBuffer(); // Deklarasikan di sini agar bisa diakses di semua .c yang menggunakannya

boolean login(Account* acc);
boolean registerAccount(Account* acc);
void saveAccount(const Account* acc);
void loadAccount(Account* acc, const char* username);
void printProfile(const Account* acc);

#endif
