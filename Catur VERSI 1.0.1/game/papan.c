#include "papan.h"
#include <stdio.h>
#include <stdlib.h> // For abs() if used, but math.h also provides it.

void initPapan(Papan *papan) {
    // Initialize empty board
    int x, y; 
    for (y = 0; y < UKURAN_PAPAN; y++) {
        for (x = 0; x < UKURAN_PAPAN; x++) {
            Bidak kosong;
            initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, x, y, -1);
            papan->grid[y][x] = kosong;
        }
    }
    
    // Initialize white pieces
    int idCounter = 0; 
    
    // Row 0 (index 0): Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook (Black)
    initBidak(&(papan->grid[0][0]), BENTENG, HITAM, 0, 0, idCounter++);
    initBidak(&(papan->grid[0][1]), KUDA, HITAM, 1, 0, idCounter++);
    initBidak(&(papan->grid[0][2]), GAJAH, HITAM, 2, 0, idCounter++);
    initBidak(&(papan->grid[0][3]), MENTERI, HITAM, 3, 0, idCounter++);
    initBidak(&(papan->grid[0][4]), RAJA, HITAM, 4, 0, idCounter++);
    initBidak(&(papan->grid[0][5]), GAJAH, HITAM, 5, 0, idCounter++);
    initBidak(&(papan->grid[0][6]), KUDA, HITAM, 6, 0, idCounter++);
    initBidak(&(papan->grid[0][7]), BENTENG, HITAM, 7, 0, idCounter++);
    
    // Row 1 (index 1): Black Pawns
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[1][x]), PION, HITAM, x, 1, idCounter++);
    }
    
    // Initialize black pieces (should be white based on standard chess board setup)
    // Row 7 (index 7): Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook (White)
    initBidak(&(papan->grid[7][0]), BENTENG, PUTIH, 0, 7, idCounter++);
    initBidak(&(papan->grid[7][1]), KUDA, PUTIH, 1, 7, idCounter++);
    initBidak(&(papan->grid[7][2]), GAJAH, PUTIH, 2, 7, idCounter++);
    initBidak(&(papan->grid[7][3]), MENTERI, PUTIH, 3, 7, idCounter++);
    initBidak(&(papan->grid[7][4]), RAJA, PUTIH, 4, 7, idCounter++);
    initBidak(&(papan->grid[7][5]), GAJAH, PUTIH, 5, 7, idCounter++);
    initBidak(&(papan->grid[7][6]), KUDA, PUTIH, 6, 7, idCounter++);
    initBidak(&(papan->grid[7][7]), BENTENG, PUTIH, 7, 7, idCounter++);
    
    // Row 6 (index 6): White Pawns
    for (x = 0; x < UKURAN_PAPAN; x++) {
        initBidak(&(papan->grid[6][x]), PION, PUTIH, x, 6, idCounter++);
    }
}

