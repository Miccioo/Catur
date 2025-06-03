#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define BOARD_SIZE 8
#define MAX_MOVES 28
#define MAX_DEPTH 4  // Depth for AI search

// Enum untuk jenis bidak
typedef enum {
    PAWN,       // Pion
    ROOK,       // Benteng
    KNIGHT,     // Kuda
    BISHOP,     // Gajah
    QUEEN,      // Menteri
    KING,       // Raja
    EMPTY       // Kotak kosong
} PieceType;

// Enum untuk warna bidak
typedef enum {
    WHITE,
    BLACK,
    NO_COLOR
} PieceColor;

// Struktur untuk posisi bidak
typedef struct {
    int x;  // Kolom (0-7)
    int y;  // Baris (0-7)
} Position;

// Struktur node untuk linked list bidak
typedef struct ChessPieceNode {
    PieceType type;
    PieceColor color;
    Position position;
    bool hasMoved;
    struct ChessPieceNode* next;
} ChessPieceNode;

// Struktur untuk linked list bidak
typedef struct {
    ChessPieceNode* head;
    int count;
} ChessPieceList;

// Struktur untuk menyimpan kemungkinan gerakan
typedef struct {
    Position moves[MAX_MOVES];
    int count;
} PossibleMoves;

// Struktur untuk stack (undo/redo)
typedef struct GameState {
    ChessPieceList whitePieces;
    ChessPieceList blackPieces;
    PieceColor currentPlayer;
    struct GameState* next;
} GameState;

typedef struct {
    GameState* top;
    int count;
} GameStateStack;

// Struktur untuk node pohon minimax
typedef struct MinimaxNode {
    ChessPieceList whitePieces;
    ChessPieceList blackPieces;
    Position moveFrom;
    Position moveTo;
    int evaluation;
    struct MinimaxNode* children;
    struct MinimaxNode* nextSibling;
} MinimaxNode;

// Variabel global
PossibleMoves possibleMoves;
bool showHints = true;
GameStateStack undoStack;
GameStateStack redoStack;
bool aiEnabled = false;
PieceColor aiColor = BLACK;  // Default AI plays as Black

// Fungsi bantu
void initChessPieceList(ChessPieceList* list) {
    list->head = NULL;
    list->count = 0;
}

ChessPieceNode* createChessPieceNode(PieceType type, PieceColor color, Position pos) {
    ChessPieceNode* newNode = (ChessPieceNode*)malloc(sizeof(ChessPieceNode));
    newNode->type = type;
    newNode->color = color;
    newNode->position = pos;
    newNode->hasMoved = false;
    newNode->next = NULL;
    return newNode;
}

void addChessPiece(ChessPieceList* list, PieceType type, PieceColor color, Position pos) {
    ChessPieceNode* newNode = createChessPieceNode(type, color, pos);
    
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        ChessPieceNode* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->count++;
}

