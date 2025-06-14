#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "../core/boolean.h"
#include <stdio.h>
#include <string.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50

typedef struct {
    char username[50];
    int totalGames;
    int wins;
    int losses;
    int Elo;
} Account;

boolean login(Account* acc);
boolean registerAccount(Account* acc);
void saveAccount(const Account* acc);
void loadAccount(Account* acc, const char* username);
void printProfile(const Account* acc);

#endif
