//chessController.c
#include "chessController.h"
#include "../ai/ai_engine.h"  // Tambahkan ini untuk GameTree, minimax, getBestMove
#include "../game/player.h"    // Tambahkan ini untuk Player
#include "../game/papan.h"     // Tambahkan ini untuk Papan
#include "../game/Gamestate.h" // Tambahkan ini untuk GameState, Move, Position, Bidak, isGameOver, applyMove
#include "../core/validator.h" // Tambahkan ini untuk isValidMove
#include <ctype.h> // For tolower
#include <stdio.h> // For sprintf, fgets, sscanf, stdin
#include <string.h> // For strcpy
#include <stdlib.h> // For free

void startChess() {
    int termWidth, termHeight;
    // Papan papan; // papan is not used directly here, only in helper functions.
    getTerminalSize(&termWidth, &termHeight);

    while (1) {
        int choice = showMainMenu(termWidth);

        switch (choice) {
            case MENU_NEW_GAME: {
                GameType gameType = showNewGameOption(termWidth);
                handleGameType(termWidth, gameType);
                break;
            }

            case MENU_LOAD_GAME:
                clearScreen();
                printCentered("Loading saved game...", termWidth, BOLD BRIGHT_BLUE);
                printCentered("(Load functionality will be implemented here)", termWidth, BOLD WHITE);
                printCentered("Please ensure your save file is in the correct directory.", termWidth, BOLD WHITE);
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

void handleGameType(int termWidth, GameType type) {
    if (type == GAME_BACK) return;

    VersusOption versusChoice = showVersusOption(termWidth);
    if (versusChoice == VERSUS_BACK) return;

    switch (type) {
        case GAME_CLASSIC_CHESS:
            classicChess(type, versusChoice);
            break;
        case GAME_EVOLVE_CHESS:
            evolveChess(type, versusChoice);
            break;
        case GAME_ZOMBIE_CHESS:
            zombieChess(type, versusChoice);
            break;
        case GAME_BACK:
        case GAME_COUNT:
            break;
    }
}

void classicChess(GameType type, VersusOption mode) {
    // Initialize game state
    GameState state;
    Player putih, hitam;
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    // Main game loop
    while (!isGameOver(&state)) {
        clearScreen();
        printPapan(state.papan);

        // Print current player turn
        char turnMsg[50];
        sprintf(turnMsg, "%s's turn (%s)",
                state.giliran->nama,
                (state.giliran->warna == PUTIH) ? "White" : "Black");
        printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);

        if ((mode == PLAYER_VS_PLAYER) ||
            (mode == PLAYER_VS_AI && state.giliran->warna == PUTIH)) {
            // Player's turn
            printCentered("Enter your move (e.g. e2 e4): ", termWidth, BOLD WHITE);

            char input[10];
            // Ensure input buffer is cleared or handle newline character
            if (fgets(input, sizeof(input), stdin) == NULL) {
                // Handle EOF or error
                return;
            }
            // Remove trailing newline if present
            input[strcspn(input, "\n")] = 0;

            // Parse move
            Position from, to;
            if (sscanf(input, "%c%c %c%c",
                       &from.col, &from.row,
                       &to.col, &to.row) == 4) {
                // Convert to 0-based indices
                from.col = tolower(from.col) - 'a';
                from.row = 8 - from.row;
                to.col = tolower(to.col) - 'a';
                to.row = 8 - to.row;

                // Create move
                Bidak piece = getBidakAt(state.papan, from.col, from.row);
                // Ensure createMove returns Move struct by value or handle pointer correctly
                Move move = createMove(from, to, piece.tipe); //

                // Validate and apply move
                if (isValidMove(state.papan, &move, state.giliran)) {
                    applyMove(&state, &move);
                } else {
                    printCentered("Invalid move! Try again.", termWidth, BOLD BRIGHT_RED);
                    waitForKeyPress();
                }
            } else {
                 printCentered("Invalid input format. Please use 'e2 e4' format.", termWidth, BOLD BRIGHT_RED);
                 waitForKeyPress();
            }
        } else {
            // AI's turn
            printCentered("AI is thinking...", termWidth, BOLD BRIGHT_CYAN);

            // Create game tree for AI
            GameTree* tree = (GameTree*)createGameTree(&state, 3, (state.giliran->warna == PUTIH)); //

            if (tree != NULL) {
                // Run minimax algorithm
                minimax(tree->root, tree->maxKedalaman, tree->isMaximizingPlayer); //

                // Get best move and apply it
                Move bestMove = getBestMove(tree); //
                applyMove(&state, &bestMove); //

                // Free memory
                free(tree); //
            } else {
                printCentered("Failed to create AI game tree!", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
            }

            waitForKeyPress();
        }
    }

    // Game over
    clearScreen();
    printPapan(state.papan);

    char resultMsg[50];
    if (state.skakmat) {
        sprintf(resultMsg, "Checkmate! %s wins!",
                (state.giliran->warna == PUTIH) ? hitam.nama : putih.nama);
    } else {
        strcpy(resultMsg, "Game ended in a draw!");
    }

    printCentered(resultMsg, termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();
}

void evolveChess(GameType type, VersusOption mode) {
    // Implementation for Evolve Chess
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("Evolve Chess Mode", termWidth, BOLD BRIGHT_CYAN);
    printCentered("(This mode will be implemented in future versions)", termWidth, BOLD WHITE);
    waitForKeyPress();
}

void zombieChess(GameType type, VersusOption mode) {
    // Implementation for Zombie Chess
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("Zombie Chess Mode", termWidth, BOLD BRIGHT_CYAN);
    printCentered("(This mode will be implemented in future versions)", termWidth, BOLD WHITE);
    waitForKeyPress();
}

GameState modeVersus() {
	
    GameState state;
    Player putih, hitam;

    initPlayer(&putih, "Player 1", PUTIH);
    initPlayer(&hitam, "Player 2", HITAM);
    initGameState(&state, &putih, &hitam);
    
    return state;
}

GameState modePVE() {
	
    GameState state;
    Player putih, hitam;

    initPlayer(&putih, "Player 1", PUTIH);
    initPlayer(&hitam, "Computer 2", HITAM);
    initGameState(&state, &putih, &hitam);
    
    return state;
}

GameState modeEVSE() {
	
	GameState state;
    Player putih, hitam;

    initPlayer(&putih, "Computer 1", PUTIH);
    initPlayer(&hitam, "Computer 2", HITAM);
    initGameState(&state, &putih, &hitam);
    
    return state;

}