ChessPieceNode* findPieceAtPosition(ChessPieceList* list, Position pos) {
    ChessPieceNode* current = list->head;
    while (current != NULL) {
        if (current->position.x == pos.x && current->position.y == pos.y) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool movePiece(ChessPieceList* list, Position from, Position to) {
    ChessPieceNode* piece = findPieceAtPosition(list, from);
    if (piece == NULL) return false;

    piece->position = to;
    piece->hasMoved = true;
    return true;
}

bool removePieceAtPosition(ChessPieceList* list, Position pos) {
    if (list->head == NULL) return false;

    ChessPieceNode* current = list->head;
    ChessPieceNode* prev = NULL;

    while (current != NULL && 
          !(current->position.x == pos.x && current->position.y == pos.y)) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return false;

    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    list->count--;
    return true;
}

void clearChessPieceList(ChessPieceList* list) {
    ChessPieceNode* current = list->head;
    while (current != NULL) {
        ChessPieceNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->count = 0;
}

// Fungsi untuk stack (undo/redo)
void initStack(GameStateStack* stack) {
    stack->top = NULL;
    stack->count = 0;
}

void pushState(GameStateStack* stack, ChessPieceList* whitePieces, ChessPieceList* blackPieces, PieceColor currentPlayer) {
    GameState* newState = (GameState*)malloc(sizeof(GameState));
    initChessPieceList(&newState->whitePieces);
    initChessPieceList(&newState->blackPieces);
    
    // Salin semua bidak putih
    ChessPieceNode* current = whitePieces->head;
    while (current != NULL) {
        addChessPiece(&newState->whitePieces, current->type, current->color, current->position);
        current = current->next;
    }
    
    // Salin semua bidak hitam
    current = blackPieces->head;
    while (current != NULL) {
        addChessPiece(&newState->blackPieces, current->type, current->color, current->position);
        current = current->next;
    }
    
    newState->currentPlayer = currentPlayer;
    newState->next = stack->top;
    stack->top = newState;
    stack->count++;
}

bool popState(GameStateStack* stack, ChessPieceList* whitePieces, ChessPieceList* blackPieces, PieceColor* currentPlayer) {
    if (stack->top == NULL) return false;
    
    GameState* topState = stack->top;
    
    // Hapus state saat ini
    clearChessPieceList(whitePieces);
    clearChessPieceList(blackPieces);
    
    // Salin kembali state sebelumnya
    ChessPieceNode* current = topState->whitePieces.head;
    while (current != NULL) {
        addChessPiece(whitePieces, current->type, current->color, current->position);
        current = current->next;
    }
    
    current = topState->blackPieces.head;
    while (current != NULL) {
        addChessPiece(blackPieces, current->type, current->color, current->position);
        current = current->next;
    }
    
    *currentPlayer = topState->currentPlayer;
    
    // Hapus dari stack
    stack->top = topState->next;
    clearChessPieceList(&topState->whitePieces);
    clearChessPieceList(&topState->blackPieces);
    free(topState);
    stack->count--;
    
    return true;
}

void clearStack(GameStateStack* stack) {
    while (stack->top != NULL) {
        GameState* temp = stack->top;
        stack->top = temp->next;
        clearChessPieceList(&temp->whitePieces);
        clearChessPieceList(&temp->blackPieces);
        free(temp);
    }
    stack->count = 0;
}

// Fungsi untuk menentukan gerakan yang mungkin
PossibleMoves getPossibleMoves(ChessPieceNode* piece, ChessPieceList* whitePieces, ChessPieceList* blackPieces) {
    PossibleMoves moves;
    moves.count = 0;
    int dx, i, dy;
    int newX, newY;
    int knightMoves[8][2] = {{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
    Position pos, target;

    if (piece == NULL) return moves;

    ChessPieceList* allyPieces = (piece->color == WHITE) ? whitePieces : blackPieces;
    ChessPieceList* enemyPieces = (piece->color == WHITE) ? blackPieces : whitePieces;

    int x = piece->position.x;
    int y = piece->position.y;
    int direction = (piece->color == WHITE) ? 1 : -1;
    
    switch (piece->type) {
        case PAWN:
            // Gerakan maju
            if (y + direction >= 0 && y + direction < BOARD_SIZE && 
                findPieceAtPosition(allyPieces, (Position){x, y + direction}) == NULL &&
                findPieceAtPosition(enemyPieces, (Position){x, y + direction}) == NULL) {
                moves.moves[moves.count++] = (Position){x, y + direction};
                
                if (!piece->hasMoved && y + 2*direction >= 0 && y + 2*direction < BOARD_SIZE &&
                    findPieceAtPosition(allyPieces, (Position){x, y + 2*direction}) == NULL &&
                    findPieceAtPosition(enemyPieces, (Position){x, y + 2*direction}) == NULL) {
                    moves.moves[moves.count++] = (Position){x, y + 2*direction};
                }
            }
            
            // Gerakan makan diagonal
            for (dx = -1; dx <= 1; dx += 2) {
                if (x + dx >= 0 && x + dx < BOARD_SIZE && y + direction >= 0 && y + direction < BOARD_SIZE) {
                    target.x = x + dx;
                    target.y = y + direction;
                    if (findPieceAtPosition(enemyPieces, target) != NULL) {
                        moves.moves[moves.count++] = target;
                    }
                }
            }
            break;
            
        case ROOK:
            // Gerakan vertikal ke atas
            for (i = y - 1; i >= 0; i--) {
                pos.x = x;
                pos.y = i;
                if (findPieceAtPosition(allyPieces, pos) != NULL) break;
                moves.moves[moves.count++] = pos;
                if (findPieceAtPosition(enemyPieces, pos) != NULL) break;
            }
            // Gerakan vertikal ke bawah
            for (i = y + 1; i < BOARD_SIZE; i++) {
                pos.x = x;
                pos.y = i;
                if (findPieceAtPosition(allyPieces, pos) != NULL) break;
                moves.moves[moves.count++] = pos;
                if (findPieceAtPosition(enemyPieces, pos) != NULL) break;
            }
            // Gerakan horizontal ke kiri
            for (i = x - 1; i >= 0; i--) {
                pos.x = i;
                pos.y = y;
                if (findPieceAtPosition(allyPieces, pos) != NULL) break;
                moves.moves[moves.count++] = pos;
                if (findPieceAtPosition(enemyPieces, pos) != NULL) break;
            }
            // Gerakan horizontal ke kanan
            for (i = x + 1; i < BOARD_SIZE; i++) {
                pos.x = i;
                pos.y = y;
                if (findPieceAtPosition(allyPieces, pos) != NULL) break;
                moves.moves[moves.count++] = pos;
                if (findPieceAtPosition(enemyPieces, pos) != NULL) break;
            }
            break;
            
        case KNIGHT:
            // Semua kemungkinan gerakan L
            for (i = 0; i < 8; i++) {
                newX = x + knightMoves[i][0];
                newY = y + knightMoves[i][1];
                if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE) {
                    pos.x = newX;
                    pos.y = newY;
                    if (findPieceAtPosition(allyPieces, pos) == NULL) {
                        moves.moves[moves.count++] = pos;
                    }
                }
            }
            break;
            
        case BISHOP:
            // Gerakan diagonal
            for (dx = -1; dx <= 1; dx += 2) {
                for (dy = -1; dy <= 1; dy += 2) {
                    for (i = 1; i < BOARD_SIZE; i++) {
                        newX = x + i*dx;
                        newY = y + i*dy;
                        if (newX < 0 || newX >= BOARD_SIZE || newY < 0 || newY >= BOARD_SIZE) break;
                        
                        pos.x = newX;
                        pos.y = newY;
                        if (findPieceAtPosition(allyPieces, pos) != NULL) break;
                        
                        moves.moves[moves.count++] = pos;
                        if (findPieceAtPosition(enemyPieces, pos) != NULL) break;
                    }
                }
            }
            break;
            
        case QUEEN:
            // Kombinasi gerakan benteng dan gajah
            // Gerakan vertikal dan horizontal (seperti benteng)
            for (i = 0; i < BOARD_SIZE; i++) {
                if (i != y) {
                    pos.x = x;
                    pos.y = i;
                    if (findPieceAtPosition(allyPieces, pos) == NULL) {
                        moves.moves[moves.count++] = pos;
                    }
                }
                if (i != x) {
                    pos.x = i;
                    pos.y = y;
                    if (findPieceAtPosition(allyPieces, pos) == NULL) {
                        moves.moves[moves.count++] = pos;
                    }
                }
            }
            // Gerakan diagonal (seperti gajah)
            for (dx = -1; dx <= 1; dx += 2) {
                for (dy = -1; dy <= 1; dy += 2) {
                    for (i = 1; i < BOARD_SIZE; i++) {
                        newX = x + i*dx;
                        newY = y + i*dy;
                        if (newX < 0 || newX >= BOARD_SIZE || newY < 0 || newY >= BOARD_SIZE) break;
                        
                        pos.x = newX;
                        pos.y = newY;
                        if (findPieceAtPosition(allyPieces, pos) != NULL) break;
                        
                        moves.moves[moves.count++] = pos;
                        if (findPieceAtPosition(enemyPieces, pos) != NULL) break;
                    }
                }
            }
            break;
            
        case KING:
            // Gerakan satu langkah ke segala arah
            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    
                    newX = x + dx;
                    newY = y + dy;
                    if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE) {
                        pos.x = newX;
                        pos.y = newY;
                        if (findPieceAtPosition(allyPieces, pos) == NULL) {
                            moves.moves[moves.count++] = pos;
                        }
                    }
                }
            }
            break;
            
        default:
            break;
    }

    return moves;
}

// Fungsi untuk menampilkan papan catur
char getPieceSymbol(PieceType type, PieceColor color) {
    char symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K'};
    if (type == EMPTY) return ' ';
    char symbol = symbols[type];
    return (color == WHITE) ? symbol : tolower(symbol);
}

void displayHintTable(PossibleMoves moves) {
    int i;
    printf("\n+---------------------+");
    printf("\n|   Possible Moves    |");
    printf("\n+---------------------+");
    
    if (moves.count == 0) {
        printf("\n| No possible moves  |");
    } else {
        for (i = 0; i < moves.count && i < 10; i++) { // Batasi 10 gerakan yang ditampilkan
            char col = 'a' + moves.moves[i].x;
            int row = 8 - moves.moves[i].y;
            printf("\n| %c%d%18s", col, row, "|");
        }
        if (moves.count > 10) {
            printf("\n| +%d more%12s", moves.count - 10, "|");
        }
    }
    
    printf("\n+---------------------+\n");
}

void displayChessBoard(ChessPieceList* whitePieces, ChessPieceList* blackPieces, Position selected) {
    int y, x;
    printf("\n   a b c d e f g h\n");
    printf("  +---------------+\n");
    
    for (y = 0; y < BOARD_SIZE; y++) {
        printf("%d |", 8 - y);
        for (x = 0; x < BOARD_SIZE; x++) {
            Position pos = {x, y};
            ChessPieceNode* white = findPieceAtPosition(whitePieces, pos);
            ChessPieceNode* black = findPieceAtPosition(blackPieces, pos);
            
            // Tandai posisi yang dipilih
            if (selected.x == x && selected.y == y) {
                printf("[");
            } else {
                printf(" ");
            }
            
            if (white) {
                printf("%c", getPieceSymbol(white->type, WHITE));
            } else if (black) {
                printf("%c", getPieceSymbol(black->type, BLACK));
            } else {
                printf(" ");
            }
            
            if (selected.x == x && selected.y == y) {
                printf("]");
            } else {
                printf(" ");
            }
        }
        printf("| %d\n", 8 - y);
    }
    
    printf("  +---------------+\n");
    printf("   a b c d e f g h\n");
    
    // Tampilkan petunjuk jika ada bidak yang dipilih dan showHints aktif
    if (showHints && selected.x != -1 && selected.y != -1) {
        ChessPieceNode* selectedPiece = findPieceAtPosition(whitePieces, selected);
        if (selectedPiece == NULL) {
            selectedPiece = findPieceAtPosition(blackPieces, selected);
        }
        
        if (selectedPiece) {
            PossibleMoves moves = getPossibleMoves(selectedPiece, whitePieces, blackPieces);
            displayHintTable(moves);
        }
    }
}

// Fungsi untuk inisialisasi papan catur
void initializeChessBoard(ChessPieceList* whitePieces, ChessPieceList* blackPieces) {
    int i;
    // Inisialisasi pion
    for (i = 0; i < BOARD_SIZE; i++) {
        addChessPiece(whitePieces, PAWN, WHITE, (Position){i, 1});
        addChessPiece(blackPieces, PAWN, BLACK, (Position){i, 6});
    }
    
    // Inisialisasi bidak putih
    addChessPiece(whitePieces, ROOK, WHITE, (Position){0, 0});
    addChessPiece(whitePieces, KNIGHT, WHITE, (Position){1, 0});
    addChessPiece(whitePieces, BISHOP, WHITE, (Position){2, 0});
    addChessPiece(whitePieces, QUEEN, WHITE, (Position){3, 0});
    addChessPiece(whitePieces, KING, WHITE, (Position){4, 0});
    addChessPiece(whitePieces, BISHOP, WHITE, (Position){5, 0});
    addChessPiece(whitePieces, KNIGHT, WHITE, (Position){6, 0});
    addChessPiece(whitePieces, ROOK, WHITE, (Position){7, 0});
    
    // Inisialisasi bidak hitam
    addChessPiece(blackPieces, ROOK, BLACK, (Position){0, 7});
    addChessPiece(blackPieces, KNIGHT, BLACK, (Position){1, 7});
    addChessPiece(blackPieces, BISHOP, BLACK, (Position){2, 7});
    addChessPiece(blackPieces, QUEEN, BLACK, (Position){3, 7});
    addChessPiece(blackPieces, KING, BLACK, (Position){4, 7});
    addChessPiece(blackPieces, BISHOP, BLACK, (Position){5, 7});
    addChessPiece(blackPieces, KNIGHT, BLACK, (Position){6, 7});
    addChessPiece(blackPieces, ROOK, BLACK, (Position){7, 7});
}

// Fungsi untuk AI (Minimax dengan Alpha-Beta Pruning)

// Fungsi evaluasi sederhana
int evaluateBoard(ChessPieceList* whitePieces, ChessPieceList* blackPieces) {
    int score = 0;
    int pieceValues[] = {1, 5, 3, 3, 9, 100}; // Nilai untuk PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
    
    // Hitung nilai untuk bidak putih
    ChessPieceNode* current = whitePieces->head;
    while (current != NULL) {
        score += pieceValues[current->type];
        current = current->next;
    }
    
    // Hitung nilai untuk bidak hitam
    current = blackPieces->head;
    while (current != NULL) {
        score -= pieceValues[current->type];
        current = current->next;
    }
    
    return score;
}

// Fungsi untuk menyalin state papan
void copyBoardState(ChessPieceList* srcWhite, ChessPieceList* srcBlack, 
                    ChessPieceList* destWhite, ChessPieceList* destBlack) {
    clearChessPieceList(destWhite);
    clearChessPieceList(destBlack);
    
    ChessPieceNode* current = srcWhite->head;
    while (current != NULL) {
        addChessPiece(destWhite, current->type, current->color, current->position);
        current = current->next;
    }
    
    current = srcBlack->head;
    while (current != NULL) {
        addChessPiece(destBlack, current->type, current->color, current->position);
        current = current->next;
    }
}

// Fungsi untuk membuat node baru dalam pohon minimax
MinimaxNode* createMinimaxNode(ChessPieceList* whitePieces, ChessPieceList* blackPieces, 
                               Position from, Position to, int eval) {
    MinimaxNode* node = (MinimaxNode*)malloc(sizeof(MinimaxNode));
    initChessPieceList(&node->whitePieces);
    initChessPieceList(&node->blackPieces);
    copyBoardState(whitePieces, blackPieces, &node->whitePieces, &node->blackPieces);
    node->moveFrom = from;
    node->moveTo = to;
    node->evaluation = eval;
    node->children = NULL;
    node->nextSibling = NULL;
    return node;
}

// Fungsi untuk menambahkan anak ke node minimax
void addMinimaxChild(MinimaxNode* parent, MinimaxNode* child) {
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        MinimaxNode* current = parent->children;
        while (current->nextSibling != NULL) {
            current = current->nextSibling;
        }
        current->nextSibling = child;
    }
}

