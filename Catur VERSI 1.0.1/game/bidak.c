#include "bidak.h"
#include <stdio.h>
#include <string.h>

void initBidak(Bidak *bidak, TipeBidak tipe, WarnaBidak warna, int x, int y, int id) {
    bidak->tipe = tipe;
    bidak->warna = warna;
    bidak->x = x;
    bidak->y = y;
    bidak->hasMoved = 0;
    bidak->id = id;
}

void printBidak(Bidak bidak) {
    const char *tipeStr[] = {"Pawn", "Rook", "Knight", "Bishop", "Queen", "King", "Empty"};
    const char *warnaStr[] = {"White", "Black", "No Color"};
    
    printf("Piece %s %s at (%d,%d) ID: %d, Has Moved: %d\n", 
           warnaStr[bidak.warna], 
           tipeStr[bidak.tipe], 
           bidak.x, bidak.y, 
           bidak.id, 
           bidak.hasMoved);
}

void printBidakColor(Bidak b, int row_visual, int cell_x) {
    // Set background based on square
    if (cell_x % 2 == 1) {
        setRgbBackground(119, 146, 86); // Dark square
    } else {
        setRgbBackground(235, 236, 208); // Light square
    }

    // Set foreground color based on piece color
    if (b.warna == HITAM) {
        setRgbColor(34, 33, 31);
    } else if (b.warna == PUTIH) {
        setRgbColor(249, 249, 249);
    }

    // Print visual line row_visual
    printf("%s", getBidakVisualLine(b.tipe, row_visual));

    // Reset all
    printf(RESETCOLOR);
}


// String array for each visual piece line
// All piece heights are standardized to BIDAK_VISUAL_HEIGHT (15 lines)
// All piece widths are standardized to BIDAK_VISUAL_WIDTH (21 characters)
const char* bidakVisualsArray[7][BIDAK_VISUAL_HEIGHT] = {
	
    // PION (Adjusted to 21 width)
    {"                     ", // Padding 1
     "                     ", // Padding 2
     "                     ", // Padding 3
     "                     ", // Padding 4
     "        ___          ",
     "       /   \\         ",
     "       \\___/         ",
     "      /_____\\        ",
     "       |   |         ",
     "       |   |         ",
     "      (=====)        ",
     "      }====={        ",
     "     (_______)       ",
     "                     ", // Padding 5
     "                     "  // Padding 6
    },
    // BENTENG (Adjusted to 21 width)
    {"                     ", // Padding 1
     "                     ", // Padding 2
     "                     ", // Padding 3
     "                     ", // Padding 4
     "    |'-'-'|          ",
     "    |_____|          ",
     "     |===|           ",
     "     |   |           ",
     "     |   |           ",
     "     )___(           ",
     "    (=====)          ",
     "    }====={          ",
     "   (_______)         ",
     "                     ", // Padding 5
     "                     "  // Padding 6
    },
    // KUDA (Adjusted to 21 width)
    {"                     ", // Padding 1
     "                     ", // Padding 2
     "                     ", // Padding 3
     "     (\\=,)           ",
     "    //  .\\           ",
     "   (( \\_  \\          ",
     "    ))  `\\_)         ",
     "    (/     \\         ",
     "    | _.-'|          ",
     "     )___(           ",
     "    (=====)          ",
     "    }====={          ",
     "   (_______)         ",
     "                     ", // Padding 4
     "                     "  // Padding 5
    },
    // GAJAH (Adjusted to 21 width)
    {"                     ", // Padding 1
     "                     ", // Padding 2
     "       ()            ",
     "       /\\            ",
     "      //\\\\           ",
     "     (    )          ",
     "      )__(           ",
     "     /____\\          ",
     "      |  |           ",
     "      |  |           ",
     "     /____\\          ",
     "    (======)         ",
     "    }======{         ",
     "   (________)        ",
     "                     "  // Padding 3
    },
    // MENTERI (Adjusted to 21 width) - REMOVED EXCESS ELEMENT HERE
    {"                     ", // Padding 1
     "       ()            ",
     "    .-:--:-.         ",
     "     \\____/          ",
     "     {====}          ",
     "      )__(           ",
     "     /____\\          ",
     "      |  |           ",
     "      |  |           ",
     "      |  |           ",
     "      |  |           ",
     "     /____\\          ",
     "    (======)         ",
     "    }======{         ",
     "   (________)        " // Removed the extra padding line here.
    },
    // RAJA (Adjusted to 21 width)
    {"      _:_            ",
     "     '-.-'           ",
     "    __.'.__          ",
     "   |_______|         ",
     "    \\=====/          ",
     "     )___(           ",
     "    /_____\\          ",
     "     |   |           ",
     "     |   |           ",
     "     |   |           ",
     "     |   |           ",
     "     |   |           ",
     "    /_____\\          ",
     "   (=======)         ",
     "  (_________)        " 
    },
    // TIDAK_ADA (Empty Square) (Adjusted to 21 width)
    {"                     ", // Padding 1
     "                     ", // Padding 2
     "                     ", // Padding 3
     "                     ", // Padding 4
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ", // Padding 5
     "                     "  // Padding 6
    }
};

// Function to get the visual line string of a piece at a given index
const char* getBidakVisualLine(TipeBidak tipe, int line_idx) {
    if (line_idx >= 0 && line_idx < BIDAK_VISUAL_HEIGHT) {
        return bidakVisualsArray[tipe][line_idx];
    }
    return "                     "; // Return empty spaces if index out of bounds, 21 spaces
}

// Old function to display individual piece visual (optional)
void tampilkanBidakVisual(TipeBidak tipe, WarnaBidak warna) {
    if (warna == HITAM) {
        printf("\033[0;31m");  // Red for black
    } else if (warna == PUTIH) {
        printf("\033[0;37m");  // White
    }
	
	int i;
    for (i = 0; i < BIDAK_VISUAL_HEIGHT; i++) {
        printf("%s\n", getBidakVisualLine(tipe, i));
    }
    
    printf("\033[0m");  // Reset color to default
}
