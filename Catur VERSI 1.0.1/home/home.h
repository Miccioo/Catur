#ifndef HOME_H
#define HOME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\io\menu.h" // Untuk fungsi utility seperti clearScreen, getKeyPress, printCentered (yang 3 argumen), dll.
#include "..\user\account.h" // Untuk fungsi login dan register

// Enum untuk opsi menu di Home Screen
typedef enum {
    HOME_LOGIN,
    HOME_REGISTER,
    HOME_EXIT,
    HOME_OPTION_COUNT
} HomeOption;

// Deklarasi fungsi kustom untuk printCentered di home.c (yang 4 argumen)
void home_printCentered(const char* text, int totalWidth, const char* textColor, const char* borderColor);

// Deklarasi fungsi untuk menampilkan banner Anda
void displayHomeBanner(int termWidth);

// Deklarasi fungsi utama untuk Home Screen
int showHomeScreen();

#endif // HOME_H
