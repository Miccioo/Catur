#include "chessController.h"

void startChess() {
	int termWidth, termHeight;
	Papan papan;
    getTerminalSize(&termWidth, &termHeight);
    
    while (1) {
        int choice = showMainMenu(termWidth);
        
        switch (choice) {
            case MENU_NEW_GAME: {
                GameType gameType = showNewGameOption(termWidth);
                handleGameType(termWidth, gameType);
                break;
            }
                break;
                
            case MENU_LOAD_GAME:
                clearScreen();
                printCentered("Loading saved game...", termWidth, BOLD BRIGHT_BLUE);
                printCentered("(Load functionality will be implemented here)", termWidth, BOLD WHITE);
                printCentered("Please ensure your save file is in the correct directory.", termWidth, BOLD WHITE); // Added line
                waitForKeyPress();
                break;
                
            case MENU_SETTINGS:
                settingsScreen(termWidth);
                break;
                
            case MENU_ABOUT:
                aboutScreen(termWidth);
                break;
                
            case MENU_EXIT:
                clearScreen();
                printCentered("Thank you for playing!", termWidth, BOLD BRIGHT_MAGENTA);
                printCentered("Goodbye!", termWidth, BOLD BRIGHT_CYAN);
                return;
        }
    }
}

VersusOption handleVersusMode(int termWidth) {
    VersusOption choice = showVersusOption(termWidth);
    
    switch (choice) {
        case PLAYER_VS_PLAYER:
            printf("PLAYER VS PLAYER");
            break;
        case PLAYER_VS_AI:
            printf("PLAYER vs AI");
            break;
        case AI_VS_AI:
            printf("AI vs AI");
            break;
        case VERSUS_BACK:
            break;
    }
    return choice;
}

void handleGameType(int termWidth, GameType type) {
    if (type == GAME_BACK) return;
    
    VersusOption versusChoice = handleVersusMode(termWidth);
    if (versusChoice == VERSUS_BACK) return;

    
}

//void showNewGameOption(int termWidth, Papan papan) {
//	// Mengurangi ukuran font (tidak bekerja di semua terminal)
//    printf("\033]50;%s\007", "6x12");  // Mencoba set font lebih kecil
//    
//    // Alternatif: resize window terminal (untuk terminal yang mendukung)
//    printf("\033[8;40;120t");  // Set 40 baris, 120 kolom
//	initPapan(&papan);
//	printPapan(papan);
//	Bidak X = getBidakAt(papan, 1, 1);
//	setBidakAt(&papan, X, 3, 3);
//	Bidak kosong;
//	initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, 0, 1, -1);
//	printPapan(papan);
//	getchar();
//	getchar();
//}
