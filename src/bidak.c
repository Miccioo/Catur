#include "bidak.h"
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

void initBidak(Bidak *bidak, TipeBidak tipe, WarnaBidak warna, int x, int y, int id) {
    bidak->tipe = tipe;
    bidak->warna = warna;
    bidak->x = x;
    bidak->y = y;
    bidak->hasMoved = 0;
    bidak->id = id;
}

void printBidak(Bidak bidak) {
    const char *tipeStr[] = {"Pion", "Benteng", "Kuda", "Gajah", "Menteri", "Raja", "Kosong"};
    const char *warnaStr[] = {"Putih", "Hitam", "Tanpa Warna"};
    
    printf("Bidak %s %s di (%d,%d) ID: %d, Has Moved: %d\n", 
           warnaStr[bidak.warna], 
           tipeStr[bidak.tipe], 
           bidak.x, bidak.y, 
           bidak.id, 
           bidak.hasMoved);
}

void printBidakColor(Bidak b, int row_visual, int cell_x) {
    // Atur background berdasarkan petak
    if (cell_x % 2 == 1) {
        setRgbBackground(119, 146, 86); // petak gelap
    } else {
        setRgbBackground(235, 236, 208); // petak terang
    }

    // Atur warna foreground berdasarkan warna bidak
    if (b.warna == HITAM) {
        setRgbColor(34, 33, 31);
    } else if (b.warna == PUTIH) {
        setRgbColor(249, 249, 249);
    }

    // Cetak visual baris ke-row_visual
    printf("%s", getBidakVisualLine(b.tipe, row_visual));

    // Reset semua
    printf(RESETCOLOR);
}


// Array string untuk setiap baris visual bidak
// Tinggi semua bidak diseragamkan menjadi BIDAK_VISUAL_HEIGHT (15 baris)
// Lebar semua bidak diseragamkan menjadi BIDAK_VISUAL_WIDTH (21 karakter)
const char* bidakVisualsArray[7][BIDAK_VISUAL_HEIGHT] = {
    // PION (Disesuaikan ke 21 lebar)
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
    // BENTENG (Disesuaikan ke 21 lebar)
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
    // KUDA (Disesuaikan ke 21 lebar)
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
    // GAJAH (Disesuaikan ke 21 lebar)
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
    // MENTERI (Disesuaikan ke 21 lebar)
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
     "   (________)        ", 
     "                     "
    },
    // RAJA (Disesuaikan ke 21 lebar)
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
    // TIDAK_ADA (Kotak Kosong) (Disesuaikan ke 21 lebar)
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

// Fungsi untuk mendapatkan string baris visual dari bidak pada indeks tertentu
const char* getBidakVisualLine(TipeBidak tipe, int line_idx) {
    if (line_idx >= 0 && line_idx < BIDAK_VISUAL_HEIGHT) {
        return bidakVisualsArray[tipe][line_idx];
    }
    return "                     "; // Mengembalikan spasi kosong jika indeks di luar batas, 21 spasi
}

// Fungsi tampilkanBidakVisual lama yang hanya untuk menampilkan bidak individual (opsional)
void tampilkanBidakVisual(TipeBidak tipe, WarnaBidak warna) {
    if (warna == HITAM) {
        printf("\033[0;31m");  // Merah untuk hitam
    } else if (warna == PUTIH) {
        printf("\033[0;37m");  // Putih
    }
	
	int i;
    for (i = 0; i < BIDAK_VISUAL_HEIGHT; i++) {
        printf("%s\n", getBidakVisualLine(tipe, i));
    }
    
    printf("\033[0m");  // Reset warna ke default
}
