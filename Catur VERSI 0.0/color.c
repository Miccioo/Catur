#include "color.h"

void setRgbColor(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void setRgbBackground(int r, int g, int b) {
    printf("\033[48;2;%d;%d;%dm", r, g, b);
}
