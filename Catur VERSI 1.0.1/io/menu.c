#include "menu.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#endif


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void getTerminalSize(int *width, int *height) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE) {
        *width = 80;
        *height = 25;
        fprintf(stderr, "Error: Failed to get console handle. Using default size.\n");
        return;
    }

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        *width = 80;
        *height = 25;
        fprintf(stderr, "Error: Failed to get console screen buffer info. Using default size.\n");
        return;
    }
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        *width = 80;
        *height = 25;
        fprintf(stderr, "Error: Failed to get terminal size (ioctl). Using default size.\n");
        return;
    }
    *width = w.ws_col;
    *height = w.ws_row;
#endif
}


void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void printCentered(const char* text, int termWidth, const char* color) {
    int textLen = strlen(text);
    int actualLen = 0;
    int inEscape = 0;
    int i;
    
    for (i = 0; i < textLen; i++) {
        if (text[i] == '\033') {
            inEscape = 1;
        } else if (inEscape && text[i] == 'm') {
            inEscape = 0;
        } else if (!inEscape) {
            actualLen++;
        }
    }
    
    int padding = (termWidth - actualLen) / 2;
    if (padding > 0) {
        printf("%*s", padding, "");
    }
    printf("%s%s%s\n", color, text, RESET);
}

void waitForKeyPress() {
#ifdef _WIN32
    _getch();
#else
    system("stty raw");
    getchar();
    system("stty cooked");
#endif
}

int getKeyPress() {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
            case 72: return 'U'; // Up
            case 80: return 'D'; // Down
            case 75: return 'L'; // Left
            case 77: return 'R'; // Right
        }
    }
    return ch;