// Fungsi untuk menghapus pohon minimax
void freeMinimaxTree(MinimaxNode* root) {
    if (root == NULL) return;
    
    MinimaxNode* child = root->children;
    while (child != NULL) {
        MinimaxNode* next = child->nextSibling;
        freeMinimaxTree(child);
        child = next;
    }
    
    clearChessPieceList(&root->whitePieces);
    clearChessPieceList(&root->blackPieces);
    free(root);
}

// Fungsi minimax dengan alpha-beta pruning
int minimax(MinimaxNode* node, int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0) {
        return evaluateBoard(&node->whitePieces, &node->blackPieces);
    }
    
    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        int i;
        // Generate all possible moves for current player (WHITE)
        ChessPieceNode* current = node->whitePieces.head;
        while (current != NULL) {
            PossibleMoves moves = getPossibleMoves(current, &node->whitePieces, &node->blackPieces);
            
            for (i = 0; i < moves.count; i++) {
                // Create new node for this move
                MinimaxNode* child = createMinimaxNode(&node->whitePieces, &node->blackPieces, 
                                                     current->position, moves.moves[i], 0);
                
                // Apply the move
                movePiece(&child->whitePieces, current->position, moves.moves[i]);
                
                // Check if capture occurred
                if (findPieceAtPosition(&child->blackPieces, moves.moves[i])) {
                    removePieceAtPosition(&child->blackPieces, moves.moves[i]);
                }
                
                // Evaluate this move
                int eval = minimax(child, depth - 1, false, alpha, beta);
                child->evaluation = eval;
                addMinimaxChild(node, child);
                
                maxEval = (eval > maxEval) ? eval : maxEval;
                alpha = (alpha > maxEval) ? alpha : maxEval;
                
                if (beta <= alpha) {
                    // Beta cutoff
                    return maxEval;
                }
            }
            
            current = current->next;
        }
        
        return maxEval;
    } else {
        int minEval = INT_MAX;
        int i;
        // Generate all possible moves for current player (BLACK)
        ChessPieceNode* current = node->blackPieces.head;
        while (current != NULL) {
            PossibleMoves moves = getPossibleMoves(current, &node->whitePieces, &node->blackPieces);
            
            for (i = 0; i < moves.count; i++) {
                // Create new node for this move
                MinimaxNode* child = createMinimaxNode(&node->whitePieces, &node->blackPieces, 
                                                     current->position, moves.moves[i], 0);
                
                // Apply the move
                movePiece(&child->blackPieces, current->position, moves.moves[i]);
                
                // Check if capture occurred
                if (findPieceAtPosition(&child->whitePieces, moves.moves[i])) {
                    removePieceAtPosition(&child->whitePieces, moves.moves[i]);
                }
                
                // Evaluate this move
                int eval = minimax(child, depth - 1, true, alpha, beta);
                child->evaluation = eval;
                addMinimaxChild(node, child);
                
                minEval = (eval < minEval) ? eval : minEval;
                beta = (beta < minEval) ? beta : minEval;
                
                if (beta <= alpha) {
                    // Alpha cutoff
                    return minEval;
                }
            }
            
            current = current->next;
        }
        
        return minEval;
    }
}

