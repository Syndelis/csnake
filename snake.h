#ifndef SNAKE_H
#define SNAKE_H

// ------------------------------------------------------------------------------
/*
Globals and structs
*/

typedef union {

    struct { short int x, y; };
    int _data;

} Vector;

// ------------------------------------

typedef unsigned char byte;
#define SnakeBlockSize 16

// ------------------------------------

typedef enum {

    Up=     0xFFFF0000, Down=   0x00010000,
    Left=   0x0000FFFF, Right=  0x00000001

} Direction;

// ------------------------------------

typedef struct _Snake {

    Vector body[SnakeBlockSize];

    Vector direction;
    byte used;

    Vector *head, *tail;
    struct _Snake *next;

} Snake;

// ------------------------------------------------------------------------------
/*
Public Functions
*/

Snake *initSnake(unsigned short x, unsigned short y);
void drawSnake(Snake *s);
void stepSnake(Snake *s);
void growSnake(Snake *s);
void freeSnake(Snake *s);
int collidesWith(Snake *s, Vector *v);

#endif