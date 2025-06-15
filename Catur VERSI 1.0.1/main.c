#include <stdio.h>
#include <stdlib.h>
#include "controller/chessController.h"
#include "home/home.h" // Tambahkan ini

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
    int homeResult = showHomeScreen(); // Panggil fungsi home screen

    if (homeResult == HOME_LOGIN) {
        startChess(); // Jika login berhasil, lanjutkan ke permainan
    } else if (homeResult == HOME_EXIT) {
        // Dealokasi memori global jika ada (misalnya, jika Anda punya pohon game yang tidak di-free)
        // Saat ini, tidak ada dealokasi eksplisit yang terlihat di sini,
        // tetapi jika ada, ini adalah tempatnya.
        // Misalnya: destroyGameTree(globalTree);
        return 0; // Keluar dari program
    }
    
    // Jika HOME_REGISTER atau pilihan lain yang tidak mengarah ke game,
    // program akan kembali ke loop di showHomeScreen atau keluar.
    // Dalam implementasi saat ini, setelah register, ia kembali ke home screen.

    return 0;
}