// Fungsi untuk mendapatkan gerakan terbaik dari AI
void getBestAIMove(ChessPieceList* whitePieces, ChessPieceList* blackPieces, 
                   Position* from, Position* to) {
    MinimaxNode* root = createMinimaxNode(whitePieces, blackPieces, (Position){-1,-1}, (Position){-1,-1}, 0);
    
    int bestEval = INT_MIN;
    Position bestFrom = {-1, -1};
    Position bestTo = {-1, -1};
    
    // Generate all possible moves for AI (BLACK)
    ChessPieceNode* current = blackPieces->head;
    while (current != NULL) {
        PossibleMoves moves = getPossibleMoves(current, whitePieces, blackPieces);
        int i;
        for (i = 0; i < moves.count; i++) {
            // Create new node for this move
            MinimaxNode* child = createMinimaxNode(whitePieces, blackPieces, 
                                                 current->position, moves.moves[i], 0);
            
            // Apply the move
            movePiece(&child->blackPieces, current->position, moves.moves[i]);
            
            // Check if capture occurred
            if (findPieceAtPosition(&child->whitePieces, moves.moves[i])) {
                removePieceAtPosition(&child->whitePieces, moves.moves[i]);
            }
            
            // Evaluate this move
            int eval = minimax(child, MAX_DEPTH - 1, true, INT_MIN, INT_MAX);
            child->evaluation = eval;
            addMinimaxChild(root, child);
            
            if (eval > bestEval) {
                bestEval = eval;
                bestFrom = current->position;
                bestTo = moves.moves[i];
            }
        }
        
        current = current->next;
    }
    
    *from = bestFrom;
    *to = bestTo;
    
    freeMinimaxTree(root);
}

