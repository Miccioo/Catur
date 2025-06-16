#include "account.h"
#include "..\io\menu.h"

// Definisi variabel global
Account* currentLoggedInAccount = NULL; // Inisialisasi dengan NULL

// Fungsi helper untuk membersihkan buffer input stdin
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

boolean login(Account* acc) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    printCentered("Masukkan Username: ", termWidth, BOLD WHITE);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // Hapus newline
    clearInputBuffer();

    printCentered("Masukkan Password: ", termWidth, BOLD WHITE);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0; // Hapus newline
    clearInputBuffer();

    FILE *fp = fopen("data/akun.txt", "r");
    if (fp == NULL) {
        printCentered("File akun tidak ditemukan. Harap daftar terlebih dahulu.", termWidth, BOLD BRIGHT_RED);
        waitForKeyPress();
        return false;
    }

    char fileUsername[MAX_USERNAME], filePassword[MAX_PASSWORD];
    int wins, losses, elo;
    boolean found = false;

    while (fscanf(fp, "%49s %49s %d %d %d\n", fileUsername, filePassword, &wins, &losses, &elo) == 5) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            // Salin data ke 'acc'
            strcpy(acc->username, fileUsername);
            acc->wins = wins;
            acc->losses = losses;
            acc->Elo = elo;
            acc->totalGames = wins + losses;
            found = true;
            
            // Set akun yang sedang login secara global
            // Anda perlu mengalokasikan memori untuk currentLoggedInAccount jika acc adalah variabel lokal.
            // Atau, jika acc sendiri adalah global/static, Anda bisa langsung mengassignnya.
            // Untuk skenario ini, kita asumsikan 'acc' yang diterima adalah penunjuk ke suatu objek yang valid
            // atau kita akan melakukan deep copy ke currentLoggedInAccount.
            
            // Cara aman adalah mengalokasikan memori baru jika currentLoggedInAccount belum dialokasikan
            // atau jika Anda ingin memastikan salinan yang terpisah.
            if (currentLoggedInAccount == NULL) {
                currentLoggedInAccount = (Account*) malloc(sizeof(Account));
            }
            if (currentLoggedInAccount != NULL) {
                *currentLoggedInAccount = *acc; // Deep copy data akun
            } else {
                fprintf(stderr, "Gagal mengalokasikan memori untuk akun login.\n");
                // Handle error more gracefully
            }

            break;
        }
    }
    fclose(fp);
    return found;
}

// RegisterAccount juga perlu set currentLoggedInAccount jika user langsung login setelah register
boolean registerAccount(Account* acc) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    printCentered("Buat Username Baru: ", termWidth, BOLD WHITE);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    clearInputBuffer();

    printCentered("Buat Password Baru: ", termWidth, BOLD WHITE);
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    clearInputBuffer();

    FILE *fp_check = fopen("data/akun.txt", "r");
    if (fp_check != NULL) {
        char fileUsername[MAX_USERNAME], tempPassword[MAX_PASSWORD];
        int tempWins, tempLosses, tempElo;
        while (fscanf(fp_check, "%49s %49s %d %d %d\n", fileUsername, tempPassword, &tempWins, &tempLosses, &tempElo) == 5) {
            if (strcmp(username, fileUsername) == 0) {
                fclose(fp_check);
                printCentered("Username sudah ada. Pilih username lain.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                return false;
            }
        }
        fclose(fp_check);
    }

    FILE *fp = fopen("data/akun.txt", "a");
    if (fp == NULL) {
        printCentered("Gagal membuka file akun untuk menulis.", termWidth, BOLD BRIGHT_RED);
        waitForKeyPress();
        return false;
    }

    fprintf(fp, "%s %s %d %d %d\n", username, password, 0, 0, 1000);
    fclose(fp);

    strcpy(acc->username, username);
    acc->wins = 0;
    acc->losses = 0;
    acc->Elo = 1000;
    acc->totalGames = 0;
    
    // Setelah registrasi, user belum otomatis "login",
    // jadi currentLoggedInAccount TIDAK diatur di sini.
    // User akan perlu login secara eksplisit setelah register.
    
    return true;
}

void saveAccount(const Account* acc) {
    // Untuk mengimplementasikan save, Anda harus membaca semua akun,
    // mencari akun yang cocok dengan `acc->username`, perbarui datanya,
    // lalu tulis ulang seluruh file. Ini adalah contoh dasarnya:
    
    // (Implementasi yang lebih robust akan memerlukan daftar akun dalam memori
    // atau file sementara untuk menulis ulang data)
    
    // Contoh sederhana (tidak ideal untuk banyak data):
    FILE *fp_read = fopen("data/akun.txt", "r");
    if (fp_read == NULL) return; // File tidak ada atau error

    // Baca semua baris ke memori
    char buffer[256]; // Buffer untuk membaca setiap baris
    char tempContent[1024 * 10]; // Buffer besar untuk seluruh konten file, asumsi maksimal 10KB
    tempContent[0] = '\0'; // Inisialisasi string kosong

    char fileUsername[MAX_USERNAME], filePassword[MAX_PASSWORD];
    int wins, losses, elo;
    int dataReadCount;
    
    while ((dataReadCount = fscanf(fp_read, "%49s %49s %d %d %d\n", fileUsername, filePassword, &wins, &losses, &elo)) == 5) {
        if (strcmp(fileUsername, acc->username) == 0) {
            // Ini adalah akun yang akan diupdate
            sprintf(buffer, "%s %s %d %d %d\n", acc->username, filePassword, acc->wins, acc->losses, acc->Elo);
        } else {
            // Ini akun lain, tambahkan kembali ke buffer
            sprintf(buffer, "%s %s %d %d %d\n", fileUsername, filePassword, wins, losses, elo);
        }
        strcat(tempContent, buffer);
    }
    fclose(fp_read);

    // Tulis ulang seluruh file
    FILE *fp_write = fopen("data/akun.txt", "w"); // Mode "w" akan menimpa file
    if (fp_write == NULL) return;
    fprintf(fp_write, "%s", tempContent);
    fclose(fp_write);
}

void loadAccount(Account* acc, const char* username) {
    FILE *fp = fopen("data/akun.txt", "r");
    if (fp == NULL) return;

    char fileUsername[MAX_USERNAME], filePassword[MAX_PASSWORD];
    int wins, losses, elo;

    while (fscanf(fp, "%49s %49s %d %d %d\n", fileUsername, filePassword, &wins, &losses, &elo) == 5) {
        if (strcmp(username, fileUsername) == 0) {
            strcpy(acc->username, fileUsername);
            acc->wins = wins;
            acc->losses = losses;
            acc->Elo = elo;
            acc->totalGames = wins + losses;
            break;
        }
    }
    fclose(fp);
}

void printProfile(const Account* acc) {
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("+----------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("|             PROFIL PEMAIN              |", termWidth, BOLD BRIGHT_CYAN);
    printCentered("+----------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    
    char buffer[100];
    
    if (acc != NULL) { // Periksa apakah akun valid
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
    } else {
        printCentered("Tidak ada akun yang login.", termWidth, BOLD BRIGHT_RED);
    }

    printCentered("+----------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("Tekan tombol apapun untuk kembali...", termWidth, BOLD BRIGHT_YELLOW);
    waitForKeyPress();
}
