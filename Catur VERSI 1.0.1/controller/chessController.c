//chessController.c
#include "chessController.h"
#include "../ai/ai_engine.h" // Assuming ai_engine uses generateAllValidMoves and minimax correctly
#include "../game/player.h"
#include "../game/papan.h"
#include "../game/Gamestate.h"
#include "../core/validator.h"
#include "../user/account.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For malloc, free

// Function to display valid moves table
void displayValidMovesTable(Papan papan, Move* validMoves, int termWidth) {
    clearScreen();
    printPapan(papan); // Display board again to be visible with the table
    printCentered("Available Moves for Selected Piece:", termWidth, BOLD BRIGHT_YELLOW);

    printf("\n");
    printCentered("+---------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("| VALID MOVES         |", termWidth, BOLD BRIGHT_CYAN);
    printCentered("+---------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printCentered("| Target Position     |", termWidth, BOLD WHITE);
    printCentered("+---------------------+", termWidth, BOLD BRIGHT_MAGENTA);

    if (validMoves[0].bidak == TIDAK_ADA) { // Check if the move list is empty using TIDAK_ADA
        printCentered("|  NO MOVES AVAILABLE |", termWidth, BOLD RED);
    } else {
        int i = 0;
        while (validMoves[i].bidak != TIDAK_ADA) { // Iterate using TIDAK_ADA as end marker
            char moveStr[50];
            char toCol = 'a' + validMoves[i].to.col; // Convert column to char
            
            // Format string for each move (only target position)
            sprintf(moveStr, "| %c%d                |", 
                    toCol, 8 - validMoves[i].to.row); // Convert row to chess notation
            printCentered(moveStr, termWidth, BOLD WHITE);
            i++;
        }
    }
    printCentered("+---------------------+", termWidth, BOLD BRIGHT_MAGENTA);
    printf("\n");
    printCentered("Enter target position (e.g., e4), 'undo', or 'quit'.", termWidth, BOLD BRIGHT_YELLOW);
}


void startChess() {
    int termWidth, termHeight;
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

            case MENU_PROFILE:
                if (currentLoggedInAccount != NULL) {
                    profileScreen(termWidth, currentLoggedInAccount);
                } else {
                    clearScreen();
                    printCentered("You must log in first to view your profile.", termWidth, BOLD BRIGHT_RED);
                    waitForKeyPress();
                }
                break;

            case MENU_ABOUT:
                aboutScreen(termWidth);
                break;

            case MENU_EXIT:
                clearScreen();
                printCentered("Thank you for playing!", termWidth, BOLD BRIGHT_MAGENTA);
                printCentered("Goodbye!", termWidth, BOLD BRIGHT_CYAN);
                if (currentLoggedInAccount != NULL) {
                    free(currentLoggedInAccount);
                    currentLoggedInAccount = NULL;
                }
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
    GameState state;
    Player putih, hitam; // Player objects for the game
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);
    
    // Initialize player names based on game mode
    switch (mode) {
    	case PLAYER_VS_PLAYER:
    		initPlayer(&putih, "White Player", PUTIH);
            initPlayer(&hitam, "Black Player", HITAM);
    		break;
    		
    	case PLAYER_VS_AI:
    		initPlayer(&putih, "Player", PUTIH);
            initPlayer(&hitam, "AI", HITAM);
    		break;
    		
    	case AI_VS_AI:
    		initPlayer(&putih, "AI White", PUTIH);
            initPlayer(&hitam, "AI Black", HITAM);
    		break;
    	
    	default:
            fprintf(stderr, "Invalid game mode\n");
            return;
	}
    initGameState(&state, &putih, &hitam); // Initialize GameState with pointers to local Player objects

    while (!isGameOver(&state)) { // Check game over condition (checkmate or draw)
        clearScreen();
        printPapan(state.papan);

        // Display current player turn and check status
        char turnMsg[100];
        sprintf(turnMsg, "%s's turn (%s)",
                state.giliran->nama,
                (state.giliran->warna == PUTIH) ? "White" : "Black");
        printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);

        // Display check warning if applicable
        if ((state.giliran->warna == PUTIH && state.isWhiteKingInCheck) ||
            (state.giliran->warna == HITAM && state.isBlackKingInCheck)) {
            printCentered("CHECK!", termWidth, BOLD BRIGHT_RED);
        }


        if ((mode == PLAYER_VS_PLAYER && state.giliran->warna == PUTIH) ||
            (mode == PLAYER_VS_PLAYER && state.giliran->warna == HITAM) ||
            (mode == PLAYER_VS_AI && state.giliran->warna == PUTIH)
           ) {
            // Player's turn
            Position from;
            char from_col_char, from_row_char;
            Bidak selectedPiece;
            Move* possibleMovesForSelectedPiece = NULL;

            boolean pieceSelected = false;
            while (!pieceSelected) { // Loop ensures a valid piece is selected
                clearScreen();
                printPapan(state.papan);
                printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);
                if ((state.giliran->warna == PUTIH && state.isWhiteKingInCheck) ||
                    (state.giliran->warna == HITAM && state.isBlackKingInCheck)) {
                    printCentered("CHECK!", termWidth, BOLD BRIGHT_RED);
                }
                printCentered("Enter the position of the piece you want to move (e.g., e2), 'undo', or 'quit': ", termWidth, BOLD WHITE);

                char input_from[10];
                if (fgets(input_from, sizeof(input_from), stdin) == NULL) {
                    return; // Handle EOF or error
                }
                input_from[strcspn(input_from, "\n")] = 0; // Remove newline

                char lowerInput_from[10];
                int k;
                for (k = 0; input_from[k]; k++) {
                    lowerInput_from[k] = tolower(input_from[k]);
                }
                lowerInput_from[k] = '\0';

                if (strcmp(lowerInput_from, "undo") == 0) {
                    if (state.history != NULL) {
                        undoMove(&state);
                        printCentered("Move undone.", termWidth, BOLD BRIGHT_GREEN);
                        // If in Player vs AI mode and AI just moved, undo AI's move too
                        if (mode == PLAYER_VS_AI && state.giliran->warna == HITAM) {
                            if (state.history != NULL) {
                                undoMove(&state);
                                printCentered("AI's previous move also undone.", termWidth, BOLD BRIGHT_GREEN);
                            }
                        }
                    } else {
                        printCentered("No moves to undo.", termWidth, BOLD BRIGHT_RED);
                    }
                    waitForKeyPress();
                    continue; // Go back to selecting a piece
                } else if (strcmp(lowerInput_from, "quit") == 0) {
                    printCentered("Exiting game...", termWidth, BOLD BRIGHT_MAGENTA);
                    waitForKeyPress();
                    return; // Exit classicChess function
                }

                if (sscanf(input_from, "%c%c", &from_col_char, &from_row_char) == 2) {
                    from.col = tolower(from_col_char) - 'a';
                    from.row = 8 - (from_row_char - '0');

                    selectedPiece = getBidakAt(state.papan, from.col, from.row);
                    
                    if (selectedPiece.id == -1 || selectedPiece.warna != state.giliran->warna) {
                        printCentered("Not your piece or starting position is empty. Try again.", termWidth, BOLD BRIGHT_RED);
                        waitForKeyPress();
                        continue; // Go back to selecting a piece
                    }

                    // Generate all valid moves for the current player, filtered by legality
                    Move* allLegalMoves = generateAllValidMoves(state.papan, state.giliran, &state, state.enPassantTargetPawn);
                    
                    possibleMovesForSelectedPiece = (Move*)malloc(sizeof(Move) * MAX_MOVES);
                    int moveCount = 0;
                    int j = 0;
                    // Filter moves only for the selected piece
                    while (allLegalMoves[j].bidak != TIDAK_ADA) { 
                        if (allLegalMoves[j].from.row == from.row && allLegalMoves[j].from.col == from.col) {
                            possibleMovesForSelectedPiece[moveCount++] = allLegalMoves[j];
                        }
                        j++;
                    }
                    possibleMovesForSelectedPiece[moveCount].bidak = TIDAK_ADA; // Mark end of array

                    free(allLegalMoves); // Free memory from generateAllValidMoves

                    if (possibleMovesForSelectedPiece[0].bidak == TIDAK_ADA) { 
                        printCentered("No valid moves for the selected piece. Choose another piece.", termWidth, BOLD BRIGHT_RED);
                        waitForKeyPress();
                        free(possibleMovesForSelectedPiece);
                        possibleMovesForSelectedPiece = NULL;
                        continue; // Go back to selecting a piece
                    }

                    pieceSelected = true;
                } else {
                    printCentered("Invalid input format. Use 'e2'.", termWidth, BOLD BRIGHT_RED);
                    waitForKeyPress();
                    continue; // Go back to selecting a piece
                }
            }

            // Display table of possible moves
            displayValidMovesTable(state.papan, possibleMovesForSelectedPiece, termWidth);
            
            // Now ask for target position
            Position to;
            char to_col_char, to_row_char;
            char input_to[10];

            // Loop for target input
            boolean validTargetInput = false;
            while(!validTargetInput) {
                printCentered("Enter target position (e.g., e4), 'undo', or 'quit'.", termWidth, BOLD WHITE);
                if (fgets(input_to, sizeof(input_to), stdin) == NULL) {
                    free(possibleMovesForSelectedPiece);
                    return;
                }
                input_to[strcspn(input_to, "\n")] = 0;

                char lowerInput_to[10];
                int k;
                for (k = 0; input_to[k]; k++) {
                    lowerInput_to[k] = tolower(input_to[k]);
                }
                lowerInput_to[k] = '\0';

                if (strcmp(lowerInput_to, "undo") == 0) {
                    if (state.history != NULL) {
                        undoMove(&state);
                        printCentered("Move undone.", termWidth, BOLD BRIGHT_GREEN);
                        if (mode == PLAYER_VS_AI && state.giliran->warna == HITAM) {
                            if (state.history != NULL) {
                                undoMove(&state);
                                printCentered("AI's previous move also undone.", termWidth, BOLD BRIGHT_GREEN);
                            }
                        }
                    } else {
                        printCentered("No moves to undo.", termWidth, BOLD BRIGHT_RED);
                    }
                    waitForKeyPress();
                    free(possibleMovesForSelectedPiece); // Ensure memory is freed
                    return; // Return to main menu or startChess()
                } else if (strcmp(lowerInput_to, "quit") == 0) {
                    printCentered("Exiting game...", termWidth, BOLD BRIGHT_MAGENTA);
                    waitForKeyPress();
                    free(possibleMovesForSelectedPiece);
                    return;
                }

                if (sscanf(input_to, "%c%c", &to_col_char, &to_row_char) == 2) {
                    to.col = tolower(to_col_char) - 'a';
                    to.row = 8 - (to_row_char - '0');

                    // Find the exact move from possibleMovesForSelectedPiece
                    Move playerMove = {0}; // Initialize to zero
                    boolean foundExactMove = false;
                    int i = 0;
                    while (possibleMovesForSelectedPiece[i].bidak != TIDAK_ADA) {
                        if (possibleMovesForSelectedPiece[i].from.row == from.row &&
                            possibleMovesForSelectedPiece[i].from.col == from.col &&
                            possibleMovesForSelectedPiece[i].to.row == to.row &&
                            possibleMovesForSelectedPiece[i].to.col == to.col) {
                            playerMove = possibleMovesForSelectedPiece[i]; // Copy the move details
                            foundExactMove = true;
                            break;
                        }
                        i++;
                    }

                    if (foundExactMove) {
                        // Handle Pawn Promotion selection [New]
                        if (playerMove.bidak == PION && ((playerMove.to.row == 0 && playerMove.bidak == PUTIH) || (playerMove.to.row == 7 && playerMove.bidak == HITAM))) {
                            TipeBidak promotedTo = TIDAK_ADA;
                            while (promotedTo == TIDAK_ADA) {
                                clearScreen();
                                printPapan(state.papan);
                                printCentered("Pawn Promotion! Promote to (Q)ueen, (R)ook, (B)ishop, or (K)night?", termWidth, BOLD BRIGHT_YELLOW);
                                char promoInput[10];
                                if (fgets(promoInput, sizeof(promoInput), stdin) == NULL) { promotedTo = MENTERI; break; }
                                promoInput[strcspn(promoInput, "\n")] = 0;
                                char promoChar = tolower(promoInput[0]);
                                switch(promoChar) {
                                    case 'q': promotedTo = MENTERI; break;
                                    case 'r': promotedTo = BENTENG; break;
                                    case 'b': promotedTo = GAJAH; break;
                                    case 'k': promotedTo = KUDA; break; // Be careful, 'K' is also King
                                    default: printCentered("Invalid choice. Try again.", termWidth, BOLD BRIGHT_RED); waitForKeyPress(); break;
                                }
                            }
                            playerMove.promotionTo = promotedTo;
                        }

                        applyMove(&state, &playerMove);
                        printCentered("Move applied successfully.", termWidth, BOLD BRIGHT_GREEN);
                        validTargetInput = true; // Exit target input loop
                    } else {
                        printCentered("Target position is not valid for the selected piece. Try again.", termWidth, BOLD BRIGHT_RED);
                        waitForKeyPress();
                        // Go back to displaying move table and asking for target input
                        displayValidMovesTable(state.papan, possibleMovesForSelectedPiece, termWidth);
                    }
                } else {
                    printCentered("Invalid target input format. Use 'e4'.", termWidth, BOLD BRIGHT_RED);
                    waitForKeyPress();
                    // Go back to displaying move table and asking for target input
                    displayValidMovesTable(state.papan, possibleMovesForSelectedPiece, termWidth);
                }
            }
            free(possibleMovesForSelectedPiece); // Free memory after move is successful or exiting loop

        } else {
            // AI's turn
            printCentered("AI is thinking...", termWidth, BOLD BRIGHT_CYAN);

            // AI needs the updated generateAllValidMoves function that considers legality
            GameTree* tree = (GameTree*)createGameTree(&state, 3, (state.giliran->warna == PUTIH)); // AI engine will call generateAllValidMoves

            if (tree != NULL) {
                minimax(tree->root, tree->maxKedalaman, tree->isMaximizingPlayer);
                Move bestMove = getBestMove(tree);
                applyMove(&state, &bestMove);
            } else {
                printCentered("Failed to create AI game tree!", termWidth, BOLD BRIGHT_RED);
            }
            waitForKeyPress();
        }
    }

    // Game over
    clearScreen();
    printPapan(state.papan);

    char resultMsg[100];
    if (state.skakmat) {
        sprintf(resultMsg, "Checkmate! %s wins!",
                (state.giliran->warna == PUTIH) ? hitam.nama : putih.nama);
    } else if (state.remis) {
        strcpy(resultMsg, "Game ended in a draw (Stalemate or 50-move rule)!");
    } else {
        strcpy(resultMsg, "Game Over (unknown reason)!"); // Should not happen if isGameOver is correct
    }

    printCentered(resultMsg, termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();
}

void evolveChess(GameType type, VersusOption mode) {
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("Evolve Chess Mode", termWidth, BOLD BRIGHT_CYAN);
    printCentered("(This mode will be implemented in future versions)", termWidth, BOLD WHITE);
    waitForKeyPress();
}

void zombieChess(GameType type, VersusOption mode) {
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("Zombie Chess Mode", termWidth, BOLD BRIGHT_CYAN);
    printCentered("(This mode will be implemented in future versions)", termWidth, BOLD WHITE);
    waitForKeyPress();
}

GameState modePVP() {
    GameState state; // Not actually used, player initialization is in classicChess
    return state;
}

GameState modePVE() {
    GameState state; // Not actually used, player initialization is in classicChess
    return state;
}

GameState modeEVE() {
    GameState state; // Not actually used, player initialization is in classicChess
    return state;
}
