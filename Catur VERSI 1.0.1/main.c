#include <stdio.h>
#include <stdlib.h>
#include "controller/chessController.h"
#include "home/home.h"
#include "user/account.h" // Tambahkan ini untuk mengakses currentLoggedInAccount

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
    int homeResult = showHomeScreen();

    if (homeResult == HOME_LOGIN) {
        startChess();
    } else if (homeResult == HOME_EXIT) {
        // Bebaskan memori currentLoggedInAccount saat keluar dari home screen
        if (currentLoggedInAccount != NULL) {
            free(currentLoggedInAccount);
            currentLoggedInAccount = NULL;
        }
        return 0; // Keluar dari program
    }
    
    return 0;
}
