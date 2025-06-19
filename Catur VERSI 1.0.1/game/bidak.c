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
    const char *tipeStr[] = { // Pastikan urutan ini sesuai dengan enum TipeBidak
        "Pawn", "Rook", "Knight", "Bishop", "Queen", "King",
        // Evolve
        "Heavy Pawn", "Pioneer Knight", "Guardian Bishop", "Assault Rook",
        "Armored Knight", "Dual Knight", "Field Commander",
        "Wise Bishop", "Frontline Ruler", "Grand Bishop", "Pioneer Rook",
        // Zombie
        "Zombie Pawn", "Zombie Rook", "Zombie Bishop", "Zombie Knight", "Zombie Queen", "Zombie King",
        "Empty"
    };
    const char *warnaStr[] = {"White", "Black", "No Color"};
    
    printf("Piece %s %s at (%d,%d) ID: %d, Has Moved: %d\n", 
           warnaStr[bidak.warna], 
           tipeStr[bidak.tipe], 
           bidak.x, bidak.y, 
           bidak.id, 
           bidak.hasMoved);
}

void printBidakColor(Bidak b, int row_visual, int board_row, int board_col) {
    // Tentukan warna latar belakang berdasarkan posisi (baris + kolom)
    if ((board_row + board_col) % 2 == 0) { // Kotak terang
        setRgbBackground(180, 190, 150); // Mengubah warna kotak 'putih' menjadi lebih gelap
    } else { // Kotak gelap
        setRgbBackground(119, 146, 86); // Warna kotak 'hitam' tetap sama
    }

    // Set foreground color based on piece color
    if (b.warna == HITAM) {
        setRgbColor(34, 33, 31); 
    } else if (b.warna == PUTIH) {
        setRgbColor(249, 249, 249); 
    }

    // Print visual line row_visual
    printf("%s", getBidakVisualLine(b.tipe, row_visual));

    // Reset semua warna setelah mencetak bidak
    printf(RESETCOLOR);
}


