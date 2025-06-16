#include "home.h"
#include "..\io\menu.h"

// Custom centered printing function for the banner
void home_printCentered(const char* text, int totalWidth, const char* textColor, const char* borderColor) {
    int textLength = strlen(text);
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

// Function to display the centered banner
void displayHomeBanner(int termWidth) {
    const int bannerWidth = 106; // Total width of the banner (100 content + 6 for borders)
    int leftPadding = (termWidth - bannerWidth) / 2;
    if (leftPadding < 0) leftPadding = 0; // Ensure no negative padding

    printf(CYAN); // Set base color for the frame
    
    // Top border
    printf("%*s", leftPadding, "");
    printf(" _____                                                                                                    _____ \n");
    printf("%*s", leftPadding, "");
    printf("( ___ )                                                                                                  ( ___ )\n");
    printf("%*s", leftPadding, "");
    printf(" |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   | \n");

    // ASCII art lines
    const char* yellowLines[] = {
        "'##:::::'##:'########:'##::::::::'######:::'#######::'##::::'##:'########:::::::",
        " ##:'##: ##: ##.....:: ##:::::::'##... ##:'##.... ##: ###::'###: ##.....::::::::",
        " ##: ##: ##: ##::::::: ##::::::: ##:::..:: ##:::: ##: ####'####: ##:::::::::::::",
        " ##: ##: ##: ######::: ##::::::: ##::::::: ##:::: ##: ## ### ##: ######:::::::::",
        " ##: ##: ##: ##...:::: ##::::::: ##::::::: ##:::: ##: ##. #: ##: ##...::::::::::",
        " ##: ##: ##: ##::::::: ##::::::: ##::: ##: ##:::: ##: ##:.:: ##: ##:::::::::::::",
        " . ###. ###:: ########: ########:. ######::. #######:: ##:::: ##: ########:::::::",
        " :...::...:::........::........:::......::::.......:::..:::::..::........::::::::"
    };
    
    const char* greenLines[] = {
        "'########::'#######::::::::",
        "... ##..::'##.... ##:::::::",
        "::: ##:::: ##:::: ##:::::::",
        "::: ##:::: ##:::: ##:::::::",
        "::: ##:::: ##:::: ##:::::::",
        "::: ##:::: ##:::: ##:::::::",
        "::: ##::::. #######::::::::",
        ":::..::::::.......:::::::::"
    };
    
    const char* magentaLines[] = {
        ":'######::'##::::'##:'########::'######:::'######:::'######::::::'###::::'##::::'##:'########:::::::",
        "'##... ##: ##:::: ##: ##.....::'##... ##:'##... ##:'##... ##::::'## ##::: ###::'###: ##.....::::::::",
        " ##:::..:: ##:::: ##: ##::::::: ##:::..:: ##:::..:: ##:::..::::'##:. ##:: ####'####: ##:::::::::::::",
        " ##::::::: #########: ######:::. ######::. ######:: ##::'####:'##:::. ##: ## ### ##: ######:::::::::",
        " ##::::::: ##.... ##: ##...:::::..... ##::..... ##: ##::: ##:: #########: ##. #: ##: ##...::::::::::",
        " ##::: ##: ##:::: ##: ##:::::::'##::: ##:'##::: ##: ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::::::::",
        ". ######:: ##:::: ##: ########:. ######::. ######::. ######::: ##:::: ##: ##:::: ##: ########:::::::",
        ":......:::..:::::..::........:::......::::......::::......::::..:::::..::..:::::..::........::::::::"
    };
	
	int i;
    // Print yellow lines
    for (i = 0; i < 8; i++) {
        printf("%*s", leftPadding, "");
        home_printCentered(yellowLines[i], 100, YELLOW, CYAN);
    }

    // Print green lines
    for (i = 0; i < 8; i++) {
        printf("%*s", leftPadding, "");
        home_printCentered(greenLines[i], 100, GREEN, CYAN);
    }

    // Print magenta lines
    for (i = 0; i < 8; i++) {
        printf("%*s", leftPadding, "");
        home_printCentered(magentaLines[i], 100, MAGENTA, CYAN);
    }

    // Bottom border
    printf("%*s", leftPadding, "");
    printf(" |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|\n");
    printf("%*s", leftPadding, "");
    printf("(_____)                                                                                                  (_____)\n");
    
    printf("\n"); // Add spacing
    printf(RESET); // Reset color
}

int showHomeScreen() {
    const char *options[] = {
        "LOGIN",
        "REGISTER",
        "EXIT"
    };
    
    int selected = 0;
    int termWidth, termHeight;
    Account currentAccount;

    while (1) {
        clearScreen();
        getTerminalSize(&termWidth, &termHeight);
        displayHomeBanner(termWidth);

        printf("\n\n");
        int i;
        for (i = 0; i < HOME_OPTION_COUNT; i++) {
            if (i == selected) {
                char menuItem[50];
                sprintf(menuItem, "> %s <", options[i]);
                printCentered(menuItem, termWidth, BOLD BRIGHT_GREEN);
            } else {
                printCentered(options[i], termWidth, BOLD WHITE);
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
                        getTerminalSize(&termWidth, &termHeight);
                        displayHomeBanner(termWidth);
                        printCentered("LOGIN", termWidth, BOLD BRIGHT_CYAN);
                        if (login(&currentAccount)) {
                            printCentered("Login berhasil! Mengalihkan ke halaman utama...", termWidth, BOLD BRIGHT_GREEN);
                            waitForKeyPress();
                            return HOME_LOGIN;
                        } else {
                            printCentered("Username atau password salah. Coba lagi.", termWidth, BOLD BRIGHT_RED);
                            waitForKeyPress();
                        }
                        break;
                    case HOME_REGISTER:
                        clearScreen();
                        getTerminalSize(&termWidth, &termHeight);
                        displayHomeBanner(termWidth);
                        printCentered("REGISTER", termWidth, BOLD BRIGHT_CYAN);
                        if (registerAccount(&currentAccount)) {
                            printCentered("Registrasi berhasil! Silakan login.", termWidth, BOLD BRIGHT_GREEN);
                            waitForKeyPress();
                        } else {
                            printCentered("Registrasi gagal. Username mungkin sudah ada atau input tidak valid.", termWidth, BOLD BRIGHT_RED);
                            waitForKeyPress();
                        }
                        break;
                    case HOME_EXIT:
                        clearScreen();
                        getTerminalSize(&termWidth, &termHeight);
                        printCentered("Terima kasih telah bermain!", termWidth, BOLD BRIGHT_MAGENTA);
                        printCentered("Sampai jumpa!", termWidth, BOLD BRIGHT_CYAN);
                        return HOME_EXIT;
                }
                break;
            }
            case 27: return HOME_EXIT; // Escape key
        }
    }
}