#else
    struct termios oldt, newt;
    int ch;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    ch = getchar();
    
    if (ch == 27) {
        if (getchar() == '[') {
            switch (getchar()) {
                case 'A': ch = 'U'; break;
                case 'B': ch = 'D'; break;
                case 'C': ch = 'R'; break;
                case 'D': ch = 'L'; break;
            }
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

void displayBanner() {
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);
    
    const char* asciiArt[] = {
        ".--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--.",
        "/ .. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\",
        "\\ \\/\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ \\/ /",
        " \\/ /`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'\\/ /",
        " / /\\                                                                            / /\\",
        "/ /\\ \\                                                                          / /\\ \\",
        "\\ \\/ /                                                                          \\ \\/ /",
        " \\/ /        .oooooo.   ooooo   ooooo oooooooooooo  .oooooo..o  .oooooo..o       \\/ /",
        " / /\\       d8P'  `Y8b  `888'   `888' `888'     `8 d8P'    `Y8 d8P'    `Y8       / /\\",
        "/ /\\ \\     888           888     888   888         Y88bo.      Y88bo.           / /\\ \\",
        "\\ \\/ /     888           888ooooo888   888oooo8     `\"Y8888o.   `\"Y8888o.       \\ \\/ /",
        " \\/ /      888           888     888   888    \"         `\"Y88b      `\"Y88b       \\/ /",
        " / /\\      `88b    ooo   888     888   888       o oo     .d8P oo     .d8P       / /\\",
        "/ /\\ \\      `Y8bood8P'  o888o   o888o o888ooooood8 8\"\"88888P'  8\"\"88888P'       / /\\ \\",
        "\\ \\/ /                                                                          \\ \\/ /",
        " \\/ /                                                                            \\/ /",
        " / /\\                                                                            / /\\",
        "/ /\\ \\       .oooooo.          .o.       ooo        ooooo oooooooooooo          / /\\ \\",
        "\\ \\/ /      d8P'  `Y8b        .888.      `88.       .888' `888'     `8          \\ \\/ /",
        " \\/ /      888               .8\"888.      888b     d'888   888                   \\/ /",
        " / /\\      888              .8' `888.     8 Y88. .P  888   888oooo8              / /\\",
        "/ /\\ \\     888     ooooo   .88ooo8888.    8  `888'   888   888    \"             / /\\ \\",
        "\\ \\/ /     `88.    .88'   .8'     `888.   8    Y     888   888       o          \\ \\/ /",
        " \\/ /       `Y8bood8P'   o88o     o8888o o8o        o888o o888ooooood8           \\/ /",
        " / /\\                                                                            / /\\",
        "/ /\\ \\                                                                          / /\\ \\",
        "\\ \\/ /                                                                          \\ \\/ /",
        " \\/ /                                                                            \\/ /",
        " / /\\.--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--..--./ /\\",
        "/ /\\ \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\.. \\/\\ \\",
        "\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `'\\ `' /",
        " `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'"
    };
    
    const char* colors[] = {
        BOLD BRIGHT_CYAN, BOLD BRIGHT_YELLOW, BOLD BRIGHT_YELLOW, BOLD BRIGHT_CYAN,
        BOLD BRIGHT_WHITE, BOLD BRIGHT_WHITE, BOLD BRIGHT_WHITE,
        BOLD BRIGHT_RED BG_BLACK, BOLD BRIGHT_RED BG_BLACK, BOLD BRIGHT_RED BG_BLACK,
        BOLD BRIGHT_RED BG_BLACK, BOLD BRIGHT_RED BG_BLACK, BOLD BRIGHT_RED BG_BLACK,
        BOLD BRIGHT_RED BG_BLACK, BOLD BRIGHT_WHITE, BOLD BRIGHT_WHITE, BOLD BRIGHT_WHITE,
        BOLD BRIGHT_BLUE BG_BLACK, BOLD BRIGHT_BLUE BG_BLACK, BOLD BRIGHT_BLUE BG_BLACK,
        BOLD BRIGHT_BLUE BG_BLACK, BOLD BRIGHT_BLUE BG_BLACK, BOLD BRIGHT_BLUE BG_BLACK,
        BOLD BRIGHT_BLUE BG_BLACK, BOLD BRIGHT_WHITE, BOLD BRIGHT_WHITE, BOLD BRIGHT_WHITE,
        BOLD BRIGHT_WHITE, BOLD BRIGHT_CYAN, BOLD BRIGHT_YELLOW, BOLD BRIGHT_YELLOW,
        BOLD BRIGHT_CYAN
    };
    
    int totalLines = sizeof(asciiArt) / sizeof(asciiArt[0]);
    int startLine = (termHeight - totalLines) / 2;
    int i;
    
    moveCursor(1, startLine);
    for (i = 0; i < totalLines; i++) {
        printCentered(asciiArt[i], termWidth, colors[i]);
    }
    
    printf("\n");
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("?  ?? ? ?  SELAMAT DATANG DI PENGALAMAN CATUR TERBAIK  ? ? ??                    ?", termWidth, BOLD BRIGHT_GREEN);
    printCentered("?--------------------------------------------------------------------------------?", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("?  ? Uji Pikiran Strategis Anda Melawan yang Terbaik! ?                          ?", termWidth, BOLD BRIGHT_YELLOW);
    printCentered("?  ?? Setiap Langkah Berarti - Jadikan Milik Anda Legendaris! ??                ?", termWidth, BOLD BRIGHT_CYAN);
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
}

int showMainMenu(int termWidth) {
    const char *options[] = {
        "GAME BARU",
        "MUAT GAME",
        "PROFIL",
        "TENTANG",
        "KELUAR"
    };
    
    int selected = 0;
    int i;
    
    while (1) {
        clearScreen();
        displayBanner();
        
        printf("\n\n");
        for (i = 0; i < MENU_COUNT; i++) {
            if (i == selected) {
                char menuItem[50];
                sprintf(menuItem, "> %s <", options[i]);
                printCentered(menuItem, termWidth, BOLD BRIGHT_GREEN);
            } else {
                printCentered(options[i], termWidth, BOLD WHITE);
            }
        }
        
        printf("\n");
        printCentered("Gunakan panah untuk navigasi, ENTER untuk memilih", termWidth, BOLD BRIGHT_YELLOW);
        
        int key = getKeyPress();
        
        switch (key) {
            case 'U': selected = (selected - 1 + MENU_COUNT) % MENU_COUNT; break;
            case 'D': selected = (selected + 1) % MENU_COUNT; break;
            case '\r':
            case '\n': return selected;
            case 27: return MENU_EXIT;
        }
    }
}

int showNewGameOption(int termWidth) {
	const char *options[] = {
        "CATUR KLASIK",
        "CATUR EVOLVE",
        "CATUR ZOMBIE",
        "KEMBALI"
    };
    
    int selected = 0;
    int i;
    
    while (1) {
        clearScreen();
        displayBanner();
        
        printf("\n\n");
        for (i = 0; i < GAME_COUNT; i++) {
            if (i == selected) {
                char menuItem[50];
                sprintf(menuItem, "> %s <", options[i]);
                printCentered(menuItem, termWidth, BOLD BRIGHT_GREEN);
            } else {
                printCentered(options[i], termWidth, BOLD WHITE);
            }
        }
        
        printf("\n");
        printCentered("Gunakan panah untuk navigasi, ENTER untuk memilih", termWidth, BOLD BRIGHT_YELLOW);
        
        int key = getKeyPress();
        
        switch (key) {
            case 'U': selected = (selected - 1 + GAME_COUNT) % GAME_COUNT; break;
            case 'D': selected = (selected + 1) % GAME_COUNT; break;
            case '\r':
            case '\n': return selected;
            case 27: return GAME_BACK;
        }
    }
}

int showVersusOption(int termWidth) {
	const char *options[] = {
        "PEMAIN VS PEMAIN",
        "PEMAIN VS AI",
        "AI VS AI",
        "KEMBALI"
    };
    
    int selected = 0;
    int i;
    
    while (1) {
        clearScreen();
        displayBanner();
        
        printf("\n\n");
        for (i = 0; i < VERSUS_COUNT; i++) {
            if (i == selected) {
                char menuItem[50];
                sprintf(menuItem, "> %s <", options[i]);
                printCentered(menuItem, termWidth, BOLD BRIGHT_GREEN);
            } else {
                printCentered(options[i], termWidth, BOLD WHITE);
            }
        }
        
        printf("\n");
        printCentered("Gunakan panah untuk navigasi, ENTER untuk memilih", termWidth, BOLD BRIGHT_YELLOW);
        
        int key = getKeyPress();
        
        switch (key) {
            case 'U': selected = (selected - 1 + VERSUS_COUNT) % VERSUS_COUNT; break;
            case 'D': selected = (selected + 1) % VERSUS_COUNT; break;
            case '\r':
            case '\n': return selected;
            case 27: return VERSUS_BACK;
        }
    }
}

void aboutScreen(int termWidth) {
    clearScreen();
    displayBanner();
    
    printf("\n");
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("?                                TENTANG PERMAINAN CATUR                           ?", termWidth, BOLD BRIGHT_CYAN);
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("?  Permainan catur ini dikembangkan sebagai proyek gairah oleh para penggemar catur.?", termWidth, BOLD WHITE);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("?  Fitur:                                                                        ?", termWidth, BOLD BRIGHT_YELLOW);
    printCentered("?  - Implementasi aturan catur klasik                                            ?", termWidth, BOLD WHITE);
    printCentered("?  - Fungsionalitas simpan/muat permainan                                       ?", termWidth, BOLD WHITE);
    printCentered("?  - Antarmuka ASCII yang indah                                                 ?", termWidth, BOLD WHITE);
    printCentered("?  - Pengaturan yang dapat disesuaikan                                         ?", termWidth, BOLD WHITE);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("?  Versi: 1.0                                                                    ?", termWidth, BOLD BRIGHT_GREEN);
    printCentered("?  Tanggal Rilis: Juni 2024                                                      ?", termWidth, BOLD BRIGHT_GREEN);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    
    printf("\n");
    printCentered("Tekan tombol apapun untuk kembali ke menu utama...", termWidth, BOLD BRIGHT_YELLOW);
    waitForKeyPress();
}

void profileScreen(int termWidth, Account* acc) {
    clearScreen();
    displayBanner();

    printProfile(acc);
}

void settingsScreen(int termWidth) {
    clearScreen();
    displayBanner();
    
    printf("\n");
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("?                                PENGATURAN PERMAINAN                                ?", termWidth, BOLD BRIGHT_CYAN);
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("?  Opsi pengaturan akan diimplementasikan di versi mendatang.                     ?", termWidth, BOLD WHITE);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("?  Fitur yang direncanakan:                                                      ?", termWidth, BOLD BRIGHT_YELLOW);
    printCentered("?  - Kustomisasi warna papan                                                     ?", termWidth, BOLD WHITE);
    printCentered("?  - Pengaturan nama pemain                                                      ?", termWidth, BOLD WHITE);
    printCentered("?  - Tingkat kesulitan                                                           ?", termWidth, BOLD WHITE);
    printCentered("?  - Opsi pengatur waktu permainan                                               ?", termWidth, BOLD WHITE);
    printCentered("?                                                                                ?", termWidth, BOLD WHITE);
    printCentered("+--------------------------------------------------------------------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    
    printf("\n");
    printCentered("Tekan tombol apapun untuk kembali ke menu utama...", termWidth, BOLD BRIGHT_YELLOW);
    waitForKeyPress();
}
