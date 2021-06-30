#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * Header containing constants for main.c and the snake module
 */

#define WIDTH 800
#define HEIGHT 600
#define SQ 48

#define FWIDTH ((float)WIDTH)
#define FHEIGHT ((float)HEIGHT)

#define VERTEX(x, y) glVertex2f((x) / FHEIGHT, (y) / FHEIGHT)

enum Error { NoError, ErrorGlfwInit, ErrorShader };

#endif
