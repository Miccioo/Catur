#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\game\papan.h"
#include "..\game\Gamestate.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#endif

// ANSI Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

// Background colors
#define BG_BLACK   "\033[40m"

// Menu options
typedef enum {
    MENU_NEW_GAME,
    MENU_LOAD_GAME,
    MENU_SETTINGS,
    MENU_ABOUT,
    MENU_EXIT,
    MENU_COUNT
} MenuOption;

// Game options
typedef enum {
	GAME_CLASSIC_CHESS,
	GAME_EVOLVE_CHESS,
	GAME_ZOMBIE_CHESS,
	GAME_BACK,
	GAME_COUNT
} GameType;

typedef enum {
	PLAYER_VS_PLAYER,
	PLAYER_VS_AI,
	AI_VS_AI,
	VERSUS_BACK,
	VERSUS_COUNT
} VersusOption;

// Function prototypes
void clearScreen();
void getTerminalSize(int *width, int *height);
void moveCursor(int x, int y);
void printCentered(const char* text, int termWidth, const char* color);
void displayBanner();
GameType showNewGameOption(int termWidth);
VersusOption showVersusOption(int termWidth);
int showMainMenu(int termWidth);
void aboutScreen(int termWidth);
void settingsScreen(int termWidth);
void waitForKeyPress();
int getKeyPress();

#endif
