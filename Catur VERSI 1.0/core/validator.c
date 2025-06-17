#include "validator.h"

boolean isPathClear(Papan papan, Position from, Position to) {
    // Menghitung arah pergerakan
    int dx = 0;
    if (to.col > from.col) dx = 1;
    else if (to.col < from.col) dx = -1;

    int dy = 0;
    if (to.row > from.row) dy = 1;
    else if (to.row < from.row) dy = -1;
    
    // Titik awal untuk pengecekan jalur (setelah kotak asal)
    Position current = {from.row + dy, from.col + dx};
    
    // Loop selama belum mencapai kotak tujuan
    while (current.row != to.row || current.col != to.col) {
        // Periksa apakah ada bidak di jalur
        if (getBidakAt(papan, current.col, current.row).id != -1) {
            return false; // Jalur tidak jelas
        }
        // Lanjutkan ke kotak berikutnya di jalur
        current.row += dy;
        current.col += dx;
    }
    return true; // Jalur jelas
}

boolean isValidMove(Papan papan, Move* move, Player* currentPlayer) {
    // Mengambil bidak di posisi asal
    Bidak piece = getBidakAt(papan, move->from.col, move->from.row);

    // 1. Validasi keberadaan bidak dan kepemilikan
    // Jika tidak ada bidak di posisi 'from' atau bidak tersebut bukan milik pemain saat ini
    if (piece.id == -1 || piece.warna != currentPlayer->warna) {
        // printf("DEBUG: Invalid - no piece or not current player's piece (id:%d, warna_piece:%d, warna_player:%d)\n", piece.id, piece.warna, currentPlayer->warna); // Debugging
        return false;
    }

    // Menghitung perbedaan baris dan kolom (delta x dan delta y)
    int dx = move->to.col - move->from.col;
    int dy = move->to.row - move->from.row;
    
    // Mengambil bidak di posisi tujuan
    Bidak target = getBidakAt(papan, move->to.col, move->to.row);
    // Menentukan apakah ini adalah langkah menangkap
    boolean isCapture = (target.id != -1 && target.warna != piece.warna);
    // Menentukan apakah kotak tujuan ditempati oleh bidak sendiri (illegal move)
    boolean isTargetOwnPiece = (target.id != -1 && target.warna == piece.warna);

    // Langkah apapun tidak boleh berakhir di kotak yang ditempati oleh bidak sendiri
    if (isTargetOwnPiece) {
        // printf("DEBUG: Invalid - target square occupied by own piece.\n"); // Debugging
        return false;
    }

    // Logika validasi berdasarkan jenis bidak
    switch (piece.tipe) {
        case PION: {
            int direction = (piece.warna == PUTIH) ? -1 : 1; // Putih bergerak ke atas (-row), Hitam ke bawah (+row)
            int startRow = (piece.warna == PUTIH) ? 6 : 1; // Baris awal pion Putih (indeks 6), Hitam (indeks 1)

            // Langkah maju 1 kotak
            if (dx == 0 && dy == direction && target.id == -1) {
                return true;
            }
            // Langkah maju 2 kotak dari posisi awal
            if (dx == 0 && dy == 2 * direction && move->from.row == startRow &&
                getBidakAt(papan, move->from.col, move->from.row + direction).id == -1 && // Pastikan kotak di depan tidak terhalang
                target.id == -1) {
                return true;
            }
            // Langkah menangkap diagonal
            if (abs(dx) == 1 && dy == direction && isCapture) {
                return true;
            } 
			
            // En Passant (belum diimplementasikan di sini, tambahkan jika perlu)
            break; // Jika tidak ada kondisi pion yang terpenuhi, lanjut ke return false di akhir fungsi
        }
        case BENTENG: // Rook
            return (dx == 0 || dy == 0) && // Bergerak horizontal atau vertikal
                   isPathClear(papan, move->from, move->to); // Jalur harus jelas
        case MENTERI: // Queen
            return (abs(dx) == abs(dy) || dx == 0 || dy == 0) && // Bergerak diagonal, horizontal, atau vertikal
                   isPathClear(papan, move->from, move->to); // Jalur harus jelas
        case GAJAH: // Bishop
            return abs(dx) == abs(dy) && // Bergerak diagonal
                   isPathClear(papan, move->from, move->to); // Jalur harus jelas
        case KUDA: // Knight
            // Gerakan 'L'
            return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2); // Kuda tidak memerlukan isPathClear
        case RAJA: // King
            return abs(dx) <= 1 && abs(dy) <= 1; // Bergerak satu kotak ke segala arah
            // Castling (belum diimplementasikan di sini, tambahkan jika perlu)
    }

    // Jika tidak ada jenis bidak yang cocok atau aturan tidak terpenuhi
    return false;
}

Move* generateAllValidMoves(Papan papan, Player* currentPlayer) {
    Move* moves = (Move*)malloc(sizeof(Move) * MAX_MOVES);
    if (moves == NULL) return NULL;
    
    int count = 0;

	int row, col, toRow, toCol;
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            Bidak piece = getBidakAt(papan, col, row);
            if (piece.id == -1 || piece.warna != currentPlayer->warna) continue;

            for (toRow = 0; toRow < 8; toRow++) {
                for (toCol = 0; toCol < 8; toCol++) {
                    // Pastikan koordinat untuk createMove sesuai (baris, kolom) atau (kol, baris)
                    // Jika Position didefinisikan sebagai {row, col}, maka:
                    Move move;
					createMove(&move, (Position){row, col},
                        	  (Position){toRow, toCol},
                        	  piece.tipe
                    );

                    if (isValidMove(papan, &move, currentPlayer)) {
                        moves[count++] = move;
                        if (count >= MAX_MOVES - 1) {
                            moves[count].bidak = '\0'; // Mark end of moves
                            return moves;
                        }
                    }
                }
            }
        }
    }

    moves[count].bidak = '\0'; // Mark end of moves
    return moves;
}

int countLegalMoves(Papan papan, int baris, int kolom, Player* currentPlayer) {
	int count = 0;
	
	Bidak piece = getBidakAt(papan, baris, kolom);
	
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Move move;
			createMove(&move, (Position){baris, kolom},
                	  (Position){x, y},
                	  piece.tipe
            );
            
            if (isValidMove(papan, &move, currentPlayer)) count++;

		}
	}
	return count;
}
