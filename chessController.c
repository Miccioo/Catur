#include "chessController.h"

void startChess() {
	int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);
    
    while (1) {
        int choice = showMainMenu(termWidth);
        
        switch (choice) {
            case MENU_NEW_GAME:
                showNewGameOption(termWidth);
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
        }
    }
}