void printPapan(Papan papan) {
    // Use macros from bidak.h for consistency
    const int VISUAL_HEIGHT = BIDAK_VISUAL_HEIGHT;
    const int CELL_WIDTH = BIDAK_VISUAL_WIDTH;

    int x, y, i, row_visual; 
    char col_char_val; 

    // Print column labels at the top
    printf("\n    "); // 4 initial spaces to offset row labels
    for (col_char_val = 0; col_char_val < UKURAN_PAPAN; col_char_val++) {
        printf("          %c          ", 'a' + col_char_val); // 10 spaces, char, 10 spaces
    }
    printf("\n");

    // Print top horizontal line
    printf("   +"); // Prefix for top horizontal line
    for (x = 0; x < UKURAN_PAPAN; x++) {
        for (i = 0; i < CELL_WIDTH; i++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    for (y = 0; y < UKURAN_PAPAN; y++) {
        for (row_visual = 0; row_visual < VISUAL_HEIGHT; row_visual++) {
            if (row_visual == VISUAL_HEIGHT / 2) { 
                printf("%d  |", 8 - y); // Row number centered vertically in cell
            } else {
                printf("   |"); // Padding for row number
            }

            for (x = 0; x < UKURAN_PAPAN; x++) {
                Bidak b = papan.grid[y][x];
				
                printBidakColor(b, row_visual, x);
                
                printf("|");
                printf(RESETCOLOR);
            }
            
            if (row_visual == VISUAL_HEIGHT / 2) { 
                 printf(" %d\n", 8 - y); // Row number centered vertically in cell
            } else {
                 printf("\n");
            }
        }
        // Print horizontal line between rows
        printf("   +"); 
        for (x = 0; x < UKURAN_PAPAN; x++) {
            for (i = 0; i < CELL_WIDTH; i++) {
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }

    // Print column labels at the bottom
    printf("    "); 
    for (col_char_val = 0; col_char_val < UKURAN_PAPAN; col_char_val++) {
        printf("          %c          ", 'a' + col_char_val); // 10 spaces, char, 10 spaces
    }
    printf("\n\n");
}


Bidak getBidakAt(Papan papan, int x, int y) { // x is column, y is row
    if (x >= 0 && x < UKURAN_PAPAN && y >= 0 && y < UKURAN_PAPAN) {
        return papan.grid[y][x]; // Access grid as [row][column]
    } else {
        Bidak kosong;
        initBidak(&kosong, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
        return kosong;
    }
}

void setBidakAt(Papan *papan, Bidak bidak, int x, int y) {
    if (x >= 0 && x < UKURAN_PAPAN && y >= 0 && y < UKURAN_PAPAN) {
        bidak.x = x; // Update internal coordinates of the piece
        bidak.y = y;
        papan->grid[y][x] = bidak; // Place piece at grid[row][column]
    }
}

void pindahkanBidak(Papan *papan, Move* move) {
	int xAwal = move->from.col;
	int yAwal = move->from.row;
	int xTujuan = move->to.col;
	int yTujuan = move->to.row;
    
    // Declare dx here
    int dx = move->to.col - move->from.col; // NEW: Declare and calculate dx here
	
    if (xAwal >= 0 && xAwal < UKURAN_PAPAN && yAwal >= 0 && yAwal < UKURAN_PAPAN &&
        xTujuan >= 0 && xTujuan < UKURAN_PAPAN && yTujuan >= 0 && yTujuan < UKURAN_PAPAN) {
        
        Bidak bidak = papan->grid[yAwal][xAwal]; // Get piece from starting position (grid[row][column])
        
        // Handle Castling
        if (move->isCastling) {
            Bidak rook;
            Position rookFrom, rookTo;
            // Determine which rook moved based on king's target column
            if (dx == 2) { // Kingside Castling (e.g., e1 to g1)
                rook = papan->grid[yAwal][7]; // Rook at h-file
                rookFrom = (Position){yAwal, 7};
                rookTo = (Position){yAwal, 5}; // Rook moves to f-file
            } else { // Queenside Castling (e.g., e1 to c1)
                rook = papan->grid[yAwal][0]; // Rook at a-file
                rookFrom = (Position){yAwal, 0};
                rookTo = (Position){yAwal, 3}; // Rook moves to d-file
            }
            // Move the rook
            Bidak emptySquareRookFrom;
            initBidak(&emptySquareRookFrom, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
            papan->grid[rookFrom.row][rookFrom.col] = emptySquareRookFrom; // Clear rook's original spot
            setBidakAt(papan, rook, rookTo.col, rookTo.row); // Place rook at new spot
            papan->grid[rookTo.row][rookTo.col].hasMoved = 1; // Mark rook as moved
        }
        
        // Handle En Passant Capture
        if (move->isEnPassant) {
            // The captured pawn is on the 'to' square's row, but 'from' square's column
            Bidak emptySquareCapturedPawn;
            initBidak(&emptySquareCapturedPawn, TIDAK_ADA, TANPA_WARNA, -1, -1, -1);
            // Clear the captured pawn's square
            papan->grid[move->from.row][move->to.col] = emptySquareCapturedPawn;
        }

        // Mark the moved piece as having moved
        bidak.hasMoved = 1;

        // Handle Pawn Promotion (type change) - The piece's type is already updated in move->promotionTo
        if (bidak.tipe == PION && (yTujuan == 0 || yTujuan == 7)) {
            // This is where the actual piece type changes on the board
            bidak.tipe = move->promotionTo; 
        }

        // Clear starting position
        Bidak emptySquareFrom; 
        initBidak(&emptySquareFrom, TIDAK_ADA, TANPA_WARNA, xAwal, yAwal, -1);
        papan->grid[yAwal][xAwal] = emptySquareFrom; // Set starting square to empty

        // Place the piece (possibly promoted) at the target position
        setBidakAt(papan, bidak, xTujuan, yTujuan);
    }
}

boolean isEmptyBidak(Papan* papan, int x, int y) {
	return papan->grid[y][x].tipe == TIDAK_ADA; // Access grid as [row][column]
}

//Position findKingPosition(Papan papan, WarnaBidak kingColor) {
//    Position kingPos = {-1, -1}; // Default invalid position
//    int r, c; // Declared loop variables here for C89/ANSI C compatibility
//    for (r = 0; r < UKURAN_PAPAN; r++) {
//        for (c = 0; c < UKURAN_PAPAN; c++) {
//            Bidak b = getBidakAt(papan, c, r);
//            if (b.tipe == RAJA && b.warna == kingColor) {
//                kingPos = (Position){r, c};
//                return kingPos;
//            }
//        }
//    }
//    return kingPos; // Should ideally always find the king
//}
