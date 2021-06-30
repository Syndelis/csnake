#include "snake.h"
#include "macros.h"
#include "constants.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>

// -----------------------------------------------------------------------------

#define SnakeInitialSize 3
Snake *initSnake(unsigned short x, unsigned short y) {

    Snake *new = (Snake*) malloc(sizeof(Snake));

    new->head = new->body;
    new->direction._data = Left;

    for (int i = 0; i < SnakeInitialSize; i++) {

        new->tail = new->body + i;

        new->tail->x = x - new->direction.x * i;
        new->tail->y = y - new->direction.y * i;

    }

    new->next = NULL;
    new->used = SnakeInitialSize;

    return new;

}

// -------------------------------------

void drawSnake(Snake *s) {

    Snake *e;
    byte i;
    Vector *v;
    float x, y;

    for (e = s; e; e = e->next)
        for (i = 0, v = e->head; i < e->used; i++, v++)
            DRAW(GL_POLYGON) {

                x = v->x * SQ;
                y = v->y * SQ;

                VERTEX(x, y);
                VERTEX(x, y + SQ);
                VERTEX(x + SQ, y + SQ);
                VERTEX(x + SQ, y);

            }

}

// -------------------------------------

void stepSnake(Snake *s) {

    Snake *e, *p = NULL;
    Vector tmp[2];
    byte i, j = 0;

    for (e = s; e; e = e->next) {

        if (e->next) memcpy(tmp + j, e->tail, sizeof(Vector));
        memcpy(e->body + 1, e->head, sizeof(Vector) * (e->used-1));

        if (p) memcpy(e->head, tmp + 1 - j, sizeof(Vector));

        p = e;
        j = 1 - j;
        
    }

    s->head->x += s->direction.x;
    s->head->y += s->direction.y;

}

// ------------------------------------

void growSnake(Snake *s) {

    Snake *e;
    for (e = s; e->next; e = e->next);

    if (e->used < SnakeBlockSize)
        e->tail = e->body + e->used++;

    else {

        e->next = (Snake*) malloc(sizeof(Snake));

        e->next->head = e->next->body;
        e->next->tail = e->next->body;
        
        e->next->used = 1;
        e->next->next = NULL;

    }
}

// ------------------------------------

void freeSnake(Snake *s) {

    Snake *a, *b = s;

    while (b) {

        a = b;
        b = b->next;

        free(a);

    }

}

// ------------------------------------

int collidesWith(Snake *s, Vector *vec) {

    Snake *e;
    int i;
    Vector *v;

    for (e = s; e; e = e->next)
        for (i = 0, v = e->head; i < e->used; i++, v++)
            if (v->_data == vec->_data) return 1;

    return 0;

}