#include "home.h"
#include "..\io\menu.h" // Pastikan ini diinclude untuk mengakses printCentered 3 argumen

// Fungsi kustom untuk printCentered yang hanya digunakan di displayHomeBanner
// Menggunakan nama unik untuk menghindari konflik dengan printCentered di menu.h
void home_printCentered(const char* text, int totalWidth, const char* textColor, const char* borderColor) {
    int textLength = strlen(text);
    // Asumsi: totalWidth adalah lebar konten di dalam border (100).
    // Padding untuk text di dalam contentWidth
    int padding = (totalWidth - textLength) / 2;

    printf("%s |   |", borderColor); // Left border
    int i;
    for (i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s%s", textColor, text); // The colored text
    for (i = 0; i < totalWidth - textLength - padding; i++) {
        printf(" ");
    }
    printf("%s|   |\n", borderColor); // Right border
}

// Fungsi untuk menampilkan banner yang Anda inginkan
void displayHomeBanner(int termWidth) {
    // Pastikan termWidth cukup besar untuk banner (minimal 100 + padding)
    // Jika tidak, tampilkan banner tanpa padding tengah atau sesuaikan lebar
    // Untuk tujuan ini, kita asumsikan termWidth > 100.
    
    // Ini adalah salinan persis dari kode banner yang Anda berikan
    int contentWidth = 100; // Width inside the border, derived from '~' line length

    printf(CYAN); // Set a base color for the frame
    printf(" _____                                                                                                    _____ \n");
    printf("( ___ )                                                                                                  ( ___ )\n");
    printf(" |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   | \n");

    // Print each line of the ASCII art centered
    home_printCentered("'##:::::'##:'########:'##::::::::'######:::'#######::'##::::'##:'########:::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" ##:'##: ##: ##.....:: ##:::::::'##... ##:'##.... ##: ###::'###: ##.....::::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" ##: ##: ##: ##::::::: ##::::::: ##:::..:: ##:::: ##: ####'####: ##:::::::::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" ##: ##: ##: ######::: ##::::::: ##::::::: ##:::: ##: ## ### ##: ######:::::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" ##: ##: ##: ##...:::: ##::::::: ##::::::: ##:::: ##: ##. #: ##: ##...::::::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" ##: ##: ##: ##::::::: ##::::::: ##::: ##: ##:::: ##: ##:.:: ##: ##:::::::::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" . ###. ###:: ########: ########:. ######::. #######:: ##:::: ##: ########:::::::", contentWidth, YELLOW, CYAN);
    home_printCentered(" :...::...:::........::........:::......::::.......:::..:::::..::........::::::::", contentWidth, YELLOW, CYAN);

    home_printCentered("'########::'#######::::::::", contentWidth, GREEN, CYAN);
    home_printCentered("... ##..::'##.... ##:::::::", contentWidth, GREEN, CYAN);
    home_printCentered("::: ##:::: ##:::: ##:::::::", contentWidth, GREEN, CYAN);
    home_printCentered("::: ##:::: ##:::: ##:::::::", contentWidth, GREEN, CYAN);
    home_printCentered("::: ##:::: ##:::: ##:::::::", contentWidth, GREEN, CYAN);
    home_printCentered("::: ##:::: ##:::: ##:::::::", contentWidth, GREEN, CYAN);
    home_printCentered("::: ##::::. #######::::::::", contentWidth, GREEN, CYAN);
    home_printCentered(":::..::::::.......:::::::::", contentWidth, GREEN, CYAN);

    home_printCentered(":'######::'##::::'##:'########::'######:::'######:::'######::::::'###::::'##::::'##:'########:::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered("'##... ##: ##:::: ##: ##.....::'##... ##:'##... ##:'##... ##::::'## ##::: ###::'###: ##.....::::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered(" ##:::..:: ##:::: ##: ##::::::: ##:::..:: ##:::..:: ##:::..::::'##:. ##:: ####'####: ##:::::::::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered(" ##::::::: #########: ######:::. ######::. ######:: ##::'####:'##:::. ##: ## ### ##: ######:::::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered(" ##::::::: ##.... ##: ##...:::::..... ##::..... ##: ##::: ##:: #########: ##. #: ##: ##...::::::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered(" ##::: ##: ##:::: ##: ##:::::::'##::: ##:'##::: ##: ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered(". ######:: ##:::: ##: ########:. ######::. ######::. ######::: ##:::: ##: ##:::: ##: ########:::::::", contentWidth, MAGENTA, CYAN);
    home_printCentered(":......:::..:::::..::........:::......::::......::::......::::..:::::..::..:::::..::........::::::::", contentWidth, MAGENTA, CYAN);

    printf(" |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|\n");
    printf("(_____)                                                                                                  (_____)\n");
    printf("\n"); // Add a new line for spacing
    printf(RESET); // Reset color to default
}


int showHomeScreen() {
    const char *options[] = {
        "LOGIN",
        "REGISTER",
        "EXIT"
    };
    
    int selected = 0;
    int termWidth, termHeight;
    Account currentAccount; // Untuk menyimpan data akun yang login

    while (1) {
        clearScreen();
        getTerminalSize(&termWidth, &termHeight);
        displayHomeBanner(termWidth); // Menggunakan banner kustom

        printf("\n\n");
        int i;
        for (i = 0; i < HOME_OPTION_COUNT; i++) {
            if (i == selected) {
                char menuItem[50];
                sprintf(menuItem, "> %s <", options[i]);
                printCentered(menuItem, termWidth, BOLD BRIGHT_GREEN); // Menggunakan printCentered dari menu.h (3 argumen)
            } else {
                printCentered(options[i], termWidth, BOLD WHITE); // Menggunakan printCentered dari menu.h (3 argumen)
            }
        }
        
        printf("\n");
        printCentered("Use arrow keys to navigate, ENTER to select", termWidth, BOLD BRIGHT_YELLOW);
        
        int key = getKeyPress();
        
        switch (key) {
            case 'U': selected = (selected - 1 + HOME_OPTION_COUNT) % HOME_OPTION_COUNT; break;
            case 'D': selected = (selected + 1) % HOME_OPTION_COUNT; break;
            case '\r': // Enter key
            case '\n': {
                switch (selected) {
                    case HOME_LOGIN:
                        clearScreen();
                        getTerminalSize(&termWidth, &termHeight); // Refresh termWidth after clearScreen
                        displayHomeBanner(termWidth);
                        printCentered("LOGIN", termWidth, BOLD BRIGHT_CYAN);
                        // Implementasi login
                        if (login(&currentAccount)) { // Memanggil fungsi login dari account.h/c
                            printCentered("Login berhasil! Mengalihkan ke halaman utama...", termWidth, BOLD BRIGHT_GREEN);
                            waitForKeyPress(); // Tunggu sebentar
                            return HOME_LOGIN; // Menandakan login berhasil
                        } else {
                            printCentered("Username atau password salah. Coba lagi.", termWidth, BOLD BRIGHT_RED);
                            waitForKeyPress();
                        }
                        break;
                    case HOME_REGISTER:
                        clearScreen();
                        getTerminalSize(&termWidth, &termHeight); // Refresh termWidth after clearScreen
                        displayHomeBanner(termWidth);
                        printCentered("REGISTER", termWidth, BOLD BRIGHT_CYAN);
                        // Implementasi register
                        if (registerAccount(&currentAccount)) { // Memanggil fungsi registerAccount dari account.h/c
                            printCentered("Registrasi berhasil! Silakan login.", termWidth, BOLD BRIGHT_GREEN);
                            waitForKeyPress();
                        } else {
                            printCentered("Registrasi gagal. Username mungkin sudah ada atau input tidak valid.", termWidth, BOLD BRIGHT_RED);
                            waitForKeyPress();
                        }
                        break;
                    case HOME_EXIT:
                        clearScreen();
                        getTerminalSize(&termWidth, &termHeight); // Refresh termWidth after clearScreen
                        printCentered("Terima kasih telah bermain!", termWidth, BOLD BRIGHT_MAGENTA);
                        printCentered("Sampai jumpa!", termWidth, BOLD BRIGHT_CYAN);
                        return HOME_EXIT; // Menandakan ingin keluar
                }
                break; // Break dari case '\r' / '\n'
            }
            case 27: return HOME_EXIT; // Escape key
        }
    }
}