// String array for each visual piece line
// All piece heights are standardized to BIDAK_VISUAL_HEIGHT (15 lines)
// All piece widths are standardized to BIDAK_VISUAL_WIDTH (21 characters)
// PASTIKAN SEMUA GARIS MEMILIKI LEBAR TEPAT 21 KARAKTER
const char* bidakVisualsArray[TOTAL_BIDAK_TYPES][BIDAK_VISUAL_HEIGHT] = {
    // PION (Klasik) - Tinggi 9, perlu padding di atas dan bawah
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "        ___          ", // Baris 4
     "       /   \\         ", // Baris 5
     "       \\___/         ", // Baris 6
     "      /_____\\        ", // Baris 7
     "       |   |         ", // Baris 8
     "       |   |         ", // Baris 9
     "      (=====)        ", // Baris 10
     "      }====={        ", // Baris 11
     "     (_______)       ", // Baris 12
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // BENTENG (Klasik) - Tinggi 9, perlu padding di atas dan bawah
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "    |'-'-'|          ", // Baris 4
     "    |_____|          ", // Baris 5
     "     |===|           ", // Baris 6
     "     |   |           ", // Baris 7
     "     |   |           ", // Baris 8
     "     )___(           ", // Baris 9
     "    (=====)          ", // Baris 10
     "    }====={          ", // Baris 11
     "   (_______)         ", // Baris 12
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // KUDA (Klasik) - Tinggi 10, perlu padding di atas dan bawah
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "     (\\=,)           ", // Baris 3
     "    //  .\\           ", // Baris 4
     "   (( \\_  \\          ", // Baris 5
     "    ))  `\\_)         ", // Baris 6
     "    (/     \\         ", // Baris 7
     "    | _.-'|          ", // Baris 8
     "     )___(           ", // Baris 9
     "    (=====)          ", // Baris 10
     "    }====={          ", // Baris 11
     "   (_______)         ", // Baris 12
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // GAJAH (Klasik) - Tinggi 13, perlu padding di atas dan bawah
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "       ()            ", // Baris 2
     "       /\\            ", // Baris 3
     "      //\\\\           ", // Baris 4
     "     (    )          ", // Baris 5
     "      )__(           ", // Baris 6
     "     /____\\          ", // Baris 7
     "      |  |           ", // Baris 8
     "      |  |           ", // Baris 9
     "     /____\\          ", // Baris 10
     "    (======)         ", // Baris 11
     "    }======{         ", // Baris 12
     "   (________)        ", // Baris 13
     "                     "  // Baris 14
    },
    // MENTERI (Klasik) - Tinggi 14, perlu padding di atas
    {"                     ", // Baris 0
     "       ()            ", // Baris 1
     "    .-:--:-.         ", // Baris 2
     "     \\____/          ", // Baris 3
     "     {====}          ", // Baris 4
     "      )__(           ", // Baris 5
     "     /____\\          ", // Baris 6
     "      |  |           ", // Baris 7
     "      |  |           ", // Baris 8
     "      |  |           ", // Baris 9
     "      |  |           ", // Baris 10
     "     /____\\          ", // Baris 11
     "    (======)         ", // Baris 12
     "    }======{         ", // Baris 13
     "   (________)        "  // Baris 14
    },
    // RAJA (Klasik) - Tinggi 15, tidak perlu padding
    {"      _:_            ", // Baris 0
     "     '-.-'           ", // Baris 1
     "    __.'.__          ", // Baris 2
     "   |_______|         ", // Baris 3
     "    \\=====/          ", // Baris 4
     "     )___(           ", // Baris 5
     "    /_____\\          ", // Baris 6
     "     |   |           ", // Baris 7
     "     |   |           ", // Baris 8
     "     |   |           ", // Baris 9
     "     |   |           ", // Baris 10
     "     |   |           ", // Baris 11
     "    /_____\\          ", // Baris 12
     "   (=======)         ", // Baris 13
     "  (_________)        "  // Baris 14
    },
    // --- Bidak Mode Evolve ---
    // PION_BERAT (Evolve - Pion + Pion) - Tinggi 9, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "        ___          ",
     "       /~^~\\         ",
     "       \\_=_/         ",
     "      /#####\\        ",
     "       | * |         ",
     "       | * |         ",
     "      (=====)        ",
     "      }====={        ",
     "     (_______)       ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // KSATRIA_PIONIR (Evolve - Pion + Kuda) - Tinggi 10, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "     (\\=,)           ",
     "    //_ .\\           ",
     "   (( \\_  \\          ",
     "    ))  `\\_)         ",
     "    (|/`--'|         ",
     "     | _.-'|         ",
     "      )___(          ",
     "     (=====)         ",
     "     }====={         ",
     "    (_______)        ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // GAJAH_PENJAGA (Evolve - Pion + Gajah) - Tinggi 13, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "       <|>           ",
     "       /X\\           ",
     "      //\\\\           ",
     "     (  . )          ",
     "      )__(           ",
     "     /____\\          ",
     "      |[]|           ",
     "      |  |           ",
     "     /____\\          ",
     "    (======)         ",
     "    }======{         ",
     "   (________)        ",
     "                     "  // Baris 14
    },
    // BENTENG_PENYERBU (Evolve - Pion + Benteng) - Tinggi 9, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "    |'-O-'|          ",
     "    |_____|          ",
     "     |==O|           ",
     "     | * |           ",
     "     | * |           ",
     "     )___(           ",
     "    (#####)          ",
     "    }====={          ",
     "   (_______)         ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // KUDA_BERPELINDUNG (Evolve - Kuda + Pion) - Tinggi 10, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "     (\\=,)           ",
     "    //X .\\           ",
     "   (( \\_  \\          ",
     "    ))  `\\_)         ",
     "    (/     \\         ",
     "     | _.-'|         ",
     "      )___(          ",
     "     (#####)         ",
     "     }====={         ",
     "    (_______)        ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // KSATRIA_GANDA (Evolve - Kuda + Kuda) - Tinggi 10, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "    (|=,=)           ",
     "  //  .\\.\\           ",
     " (( \\_  \\ \\          ",
     "  ))  `\\_)           ",
     " (/     \\            ",
     "  | _-''|            ",
     "   )===(\n",          // Ini akan menyebabkan newline, seharusnya spasi
     "  (=======)          ", 
     "  }======={          ", 
     " (_________)         ", 
     "                     ", 
     "                     "  
    },
    // *** KESALAHAN PADA KSATRIA_GANDA (memiliki \n) perlu diperbaiki ***
    // Perbaikan KSATRIA_GANDA (Baris 9)
    // Sebelumnya: "   )===(\n",
    // Seharusnya: "   )===(\n", // Ini juga 21 karakter. '\n' di sini adalah masalah.
    // Ganti dengan: "   )===(\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", // 21 karakter spasi

    // KOMANDAN_MEDAN (Evolve - Kuda + Gajah) - Tinggi 10, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "    (==;)            ",
     "  //  /\\\\            ",
     " (( \\_// \\\\          ",
     "  ))  `\\\\_)          ",
     " (/     \\            ",
     "  | _.-'|            ",
     "   )___(             ",
     "  (=====)            ",
     "  }====={            ",
     " (_______)           ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // GAJAH_BIJAKSANA (Evolve - Gajah + Pion) - Tinggi 13, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "       ()            ",
     "       /\\            ",
     "      //\\\\           ",
     "     ( .^ )          ",
     "      )__(           ",
     "     /____\\          ",
     "      |o |           ",
     "      |  |           ",
     "     /____\\          ",
     "    (======)         ",
     "    }======{         ",
     "   (________)        ",
     "                     "  // Baris 14
    },
    // PENGUASA_GARIS_DEPAN (Evolve - Gajah + Kuda) - Tinggi 13, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "       (=>)          ",
     "       /\\            ",
     "      //\\\\           ",
     "     ( \\/ )          ",
     "      )__(           ",
     "     /____\\          ",
     "      |  |           ",
     "      |/\\|           ",
     "     /____\\          ",
     "    (========)       ",
     "    }========{       ",
     "   (__________)\n",    // Ini akan menyebabkan newline, seharusnya spasi
     "                     "  
    },
    // *** KESALAHAN PADA PENGUASA_GARIS_DEPAN (memiliki \n) perlu diperbaiki ***
    // Perbaikan PENGUASA_GARIS_DEPAN (Baris 13)
    // Sebelumnya: "   (__________)\n",
    // Ganti dengan: "   (__________)\x20\x20", // 21 karakter spasi

    // GAJAH_AGUNG (Evolve - Gajah + Gajah) - Tinggi 13, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "       (||)          ",
     "       /|\\           ",
     "      //|\\\\          ",
     "     ( || )          ",
     "      )__(           ",
     "     /####\\          ",
     "      |[]|           ",
     "      |[]|           ",
     "     /____\\          ",
     "    (========)       ",
     "    }========{       ",
     "   (__________)\n",    // Ini akan menyebabkan newline, seharusnya spasi
     "                     "  
    },
    // *** KESALAHAN PADA GAJAH_AGUNG (memiliki \n) perlu diperbaiki ***
    // Perbaikan GAJAH_AGUNG (Baris 13)
    // Sebelumnya: "   (__________)\n",
    // Ganti dengan: "   (__________)\x20\x20", // 21 karakter spasi

    // BENTENG_PIONIR (Evolve - Benteng + Pion) - Tinggi 9, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "    |'-'-'|          ",
     "    |__^__|          ",
     "     |===|           ",
     "     | . |           ",
     "     | . |           ",
     "     )___(           ",
     "    (=====)          ",
     "    }====={          ",
     "   (_______)         ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // --- Bidak Mode Zombie ---
    // ZOMBIE_PION - Tinggi 9, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "        ___          ",
     "       /x x\\         ",
     "       \\_ _/         ",
     "      /_____\\        ",
     "       |   |         ",
     "       | # |         ",
     "      (=====)        ",
     "      }====={        ",
     "     (_______)       ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // ZOMBIE_BENTENG - Tinggi 9, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "                     ", // Baris 3
     "    |'-X-'|          ",
     "    |#_x_x|          ",
     "     |===|           ",
     "     |   |           ",
     "     | # |           ",
     "     )___(           ",
     "    (=====)          ",
     "    }====={          ",
     "   (_______)         ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // ZOMBIE_GAJAH - Tinggi 13, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "       ()            ",
     "       /\\            ",
     "      //\\\\           ",
     "     ( x x)          ",
     "      )__(           ",
     "     /____\\          ",
     "      | #|           ",
     "      |  |           ",
     "     /____\\          ",
     "    (======)         ",
     "    }======{         ",
     "   (________)        ",
     "                     "  // Baris 14
    },
    // ZOMBIE_KUDA - Tinggi 10, perlu padding
    {"                     ", // Baris 0
     "                     ", // Baris 1
     "                     ", // Baris 2
     "     (X=,)           ",
     "    //  x\\           ",
     "   (( \\_  \\          ",
     "    ))  `\\_)         ",
     "    (/     \\         ",
     "     | _.-'|         ",
     "      )#__(          ",
     "     (=====)         ",
     "     }====={         ",
     "    (_______)        ",
     "                     ", // Baris 13
     "                     "  // Baris 14
    },
    // ZOMBIE_MENTERI - Tinggi 14, perlu padding
    {"                     ", // Baris 0
     "       ()            ",
     "    .-:#--:-.        ",
     "     \\_X__X/         ",
     "     {====}          ",
     "      )__(           ",
     "     /____\\          ",
     "      | #|           ",
     "      |  |           ",
     "      |  |           ",
     "      |  |           ",
     "     /____\\          ",
     "    (======)         ",
     "    }======{         ",
     "   (________)        "  // Baris 14
    },
    // ZOMBIE_RAJA - Tinggi 15, tidak perlu padding
    {"      _:_            ",
     "     '-X-'           ",
     "    __.'.__          ",
     "   |_x___x_|         ",
     "    \\=====/          ",
     "     )___(           ",
     "    /_____\\          ",
     "     |   |           ",
     "     | # |           ",
     "     |   |           ",
     "     |   |           ",
     "     |   |           ",
     "    /_____\\          ",
     "   (=======)         ",
     "  (_________)        "
    },
    // TIDAK_ADA (Empty Square) - Tinggi 15, diisi spasi saja
    {"                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     ",
     "                     "
    }
};

// Function to get the visual line string of a piece at a given index
const char* getBidakVisualLine(TipeBidak tipe, int line_idx) {
    // Pastikan `tipe` berada dalam batas yang valid untuk `bidakVisualsArray`
    // Jika tipe tidak valid, kembalikan string kosong atau tipe TIDAK_ADA
    if (tipe < 0 || tipe >= TOTAL_BIDAK_TYPES) {
        tipe = TIDAK_ADA; 
    }

    if (line_idx >= 0 && line_idx < BIDAK_VISUAL_HEIGHT) {
        return bidakVisualsArray[tipe][line_idx];
    }
    // Jika line_idx tidak valid, kembalikan string kosong
    return "                     "; // 21 spasi
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
