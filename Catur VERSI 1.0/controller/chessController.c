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

// Assuming these are defined in chessController.h or another included utility header
extern void getTerminalSize(int* width, int* height);
extern int showMainMenu(int termWidth);
extern GameType showNewGameOption(int termWidth);
extern void clearScreen();
extern void printCentered(const char* text, int termWidth, const char* color);
extern void waitForKeyPress();
extern void settingsScreen(int termWidth);
extern void aboutScreen(int termWidth);
extern VersusOption showVersusOption(int termWidth);
extern void printPapan(Papan papan); // From papan.h

// Forward declarations if not in chessController.h
VersusOption handleVersusMode(int termWidth);
void handleGameType(int termWidth, GameType type);
void classicChess(GameType type, VersusOption mode);
void evolveChess(GameType type, VersusOption mode);
void zombieChess(GameType type, VersusOption mode);
void modePVP(VersusOption mode);
void modePVE(VersusOption mode);
void modeEVSE(VersusOption mode);


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

VersusOption handleVersusMode(int termWidth) {
    VersusOption choice = showVersusOption(termWidth);

    switch (choice) {
        case PLAYER_VS_PLAYER:
            modePVP(choice);
            break;
        case PLAYER_VS_AI:
            modePVE(choice);
            break;
        case AI_VS_AI:
            modeEVSE(choice);
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

    initPlayer(&putih, "Player 1", PUTIH);
    initPlayer(&hitam, (mode == PLAYER_VS_PLAYER) ? "Player 2" : "Computer", HITAM);
    initGameState(&state, &putih, &hitam);

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
            if (sscanf(input, "%c%d %c%d",
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
            // The warning about 'cast to pointer from integer of different size' indicates
            // that 'createGameTree' is still not properly prototyped to return 'GameTree*'.
            // Ensure '../ai/ai_engine.h' has 'GameTree* createGameTree(...);' declared.
            // Keeping the cast for now, but the ideal fix is the header.
            GameTree* tree = (GameTree*)createGameTree(&state, 3, (state.giliran->warna == PUTIH)); //

            if (tree != NULL) {
                // Run minimax algorithm
                minimax(tree->root, tree->maxKedalaman, tree->isMaximizingPlayer); //

                // Get best move and apply it
                // The error 'invalid initializer' here means 'getBestMove' is not prototyped
                // to return a 'Move' struct. Ensure '../ai/ai_engine.h' has
                // 'Move getBestMove(GameTree* tree);' declared.
                Move bestMove = getBestMove(tree); //
                applyMove(&state, &bestMove); //

                // Free memory
                // Ensure free(tree) is safe and tree was allocated by malloc or similar
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

void modePVP(VersusOption mode) {
    // Player vs Player mode specific initialization
    // Handled in classicChess function
}

void modePVE(VersusOption mode) {
    // Player vs AI mode specific initialization
    // Handled in classicChess function
}

void modeEVSE(VersusOption mode) {
    // AI vs AI mode specific initialization
    GameState state;
    Player putih, hitam;
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    initPlayer(&putih, "Computer 1", PUTIH);
    initPlayer(&hitam, "Computer 2", HITAM);
    initGameState(&state, &putih, &hitam);

    // Main game loop
    while (!isGameOver(&state)) {
        clearScreen();
        printPapan(state.papan);

        // Print current AI turn
        char turnMsg[50];
        sprintf(turnMsg, "%s's turn (%s)",
                state.giliran->nama,
                (state.giliran->warna == PUTIH) ? "White" : "Black");
        printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);

        printCentered("AI is thinking...", termWidth, BOLD BRIGHT_CYAN);

        // Create game tree for AI
        // Same issue as in classicChess. Ensure '../ai/ai_engine.h' has
        // 'GameTree* createGameTree(...);' declared.
        GameTree* tree = (GameTree*)createGameTree(&state, 3, (state.giliran->warna == PUTIH)); //

        if (tree != NULL) {
            // Run minimax algorithm
            minimax(tree->root, tree->maxKedalaman, tree->isMaximizingPlayer); //

            // Get best move and apply it
            // Same issue as in classicChess. Ensure '../ai/ai_engine.h' has
            // 'Move getBestMove(GameTree* tree);' declared.
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
