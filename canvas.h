#ifndef CANVAS_H
#define CANVAS_H

#include "constants.h"
#include "snake.h"

typedef struct _Canvas {

    Vector dimensions, pos;
    unsigned int framebuffer, texture;
    unsigned int oldfb;

} Canvas;

Canvas *initCanvas(int width, int height);
void destroyCanvas(Canvas *c);

void clearCanvas(Canvas *c);

void useCanvas(Canvas *c);
void resetCanvas();

void drawCanvas(Canvas *c);

#endif