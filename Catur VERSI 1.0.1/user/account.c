#include "account.h"
#include "../io/menu.h"
#include <stdlib.h>

boolean login(Account* acc) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    printCentered("Masukkan Username: ", termWidth, BOLD WHITE);
    fflush(stdin);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printCentered("Masukkan Password: ", termWidth, BOLD WHITE);
    fflush(stdin);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    FILE *fp = fopen("data/akun.txt", "r");
    if (fp == NULL) {
        printCentered("File akun tidak ditemukan. Harap daftar terlebih dahulu.", termWidth, BOLD BRIGHT_RED);
        waitForKeyPress();
        return false;
    }

    char fileUsername[MAX_USERNAME], filePassword[MAX_PASSWORD];
    int wins, losses, elo;
    boolean found = false;

    while (fscanf(fp, "%s %s %d %d %d\n", fileUsername, filePassword, &wins, &losses, &elo) == 5) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            strcpy(acc->username, fileUsername);
            acc->wins = wins;
            acc->losses = losses;
            acc->Elo = elo;
            acc->totalGames = wins + losses;
            found = true;
            break;
        }
    }
    fclose(fp);
    return found;
}

boolean registerAccount(Account* acc) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    printCentered("Buat Username Baru: ", termWidth, BOLD WHITE);
    fflush(stdin);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printCentered("Buat Password Baru: ", termWidth, BOLD WHITE);
    fflush(stdin);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // Periksa apakah username sudah ada
    FILE *fp_check = fopen("data/akun.txt", "r");
    if (fp_check != NULL) {
        char fileUsername[MAX_USERNAME], tempPassword[MAX_PASSWORD];
        int tempWins, tempLosses, tempElo;
        while (fscanf(fp_check, "%s %s %d %d %d\n", fileUsername, tempPassword, &tempWins, &tempLosses, &tempElo) == 5) {
            if (strcmp(username, fileUsername) == 0) {
                fclose(fp_check);
                printCentered("Username sudah ada. Pilih username lain.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                return false;
            }
        }
        fclose(fp_check);
    }

    // Coba buka file untuk ditulis
    FILE *fp = fopen("data/akun.txt", "a");
    if (fp == NULL) {
        // Coba buat direktori jika belum ada
        #ifdef _WIN32
            system("mkdir ..\\data 2> nul");
        #else
            system("mkdir -p ../data");
        #endif
        
        // Coba buka file lagi
        fp = fopen("data/akun.txt", "a");
        if (fp == NULL) {
            printCentered("Gagal membuat/membuka file akun.", termWidth, BOLD BRIGHT_RED);
            printCentered("Pastikan program memiliki izin menulis.", termWidth, BOLD BRIGHT_RED);
            waitForKeyPress();
            return false;
        }
    }

    // Tulis data akun baru
    fprintf(fp, "%s %s %d %d %d\n", username, password, 0, 0, 1000);
    fclose(fp);

    // Isi data ke struct account
    strcpy(acc->username, username);
    acc->wins = 0;
    acc->losses = 0;
    acc->Elo = 1000;
    acc->totalGames = 0;
    
    printCentered("Registrasi berhasil!", termWidth, BOLD GREEN);
    waitForKeyPress();
    return true;
}

void saveAccount(const Account* acc) {
    // Implementasi menyimpan perubahan akun
    // (Butuh membaca seluruh file dan menulis ulang)
}

void loadAccount(Account* acc, const char* username) {
    // Implementasi memuat akun
    // (Butuh membaca file dan mencari username)
}

void printProfile(const Account* acc) {
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("+----------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("|             PROFIL PEMAIN              |", termWidth, BOLD BRIGHT_CYAN);
    printCentered("+----------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    
    char buffer[100];
    sprintf(buffer, "Username: %s", acc->username);
    printCentered(buffer, termWidth, BOLD WHITE);
    
    sprintf(buffer, "Total Game: %d", acc->totalGames);
    printCentered(buffer, termWidth, BOLD WHITE);
    
    sprintf(buffer, "Menang: %d", acc->wins);
    printCentered(buffer, termWidth, BOLD GREEN);
    
    sprintf(buffer, "Kalah: %d", acc->losses);
    printCentered(buffer, termWidth, BOLD RED);
    
    sprintf(buffer, "ELO: %d", acc->Elo);
    printCentered(buffer, termWidth, BOLD YELLOW);

    printCentered("+----------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("Tekan tombol apapun untuk kembali...", termWidth, BOLD BRIGHT_YELLOW);
    waitForKeyPress();
}
