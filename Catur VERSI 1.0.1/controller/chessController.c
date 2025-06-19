//chessController.c
#include "chessController.h"

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
        // Handle back option, do nothing
            break;
    }
}

void classicChess(GameType type, VersusOption mode) {
    // Initialize game state
    GameState state;
    Player *putih_ptr, *hitam_ptr; // Menggunakan pointer untuk pemain agar bisa dialokasikan dinamis

    // Alokasi pemain
    putih_ptr = malloc(sizeof(Player));
    hitam_ptr = malloc(sizeof(Player));

    if (putih_ptr == NULL || hitam_ptr == NULL) {
        fprintf(stderr, "Error: Gagal mengalokasi memori untuk pemain.\n");
        free(putih_ptr); // Bebaskan jika salah satu berhasil dialokasikan
        free(hitam_ptr);
        return;
    }

    int kedalamanMaks = 3; // Contoh kedalaman
	int termWidth, termHeight;
    char from_col, from_row_char, to_col, to_row_char; // Mengubah from_row dan to_row menjadi char
    
    // Inisialisasi killerMoves dengan ukuran yang sesuai
    Move* killerMoves = (Move*)calloc(kedalamanMaks + 1, sizeof(Move)); // +1 karena kedalaman 0 juga bisa punya killer move
    if (killerMoves == NULL) {
        fprintf(stderr, "Error: Gagal mengalokasi memori untuk killerMoves.\n");
        free(putih_ptr); free(hitam_ptr);
        return;
    }

    // Inisialisasi MoveList satu kali
    MoveList *list = createMoveList();
    if (list == NULL) {
        fprintf(stderr, "Error: Gagal mengalokasi memori untuk MoveList.\n");
        free(putih_ptr); free(hitam_ptr); free(killerMoves);
        return;
    }
    
	getTerminalSize(&termWidth, &termHeight);
    
    switch (mode) {
    	case PLAYER_VS_PLAYER:
    		initPlayer(putih_ptr, "Player 1", PUTIH);
            initPlayer(hitam_ptr, "Player 2", HITAM);
    		initGameState(&state, putih_ptr, hitam_ptr);
    		break;
    		
    	case PLAYER_VS_AI:
    		initPlayer(putih_ptr, "Player 1", PUTIH);
            initPlayer(hitam_ptr, "Computer 1", HITAM);
    		initGameState(&state, putih_ptr, hitam_ptr);
    		break;
    		
    	case AI_VS_AI:
    		initPlayer(putih_ptr, "Computer A", PUTIH);
            initPlayer(hitam_ptr, "Computer B", HITAM);
    		initGameState(&state, putih_ptr, hitam_ptr);
    		break;
    	
    	default:
        fprintf(stderr, "Invalid game mode\n");
        destroyList(list);
        free(killerMoves);
        free(putih_ptr); free(hitam_ptr);
        return;
	}

    // Main game loop
    while (!isGameOver(&state)) {
        clearScreen();
        printPapan(state.papan);
        
        char turnMsg[50];
        sprintf(turnMsg, "%s's turn (%s)",
                state.giliran->nama,
                (state.giliran->warna == PUTIH) ? "White" : "Black");
        printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);
        
        // Cek apakah pemain saat ini adalah AI (berdasarkan nama atau atribut khusus)
        // Contoh sederhana: jika nama dimulai dengan "Computer"
        boolean isAIPlayer = (strstr(state.giliran->nama, "Computer") != NULL);

        if (!isAIPlayer) { // Human player's turn
        	printCentered("1. Move", termWidth, BOLD BRIGHT_RED);
			printCentered("2. Undo", termWidth, BOLD BRIGHT_RED);
			printCentered("3. Exit", termWidth, BOLD BRIGHT_RED);
			
			int inputOption;
			char input[10];
			
	        if (fgets(input, sizeof(input), stdin) == NULL) {
	            destroyList(list); free(killerMoves); free(putih_ptr); free(hitam_ptr);
	            return;
	        }
	        input[strcspn(input, "\n")] = 0; // Remove trailing newline
	
	        if (sscanf(input, "%d", &inputOption) != 1) {
	           	printCentered("Invalid input format. Please use number format.", termWidth, BOLD BRIGHT_RED);
	            waitForKeyPress();
	            continue;
			}
            
            // Handle player options (move, undo, exit)
            if (inputOption == 1) {
                printCentered("Enter your move (e.g. e2 e4): ", termWidth, BOLD WHITE);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    destroyList(list); free(killerMoves); free(putih_ptr); free(hitam_ptr);
                    return;
                }
                input[strcspn(input, "\n")] = 0; // Remove trailing newline

                Position from, to;
                if (sscanf(input, "%c%c %c%c",
                           &from_col, &from_row_char,
                           &to_col, &to_row_char) == 4) {
                    // Convert to 0-based indices
                    from.col = tolower(from_col) - 'a';
                    from.row = 8 - (from_row_char - '0'); // Convert char '1'-'8' to int 0-7
                    to.col = tolower(to_col) - 'a';
                    to.row = 8 - (to_row_char - '0');

                    Bidak piece = getBidakAt(state.papan, from.col, from.row);
                    Move currentMove;
                    createMove(&currentMove, from, to, piece.tipe);

                    if (isValidMove(state.papan, &currentMove, state.giliran)) {
                        applyMove(&state, &currentMove);
                        printf("Move applied: %c%c -> %c%c\n", from_col, from_row_char, to_col, to_row_char);
                    } else {
                        printCentered("Invalid move! Try again.", termWidth, BOLD BRIGHT_RED);
                        waitForKeyPress();
                    }
                } else {
                     printCentered("Invalid input format. Please use 'e2 e4' format.", termWidth, BOLD BRIGHT_RED);
                     waitForKeyPress();
                }
            } else if (inputOption == 2) {
                undoMove(&state);
                printCentered("Last move undone.", termWidth, BOLD BRIGHT_YELLOW);
                waitForKeyPress();
            } else if (inputOption == 3) {
                printCentered("Exiting game...", termWidth, BOLD BRIGHT_MAGENTA);
                destroyList(list); free(killerMoves); free(putih_ptr); free(hitam_ptr);
                return;
            } else {
                printCentered("Invalid option.", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
            }
        } else { // AI's turn
            printCentered("AI is thinking...", termWidth, BOLD BRIGHT_CYAN);

            GameTree* tree = createGameTree(&state, kedalamanMaks, (state.giliran->warna == PUTIH));
            if (tree == NULL) {
                printCentered("Failed to create AI game tree!", termWidth, BOLD BRIGHT_RED);
                waitForKeyPress();
                destroyList(list); free(killerMoves); free(putih_ptr); free(hitam_ptr);
                return;
            }
		
		    expandNode(list, tree->root, tree, killerMoves);

            // Setelah expandNode, periksa apakah ada langkah yang ditemukan
            if (tree->root->jumlahAnak == 0) {
                // Tidak ada langkah legal yang ditemukan
                if (isKingInCheck(&state, state.giliran->warna)) {
                    state.skakmat = 1; // Raja dalam skak dan tidak ada langkah legal
                    printCentered("Checkmate! No legal moves for current player.", termWidth, BOLD BRIGHT_GREEN);
                } else {
                    state.remis = 1; // Raja tidak dalam skak, tapi tidak ada langkah legal (Stalemate)
                    printCentered("Stalemate! No legal moves for current player.", termWidth, BOLD BRIGHT_YELLOW);
                }
                waitForKeyPress();
                // Lanjutkan loop utama, yang akan keluar karena isGameOver sekarang true
            } else {
                // Ada langkah legal, lanjutkan dengan minimax
                minimax(tree->root, tree->maxKedalaman, INT_MIN, INT_MAX, tree->isMaximizingPlayer, killerMoves);
                Move bestMove = getBestMove(tree);
                
                // Final check (seharusnya tidak perlu jika validasi di generateAllValidMoves sempurna)
                if (bestMove.from.row != -1 && bestMove.bidak != TIDAK_ADA && isValidMove(state.papan, &bestMove, state.giliran)) {
				    applyMove(&state, &bestMove);
				    printf("AI played: %c%d -> %c%d\n", 
                           'a' + bestMove.from.col, 8 - bestMove.from.row,
                           'a' + bestMove.to.col, 8 - bestMove.to.row);
				} else {
                    // Ini adalah "fallback" jika ada bug yang terlewat
                    printCentered("AI generated a move that failed final validation (major bug!).", termWidth, BOLD BRIGHT_RED);
                    state.remis = 1; // Anggap remis karena AI tidak bisa bergerak
                    waitForKeyPress();
                }
            }

            freeTree(tree->root);
            free(tree);
			printf("\n");
			printCentered("Click Enter to Continue... ", termWidth, BOLD BRIGHT_CYAN);
            waitForKeyPress();
        }
    }

    // Game over
    clearScreen();
    printPapan(state.papan);

    char resultMsg[50];
    if (state.skakmat) {
        sprintf(resultMsg, "Checkmate! %s wins!",
                (state.giliran->warna == PUTIH) ? state.pemainHitam->nama : state.pemainPutih->nama);
    } else if (state.remis) {
        strcpy(resultMsg, "Game ended in a draw (Stalemate or AI no moves)!");
    } else {
        strcpy(resultMsg, "Game ended (unknown reason)!"); // Should ideally be covered by skakmat/remis
    }

    printCentered(resultMsg, termWidth, BOLD BRIGHT_GREEN);
    waitForKeyPress();

    // Clean up all dynamically allocated memory before exiting classicChess
    destroyList(list);
    free(killerMoves);
    free(putih_ptr);
    free(hitam_ptr);
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

