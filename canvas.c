#include <GL/glew.h>
#include "canvas.h"
#include "macros.h"
#include "constants.h"
#include <GL/gl.h>
#include <stdlib.h>

GLenum drawbuffers = GL_COLOR_ATTACHMENT0;

Canvas *initCanvas(int width, int height) {

    Canvas *c = (Canvas*) malloc(sizeof(Canvas));
    c->dimensions.x = width;
    c->dimensions.y = height;

    c->pos.x = 0;
    c->pos.y = 0;

    c->oldfb = 0;

    glGenFramebuffers(1, &c->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, c->framebuffer);

    glGenTextures(1, &c->texture);
    glBindTexture(GL_TEXTURE_2D, c->texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, width, height,
        0, GL_RGB, GL_UNSIGNED_BYTE, NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, c->texture, 0);

    glDrawBuffers(1, &drawbuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        free(c);
        return NULL;

    }

    return c;

}


void destroyCanvas(Canvas *c) {

    glDeleteTextures(1, &c->texture);
    glDeleteFramebuffers(1, &c->framebuffer);
    free(c);

}

void clearCanvas(Canvas *c) {

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &c->oldfb);

    glBindFramebuffer(GL_FRAMEBUFFER, c->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, c->oldfb);

}

void useCanvas(Canvas *c) {

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &c->oldfb);

    glBindFramebuffer(GL_FRAMEBUFFER, c->framebuffer);
    glViewport(0, 0, c->dimensions.x, c->dimensions.y);

}

void resetCanvas() {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glViewport(0, 0, WIDTH, HEIGHT);

}

void drawCanvas(Canvas *c) {

    glBindTexture(GL_TEXTURE_2D, c->texture);

    glColor4f(1., 1., 1., 1.);
    glPointSize(1.);

    DRAW(GL_QUADS) {

        glTexCoord2f(1., 1.);
        VERTEX(c->pos.x + c->dimensions.x   , c->pos.y                  );

        glTexCoord2f(1., 0.);
        VERTEX(c->pos.x + c->dimensions.x   , c->pos.y + c->dimensions.y);

        glTexCoord2f(0., 0.);
        VERTEX(c->pos.x                     , c->pos.y + c->dimensions.y);

        glTexCoord2f(0., 1.);
        VERTEX(c->pos.x                     , c->pos.y                  );

    }

}