// Fungsi untuk melakukan gerakan AI
void makeAIMove(ChessPieceList* whitePieces, ChessPieceList* blackPieces, PieceColor* currentPlayer) {
    Position from, to;
    getBestAIMove(whitePieces, blackPieces, &from, &to);
    
    if (from.x == -1 || from.y == -1 || to.x == -1 || to.y == -1) {
        printf("\nAI cannot find a valid move!\n");
        *currentPlayer = (*currentPlayer == WHITE) ? BLACK : WHITE;
        return;
    }
    
    // Simpan state sebelum bergerak untuk undo
    pushState(&undoStack, whitePieces, blackPieces, *currentPlayer);
    clearStack(&redoStack);
    
    // Lakukan gerakan
    movePiece(blackPieces, from, to);
    
    // Cek jika ada bidak lawan di posisi tujuan
    if (findPieceAtPosition(whitePieces, to)) {
        removePieceAtPosition(whitePieces, to);
    }
    
    // Ganti pemain
    *currentPlayer = WHITE;
    
    printf("\nAI moves: %c%d to %c%d\n", 
           'a' + from.x, 8 - from.y, 
           'a' + to.x, 8 - to.y);
    getchar(); // Tunggu input
}

// Fungsi utama
int main() {
    ChessPieceList whitePieces, blackPieces;
    initChessPieceList(&whitePieces);
    initChessPieceList(&blackPieces);
    initializeChessBoard(&whitePieces, &blackPieces);
    
    initStack(&undoStack);
    initStack(&redoStack);
    
    Position selected = {-1, -1};
    PieceColor currentPlayer = WHITE;
    bool gameRunning = true;
    char input[10];
    Position pos;
    ChessPieceList* currentPieces;
    ChessPieceNode* piece;
    PossibleMoves moves;
    int i;
    bool validMove;
    
    printf("Enable AI opponent? (y/n): ");
    char aiChoice = getchar();
    if (aiChoice == 'y' || aiChoice == 'Y') {
        aiEnabled = true;
        printf("AI enabled. AI plays as Black.\n");
    } else {
        aiEnabled = false;
        printf("AI disabled. Two-player mode.\n");
    }
    getchar(); // Consume newline
    
    while (gameRunning) {
        system("clear || cls");  // Bersihkan layar
        displayChessBoard(&whitePieces, &blackPieces, selected);
        
        if (aiEnabled && currentPlayer == aiColor) {
            makeAIMove(&whitePieces, &blackPieces, &currentPlayer);
            continue;
        }
        
        printf("\n%s's turn.", (currentPlayer == WHITE) ? "White" : "Black");
        printf("\nCommands: [move] [u]ndo [r]edo [h]ints [q]uit");
        printf("\nSelect piece or command: ");
        
        scanf("%s", input);
        
        if (strcmp(input, "q") == 0) {
            gameRunning = false;
        } else if (strcmp(input, "u") == 0) {
            // Undo
            if (popState(&undoStack, &whitePieces, &blackPieces, &currentPlayer)) {
                pushState(&redoStack, &whitePieces, &blackPieces, currentPlayer);
                selected.x = -1;
                selected.y = -1;
            } else {
                printf("\nCannot undo further!\n");
                getchar(); // Tunggu input
            }
        } else if (strcmp(input, "r") == 0) {
            // Redo
            if (popState(&redoStack, &whitePieces, &blackPieces, &currentPlayer)) {
                pushState(&undoStack, &whitePieces, &blackPieces, currentPlayer);
                selected.x = -1;
                selected.y = -1;
            } else {
                printf("\nCannot redo further!\n");
                getchar(); // Tunggu input
            }
        } else if (strcmp(input, "h") == 0) {
            // Toggle hints
            showHints = !showHints;
            printf("\nHints %s\n", showHints ? "ON" : "OFF");
            getchar(); // Tunggu input
        } else if (strlen(input) == 2) {
            // Gerakan bidak
            pos.x = tolower(input[0]) - 'a';
            pos.y = 8 - (input[1] - '0');
            
            if (pos.x < 0 || pos.x >= BOARD_SIZE || pos.y < 0 || pos.y >= BOARD_SIZE) {
                printf("\nInvalid position!\n");
                getchar(); // Tunggu input
                continue;
            }
            
            currentPieces = (currentPlayer == WHITE) ? &whitePieces : &blackPieces;
            piece = findPieceAtPosition(currentPieces, pos);
            
            if (piece != NULL && (selected.x == -1 && selected.y == -1)) {
                // Pilih bidak
                selected = pos;
            } else if (selected.x != -1 && selected.y != -1) {
                // Coba gerakan
                piece = findPieceAtPosition(currentPieces, selected);
                if (piece == NULL) {
                    printf("\nNo piece selected!\n");
                    getchar(); // Tunggu input
                    continue;
                }
                
                moves = getPossibleMoves(piece, &whitePieces, &blackPieces);
                validMove = false;
                
                for (i = 0; i < moves.count; i++) {
                    if (moves.moves[i].x == pos.x && moves.moves[i].y == pos.y) {
                        validMove = true;
                        break;
                    }
                }
                
                if (validMove) {
                    // Simpan state sebelum bergerak untuk undo
                    pushState(&undoStack, &whitePieces, &blackPieces, currentPlayer);
                    clearStack(&redoStack);
                    
                    // Lakukan gerakan
                    movePiece(currentPieces, selected, pos);
                    
                    // Cek jika ada bidak lawan di posisi tujuan
                    ChessPieceList* opponentPieces = (currentPlayer == WHITE) ? &blackPieces : &whitePieces;
                    if (findPieceAtPosition(opponentPieces, pos)) {
                        removePieceAtPosition(opponentPieces, pos);
                    }
                    
                    // Ganti pemain
                    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
                    selected.x = -1;
                    selected.y = -1;
                } else {
                    printf("\nInvalid move!\n");
                    getchar(); // Tunggu input
                }
            }
        } else {
            printf("\nInvalid input!\n");
            getchar(); // Tunggu input
        }
    }
    
    // Bersihkan memori
    clearChessPieceList(&whitePieces);
    clearChessPieceList(&blackPieces);
    clearStack(&undoStack);
    clearStack(&redoStack);
    
    return 0;
}