// Mode inisialisasi GameState sudah dipindahkan ke classicChess

GameState modePVP() {
    // Mode functions now just return an initialized GameState
    // Player pointers are passed and managed from classicChess
    GameState state;
    // Assume player pointers are handled by the caller (classicChess)
    return state;
}

GameState modePVE() {
    GameState state;
    return state;
}

GameState modeEVE() {
    GameState state;
    return state;
}



////chessController.c
//#include "chessController.h"
//
//void startChess() {
//    int termWidth, termHeight;
//    // Papan papan; // papan is not used directly here, only in helper functions.
//    getTerminalSize(&termWidth, &termHeight);
//
//    while (1) {
//        int choice = showMainMenu(termWidth);
//
//        switch (choice) {
//            case MENU_NEW_GAME: {
//                GameType gameType = showNewGameOption(termWidth);
//                handleGameType(termWidth, gameType);
//                break;
//            }
//
//            case MENU_LOAD_GAME:
//                clearScreen();
//                printCentered("Loading saved game...", termWidth, BOLD BRIGHT_BLUE);
//                printCentered("(Load functionality will be implemented here)", termWidth, BOLD WHITE);
//                printCentered("Please ensure your save file is in the correct directory.", termWidth, BOLD WHITE);
//                waitForKeyPress();
//                break;
//
//            case MENU_ABOUT:
//                aboutScreen(termWidth);
//                break;
//
//            case MENU_EXIT:
//                clearScreen();
//                printCentered("Thank you for playing!", termWidth, BOLD BRIGHT_MAGENTA);
//                printCentered("Goodbye!", termWidth, BOLD BRIGHT_CYAN);
//                return;
//        }
//    }
//}
//
//void handleGameType(int termWidth, GameType type) {
//    if (type == GAME_BACK) return;
//
//    VersusOption versusChoice = showVersusOption(termWidth);
//    if (versusChoice == VERSUS_BACK) return;
//
//    switch (type) {
//        case GAME_CLASSIC_CHESS:
//            classicChess(type, versusChoice);
//            break;
//        case GAME_EVOLVE_CHESS:
//            evolveChess(type, versusChoice);
//            break;
//        case GAME_ZOMBIE_CHESS:
//            zombieChess(type, versusChoice);
//            break;
//        case GAME_BACK:
//        // Handle back option, do nothing
//            break;
//
//    }
//}
//
//void classicChess(GameType type, VersusOption mode) {
//    // Initialize game state
//    GameState state;
//    Player putih, hitam;
//
//    int kedalamanMaks = 2;
//	int termWidth, termHeight, count = 0;
//    char from_col, from_row, to_col, to_row;
//    
//    Move* killerMoves = (Move*)calloc(kedalamanMaks, sizeof(Move));
//    MoveList *list = createMoveList();
//    
//	getTerminalSize(&termWidth, &termHeight);
//    
//    switch (mode) {
//    	case PLAYER_VS_PLAYER:
//    		state = modePVP();
//    		break;
//    		
//    	case PLAYER_VS_AI:
//    		state = modePVE();
//    		break;
//    		
//    	case AI_VS_AI:
//    		state = modeEVE();
//    		break;
//    	
//    	default:
//        // Handle invalid mode
//        fprintf(stderr, "Invalid game mode\n");
//        return;
//	}
//
//    // Main game loop
//    while (!isGameOver(&state)) {
//        clearScreen();
//        printPapan(state.papan);
//        
//        // Print current player turn
//        char turnMsg[50];
//        sprintf(turnMsg, "%s's turn (%s)",
//                state.giliran->nama,
//                (state.giliran->warna == PUTIH) ? "White" : "Black");
//        printCentered(turnMsg, termWidth, BOLD BRIGHT_YELLOW);
//        
//        if (mode == PLAYER_VS_PLAYER) {
//        	
//        	printCentered("1. Move", termWidth, BOLD BRIGHT_RED);
//			printCentered("2. Undo", termWidth, BOLD BRIGHT_RED);
//			printCentered("3. Exit", termWidth, BOLD BRIGHT_RED);
//			
//			int inputOption;
//			char input[10];
//			
//			// Ensure input buffer is cleared or handle newline character
//	        if (fgets(input, sizeof(input), stdin) == NULL) {
//	            return; // Handle EOF or error
//	        }
//	        
//	        // Remove trailing newline if present
//	        input[strcspn(input, "\n")] = 0;
//	
//	        if (sscanf(input, "%d", &inputOption) != 1) {
//	           		printCentered("Invalid input format. Please use number format.", termWidth, BOLD BRIGHT_RED);
//	                waitForKeyPress();
//	                continue;
//				}
//			
//            // Player's turn
//            printCentered("Enter your move (e.g. e2 e4): ", termWidth, BOLD WHITE);
//
//            // Ensure input buffer is cleared or handle newline character
//            if (fgets(input, sizeof(input), stdin) == NULL) {
//                // Handle EOF or error
//                return;
//            }
//
//            // Remove trailing newline if present
//            input[strcspn(input, "\n")] = 0;
//
//            // Parse move
//            Position from, to;
//            if (sscanf(input, "%c%c %c%c",
//                       &from_col, &from_row,
//                       &to_col, &to_row) == 4) {
//                // Convert to 0-based indices
//                from.col = tolower(from_col) - 'a';
//                from.row = 56 - from_row;
//                to.col = tolower(to_col) - 'a';
//                to.row = 56 - to_row;
//
//                // Create move
//                Bidak piece = getBidakAt(state.papan, from.col, from.row);
//                
//                Move* move = malloc(sizeof(Move));
//                createMove(move, from, to, piece.tipe); //
//
//                // Validate and apply move
//                if (isValidMove(state.papan, move, state.giliran)) {
//                    applyMove(&state, move);
//                    printf("FROM COL : %d, FROM ROW: %d, TO COL: %d, TO ROW: %d\n", from.col, from.row, to.col, to.row);
//                    
//                } else {
//                	printf("FROM COL : %d, FROM ROW: %d, TO COL: %d, TO ROW: %d\n", from.col, from.row, to.col, to.row);
//                    printCentered("Invalid move! Try again.", termWidth, BOLD BRIGHT_RED);
//                    waitForKeyPress();
//                    
//                }
//                
//            } else {
//                 printCentered("Invalid input format. Please use 'e2 e4' format.", termWidth, BOLD BRIGHT_RED);
//                 waitForKeyPress();
//            }
//        } else {
//            // AI's turn
//            printCentered("AI is thinking...", termWidth, BOLD BRIGHT_CYAN);
//
//            // Create game tree for AI
//            GameTree* tree = (GameTree*)createGameTree(&state, kedalamanMaks, (state.giliran->warna == PUTIH)); //
//		
//		    expandNode(list, tree->root, tree, killerMoves);
//
//            if (tree != NULL) {
//                // Run minimax algorithm
//                minimax(tree->root, tree->maxKedalaman, INT_MIN, INT_MAX, tree->isMaximizingPlayer, killerMoves);
//                // Get best move and apply it
//                Move bestMove = getBestMove(tree);
//                
////              Position from;      // Posisi awal bidak
////			    Position to;        // Posisi tujuan bidak
////			    TipeBidak bidak;         // Jenis bidak yang gerak
////			    TipeBidak captured;      // Bidak yang dimakan (jika ada)
////			    bool isCheck;       // cek status skak
////			    bool isCheckmate;   // cek status skakmat
////			    bool isFromTT; // apakah langkah ini berasal dari transposition table
////			    int score; // skor evaluasi lang
//                
//                if (isValidMove(state.papan, &bestMove, state.giliran)) {
//				    applyMove(&state, &bestMove);
//				}
//
//                // Free memory
//                freeTree(tree->root);
//                free(tree);
//            } else {
//                printCentered("Failed to create AI game tree!", termWidth, BOLD BRIGHT_RED);
//                waitForKeyPress();
//            }
//			printf("\n");
//			printCentered("Click Enter to Continue... ", termWidth, BOLD BRIGHT_CYAN);
//            waitForKeyPress();
//        }
//    }
//
//    // Game over
//    clearScreen();
//    printPapan(state.papan);
//
//    char resultMsg[50];
//    if (state.skakmat) {
//        sprintf(resultMsg, "Checkmate! %s wins!",
//                (state.giliran->warna == PUTIH) ? hitam.nama : putih.nama);
//    } else {
//        strcpy(resultMsg, "Game ended in a draw!");
//    }
//
//    printCentered(resultMsg, termWidth, BOLD BRIGHT_GREEN);
//    waitForKeyPress();
//}
//
//void evolveChess(GameType type, VersusOption mode) {
//    // Implementation for Evolve Chess
//    int termWidth, termHeight;
//    getTerminalSize(&termWidth, &termHeight);
//
//    clearScreen();
//    printCentered("Evolve Chess Mode", termWidth, BOLD BRIGHT_CYAN);
//    printCentered("(This mode will be implemented in future versions)", termWidth, BOLD WHITE);
//    waitForKeyPress();
//}
//
//void zombieChess(GameType type, VersusOption mode) {
//    // Implementation for Zombie Chess
//    int termWidth, termHeight;
//    getTerminalSize(&termWidth, &termHeight);
//
//    clearScreen();
//    printCentered("Zombie Chess Mode", termWidth, BOLD BRIGHT_CYAN);
//    printCentered("(This mode will be implemented in future versions)", termWidth, BOLD WHITE);
//    waitForKeyPress();
//}
//
//GameState modePVP() {
//	
//    GameState state;
//    Player *putih = malloc(sizeof(Player));
//	Player *hitam = malloc(sizeof(Player));
//
//    initPlayer(putih, "Player 1", PUTIH);
//    initPlayer(hitam, "Player 2", HITAM);
//    initGameState(&state, putih, hitam);
//
//    return state;
//}
//
//GameState modePVE() {
//
//    GameState state;
//    Player *putih = malloc(sizeof(Player));
//	Player *hitam = malloc(sizeof(Player));
//
//    initPlayer(putih, "Player 1", PUTIH);
//    initPlayer(hitam, "Computer 1", HITAM);
//    initGameState(&state, putih, hitam);
//    
//    return state;
//}
//
//GameState modeEVE() {
//
//    GameState state;
//    Player *putih = malloc(sizeof(Player));
//	Player *hitam = malloc(sizeof(Player));
//
//    initPlayer(putih, "Player 1", PUTIH);
//    initPlayer(hitam, "Player 2", HITAM);
//    initGameState(&state, putih, hitam);
//    
//    return state;
//}
