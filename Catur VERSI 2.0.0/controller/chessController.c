//chessController.c
#include "chessController.h"


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

            case MENU_LOAD_GAME: {
                clearScreen();
                printCentered("Loading saved game...", termWidth, BOLD BRIGHT_BLUE);
                GameState state;
                if (loadGame("savegame.txt", &state)) {
                    printCentered("Game loaded successfully!", termWidth, BOLD BRIGHT_GREEN);
                    waitForKeyPress();
                    // Lanjutkan ke mode permainan dengan state hasil load
                    classicChessLoaded(&state, termWidth);
                } else {
                    printCentered("Failed to load game!", termWidth, BOLD BRIGHT_RED);
                    waitForKeyPress();
                }
                break;
            }

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

// New: Function to handle Evolve mode specific actions (combining pieces)
boolean handleEvolveAction(GameState* state, int termWidth) {
    clearScreen();
    printPapan(state->papan);
    printCentered("EVOLVE MODE: Choose two pieces to combine.", termWidth, BOLD BRIGHT_YELLOW);
    printCentered("Max combined value is 8. Type 'cancel' to go back.", termWidth, BOLD WHITE);

    Position pos1 = {-1, -1}, pos2 = {-1, -1};
    char input_pos[10];
    boolean validSelection = false;
    Bidak piece1, piece2;

    // Select first piece
    while (!validSelection) {
        printCentered("Enter position of first piece (e.g., e2) or 'cancel': ", termWidth, BOLD WHITE);
        if (fgets(input_pos, sizeof(input_pos), stdin) == NULL) return false;
        input_pos[strcspn(input_pos, "\n")] = 0;
        char lowerInput[10];
        int k;
        for (k = 0; input_pos[k]; k++) lowerInput[k] = tolower(input_pos[k]);

        if (strcmp(lowerInput, "cancel") == 0) return false;

        if (sscanf(input_pos, "%c%c", &lowerInput[0], &lowerInput[1]) == 2) {
            pos1.col = tolower(lowerInput[0]) - 'a';
            pos1.row = 8 - (lowerInput[1] - '0');

            piece1 = getBidakAt(state->papan, pos1.col, pos1.row);

            if (piece1.id == -1 || piece1.warna != state->giliran->warna) {
                printCentered("Not your piece or empty. Try again.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                clearScreen();
                printPapan(state->papan);
                printCentered("EVOLVE MODE: Choose two pieces to combine.", termWidth, BOLD BRIGHT_YELLOW);
                printCentered("Max combined value is 8. Type 'cancel' to go back.", termWidth, BOLD WHITE);
                continue;
            }
            if (piece1.tipe == RAJA || piece1.tipe == MENTERI) { // Raja dan Menteri tidak bisa digabungkan
                printCentered("King and Queen cannot be combined. Try again.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                clearScreen();
                printPapan(state->papan);
                printCentered("EVOLVE MODE: Choose two pieces to combine.", termWidth, BOLD BRIGHT_YELLOW);
                printCentered("Max combined value is 8. Type 'cancel' to go back.", termWidth, BOLD WHITE);
                continue;
            }
            validSelection = true;
        } else {
            printCentered("Invalid input format. Use 'e2'.", termWidth, BOLD BRIGHT_RED);
            waitForKeyPress();
            clearScreen();
            printPapan(state->papan);
            printCentered("EVOLVE MODE: Choose two pieces to combine.", termWidth, BOLD BRIGHT_YELLOW);
            printCentered("Max combined value is 8. Type 'cancel' to go back.", termWidth, BOLD WHITE);
        }
    }
    
    clearScreen();
    printPapan(state->papan);
    printCentered("Selected first piece at:", termWidth, BOLD BRIGHT_GREEN);
    char piece1_pos_str[10]; sprintf(piece1_pos_str, "%c%d", 'a'+pos1.col, 8-pos1.row);
    printCentered(piece1_pos_str, termWidth, BOLD BRIGHT_GREEN);
    printCentered("Enter position of second piece (e.g., d4) or 'cancel': ", termWidth, BOLD WHITE);

    validSelection = false;
    // Select second piece
    while (!validSelection) {
        if (fgets(input_pos, sizeof(input_pos), stdin) == NULL) return false;
        input_pos[strcspn(input_pos, "\n")] = 0;
        char lowerInput[10];
        int k;
        for (k = 0; input_pos[k]; k++) lowerInput[k] = tolower(input_pos[k]);

        if (strcmp(lowerInput, "cancel") == 0) return false;

        if (sscanf(input_pos, "%c%c", &lowerInput[0], &lowerInput[1]) == 2) {
            pos2.col = tolower(lowerInput[0]) - 'a';
            pos2.row = 8 - (lowerInput[1] - '0');

            if (pos1.row == pos2.row && pos1.col == pos2.col) {
                printCentered("Cannot combine a piece with itself. Choose a different piece.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                clearScreen();
                printPapan(state->papan);
                printCentered("Selected first piece at:", termWidth, BOLD BRIGHT_GREEN);
                printCentered(piece1_pos_str, termWidth, BOLD BRIGHT_GREEN);
                printCentered("Enter position of second piece (e.g., d4) or 'cancel': ", termWidth, BOLD WHITE);
                continue;
            }

            piece2 = getBidakAt(state->papan, pos2.col, pos2.row);

            if (piece2.id == -1 || piece2.warna != state->giliran->warna) {
                printCentered("Not your piece, empty, or wrong color. Try again.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                clearScreen();
                printPapan(state->papan);
                printCentered("Selected first piece at:", termWidth, BOLD BRIGHT_GREEN);
                printCentered(piece1_pos_str, termWidth, BOLD BRIGHT_GREEN);
                printCentered("Enter position of second piece (e.g., d4) or 'cancel': ", termWidth, BOLD WHITE);
                continue;
            }
            if (piece2.tipe == RAJA || piece2.tipe == MENTERI) { // Raja dan Menteri tidak bisa digabungkan
                printCentered("King and Queen cannot be combined. Try again.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                clearScreen();
                printPapan(state->papan);
                printCentered("Selected first piece at:", termWidth, BOLD BRIGHT_GREEN);
                printCentered(piece1_pos_str, termWidth, BOLD BRIGHT_GREEN);
                printCentered("Enter position of second piece (e.g., d4) or 'cancel': ", termWidth, BOLD WHITE);
                continue;
            }
            validSelection = true;
        } else {
            printCentered("Invalid input format. Use 'e2'.", termWidth, BOLD BRIGHT_RED);
            waitForKeyPress();
            clearScreen();
            printPapan(state->papan);
            printCentered("Selected first piece at:", termWidth, BOLD BRIGHT_GREEN);
            printCentered(piece1_pos_str, termWidth, BOLD BRIGHT_GREEN);
            printCentered("Enter position of second piece (e.g., d4) or 'cancel': ", termWidth, BOLD WHITE);
        }
    }

    // Determine combined piece type
    int value1 = getBidakValue(piece1.tipe);
    int value2 = getBidakValue(piece2.tipe);
    int combinedValue = value1 + value2;

    if (combinedValue > 8) {
        printCentered("Combined value exceeds 8. Cannot evolve.", termWidth, BOLD BRIGHT_RED);
        waitForKeyPress();
        return false;
    }

    TipeBidak evolvedType = TIDAK_ADA;

    // Logic for determining evolved piece type
    // This is based on your "desain bidak.c" and the assumed values
    if ((piece1.tipe == PION && piece2.tipe == PION) || (piece1.tipe == PION_BERAT && piece2.tipe == PION) || (piece1.tipe == PION && piece2.tipe == PION_BERAT)) {
        evolvedType = PION_BERAT; // Pion + Pion (value 2)
    } else if ((value1 == NILAI_PION && value2 == NILAI_KUDA) || (value1 == NILAI_KUDA && value2 == NILAI_PION)) {
        evolvedType = KSATRIA_PIONIR; // Pion + Kuda (value 4)
    } else if ((value1 == NILAI_PION && value2 == NILAI_GAJAH) || (value1 == NILAI_GAJAH && value2 == NILAI_PION)) {
        evolvedType = GAJAH_PENJAGA; // Pion + Gajah (value 4)
    } else if ((value1 == NILAI_PION && value2 == NILAI_BENTENG) || (value1 == NILAI_BENTENG && value2 == NILAI_PION)) {
        evolvedType = BENTENG_PENYERBU; // Pion + Benteng (value 6)
    } else if ((value1 == NILAI_KUDA && value2 == NILAI_KUDA)) {
        evolvedType = KSATRIA_GANDA; // Kuda + Kuda (value 6)
    } else if ((value1 == NILAI_KUDA && value2 == NILAI_GAJAH) || (value1 == NILAI_GAJAH && value2 == NILAI_KUDA)) {
        evolvedType = KOMANDAN_MEDAN; // Kuda + Gajah (value 6)
    } else if ((value1 == NILAI_GAJAH && value2 == NILAI_GAJAH)) {
        evolvedType = GAJAH_AGUNG; // Gajah + Gajah (value 6)
    } else {
        // Handle other combinations or show an error
        printCentered("Invalid combination for Evolve. Try again.", termWidth, BOLD BRIGHT_RED);
        waitForKeyPress();
        return false;
    }
    
    // Choose where to place the new evolved piece (on pos1 or pos2)
    clearScreen();
    printPapan(state->papan);
    printCentered("Evolve successful! Choose where to place the new piece:", termWidth, BOLD BRIGHT_GREEN);
    char choose_pos_msg[100];
    sprintf(choose_pos_msg, "Type '%c%d' (for first piece's spot) or '%c%d' (for second piece's spot): ",
            'a' + pos1.col, 8 - pos1.row, 'a' + pos2.col, 8 - pos2.row);
    printCentered(choose_pos_msg, termWidth, BOLD WHITE);

    Position finalPos;
    boolean validFinalPos = false;
    while(!validFinalPos) {
        if (fgets(input_pos, sizeof(input_pos), stdin) == NULL) return false;
        input_pos[strcspn(input_pos, "\n")] = 0;
        char lowerInput[10];
        int k;
        for (k = 0; input_pos[k]; k++) lowerInput[k] = tolower(input_pos[k]);

        if (sscanf(input_pos, "%c%c", &lowerInput[0], &lowerInput[1]) == 2) {
            finalPos.col = tolower(lowerInput[0]) - 'a';
            finalPos.row = 8 - (lowerInput[1] - '0');

            if ((finalPos.row == pos1.row && finalPos.col == pos1.col) ||
                (finalPos.row == pos2.row && finalPos.col == pos2.col)) {
                validFinalPos = true;
            } else {
                printCentered("Invalid position. Must be one of the original piece positions. Try again.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                clearScreen();
                printPapan(state->papan);
                printCentered("Evolve successful! Choose where to place the new piece:", termWidth, BOLD BRIGHT_GREEN);
                printCentered(choose_pos_msg, termWidth, BOLD WHITE);
            }
        } else {
            printCentered("Invalid input format. Use 'e2'.", termWidth, BOLD BRIGHT_RED);
            waitForKeyPress();
            clearScreen();
            printPapan(state->papan);
            printCentered("Evolve successful! Choose where to place the new piece:", termWidth, BOLD BRIGHT_GREEN);
            printCentered(choose_pos_msg, termWidth, BOLD WHITE);
        }
    }


    // Perform the evolution
    Bidak newPiece;
    initBidak(&newPiece, evolvedType, state->giliran->warna, finalPos.col, finalPos.row, -1); // -1 for new ID, needs proper ID management

    // Set both original positions to empty
    Bidak emptyBidak;
    initBidak(&emptyBidak, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
    setBidakAt(&state->papan, emptyBidak, pos1.col, pos1.row);
    setBidakAt(&state->papan, emptyBidak, pos2.col, pos2.row);

    // Place the new evolved piece
    setBidakAt(&state->papan, newPiece, finalPos.col, finalPos.row);

    printCentered("Pieces evolved successfully!", termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();
    return true; // Evolution successful
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
                
                // New: Prompt for Evolve action in Evolve Chess PvP mode
                if (type == GAME_EVOLVE_CHESS && mode == PLAYER_VS_PLAYER) {
                    printCentered("Enter the position of the piece you want to move (e.g., e2), 'evolve', 'undo', or 'quit': ", termWidth, BOLD WHITE);
                } else {
                    printCentered("Enter the position of the piece you want to move (e.g., e2), 'undo', or 'quit': ", termWidth, BOLD WHITE);
                }


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

                // Handle Evolve option
                if (type == GAME_EVOLVE_CHESS && mode == PLAYER_VS_PLAYER && strcmp(lowerInput_from, "evolve") == 0) {
                    if (handleEvolveAction(&state, termWidth)) {
                        switchTurn(&state); // Evolution counts as a turn
                        updateGameStatus(&state);
                    }
                    continue; // Go back to selecting a piece/action
                }
                
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
                    // PROMPT SAVE sebelum keluar dari game
                    char jawab;
                    printf("Apakah Anda ingin menyimpan permainan sebelum keluar? (y/n): ");
                    scanf(" %c", &jawab);
                    if (jawab == 'y' || jawab == 'Y') {
                        if (saveGame("savegame.txt", &state)) {
                            printf("Permainan berhasil disimpan!\n");
                        } else {
                            printf("Gagal menyimpan permainan!\n");
                        }
                    }
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
                        if (playerMove.bidak == PION && ((playerMove.to.row == 0 && state.giliran->warna == PUTIH) || (playerMove.to.row == 7 && state.giliran->warna == HITAM))) {
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
                minimax(tree->root, tree->maxKedalaman, tree->isMaximizingPlayer, INT_MIN, INT_MAX);
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
    
    int scorePutih = getScorePutih(state.pemainHitam, state.pemainPutih);
    int scoreHitam = getScoreHitam(state.pemainHitam, state.pemainPutih);
    
    hitam.skor = 1000;
    putih.skor = 1000;
    
    if (state.giliran->warna == PUTIH) {
    	hitam.skor = hitam.skor - scoreHitam;
    	putih.skor = putih.skor + scorePutih;
	} 
	else {
    	hitam.skor = hitam.skor + scoreHitam;
    	putih.skor = putih.skor - scorePutih;		
	}
	
	printf("%s ELO: %d\n", putih.nama, putih.skor);
	printf("%s ELO: %d", hitam.nama, hitam.skor);

    printCentered(resultMsg, termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();
}

void evolveChess(GameType type, VersusOption mode) {
    int termWidth, termHeight;
    getTerminalSize(&termWidth, &termHeight);

    clearScreen();
    printCentered("Evolve Chess Mode", termWidth, BOLD BRIGHT_CYAN);
    printCentered("Starting Player vs Player Evolve Chess...", termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();

    // Re-use classicChess logic for now, with GameType passed correctly
    classicChess(type, mode); // Pass 'type' so classicChess knows it's Evolve mode
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

void classicChessLoaded(GameState* state, int termWidth) {
    // Mirip dengan loop di classicChess, tapi gunakan *state yang sudah di-load
    while (!isGameOver(state)) {
        clearScreen();
        printPapan(state->papan);
        char turnMsg[100];
        sprintf(turnMsg, "%s's turn (%s)",
                state->giliran->nama,
                (state->giliran->warna == PUTIH) ? "White" : "Black");
        printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);
        if ((state->giliran->warna == PUTIH && state->isWhiteKingInCheck) ||
            (state->giliran->warna == HITAM && state->isBlackKingInCheck)) {
            printCentered("CHECK!", termWidth, BOLD BRIGHT_RED);
        }
        // Anda bisa menambahkan logika input dan AI di sini sesuai classicChess
        printCentered("Fitur lanjutan: lanjutkan permainan dari save belum sepenuhnya diimplementasikan.", termWidth, BOLD BRIGHT_YELLOW);
        waitForKeyPress();
        break; // Hanya demo, keluar setelah satu tampilan
    }
    clearScreen();
    printPapan(state->papan);
    printCentered("Permainan dari save telah selesai atau keluar.", termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();
}
