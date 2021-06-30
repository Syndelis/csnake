#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "macros.h"
#include "constants.h"
#include "canvas.h"
#include "shader.h"
#include "snake.h"
#include "callbacks.h"
#include <GLFW/glfw3.h>
#include <time.h>

#define SPEED 4
#define FPS 60
#define SHADERS 3

static char *SH_NAMES[] = {
    "shader/crt.frag", "shader/chroma.frag", "shader/vignette.frag"
};

Snake *player;
Vector apple;

void randomizeApple() {

    do {

        apple.x = rand() % (WIDTH/SQ);
        apple.y = rand() % (HEIGHT/SQ);

    } while (collidesWith(player, &apple));

}

int main() {

    // SETUP -------------------------------------------------------------------

    if (!glfwInit()) return ErrorGlfwInit;

    // Setting the Window --------------
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CSnake", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);

    // Setting GL defaults -------------
    glClearColor(0., 0., 0., 1.);
    glColor4f(1., 1., 1., 1.);
    glPointSize(1.);

    // Setting GL view -----------------
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0., FWIDTH / FHEIGHT, 1., 0., 1., -1.);
    glViewport(0, 0, WIDTH, HEIGHT);

    // Shader Setup --------------------
    forceGlew();

    int i, shaders[SHADERS];
    for (i = 0; i < SHADERS; i++) {

        shaders[i] = newShader("shader/pass.vert", SH_NAMES[i]);
        if (shaders[i] < 0) return ErrorShader;

    }

    Canvas *c[2];
    c[0] = initCanvas(WIDTH, HEIGHT);
    c[1] = initCanvas(WIDTH, HEIGHT);

    // MAIN LOOP ---------------------------------------------------------------

    float x, y;
    player = initSnake(10, 3);
    randomizeApple();

    int frame = 0, *d, next, t, hit = 0;
    Vector tmp;

    while (!glfwWindowShouldClose(window)) {

        frame = (frame + 1) % SPEED;

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor4f(1., 1., 1., 1.);

        resetCanvas();
        resetShader();

        // Draw ------------------------
        useCanvas(c[0]);

        // Apple
        x = apple.x * SQ;
        y = apple.y * SQ;

        glColor4f(.8, .2, .2, 1.);
        DRAW(GL_POLYGON) {

            VERTEX(x, y);
            VERTEX(x + SQ, y);
            VERTEX(x + SQ, y + SQ);
            VERTEX(x, y + SQ);

        }

        glColor4f(1., 1., 1., 1.);
        drawSnake(player);

        // Lines -----------------------
        glColor4b(50, 50, 50, 50);
        for (y = 0.; y < FHEIGHT; y += SQ)
            DRAW(GL_LINE_LOOP) {
                VERTEX(0, y);
                VERTEX(FWIDTH, y);
            }

        for (x = 0.; x < FWIDTH; x += SQ)
            DRAW(GL_LINE_LOOP) {
                VERTEX(x, 0);
                VERTEX(x, FHEIGHT);
            }

        resetCanvas();

        // Shader ----------------------
        for (i = 0; i < SHADERS; i++) {

            t = i&1;
            useCanvas(c[1 - t]);

            useProgram(shaders[i]);

            glUniform1f(glGetUniformLocation(shaders[i], "canvasTexture"), 0.);
            glUniform2f(glGetUniformLocation(shaders[i], "resolution"),
                        FWIDTH, FHEIGHT);

            drawCanvas(c[t]);
            resetShader();

        }

        resetCanvas();
        drawCanvas(c[1 - (i&1)]);

        clearCanvas(c[0]);
        clearCanvas(c[1]);

        // Logic -----------------------

        // ESC key
        if (keyPressed(0)) glfwSetWindowShouldClose(window, 1);

        if (keyPressed('R')) {

            freeSnake(player);
            player = initSnake(10, 3);

            randomizeApple();
            keyRelease('R');
            
            continue;

        }


        if (frame == 0) {

            d = &player->direction._data;
            next = *d;

            if (keyPressed('W') && *d != Down)
                next = Up;

            if (keyPressed('S') && *d != Up)
                next = Down;

            if (keyPressed('A') && *d != Right)
                next = Left;

            if (keyPressed('D') && *d != Left)
                next = Right;

            *d = next;

            if (player->head->_data == apple._data) {

                growSnake(player);
                randomizeApple();

            }

            tmp.x = player->head->x + player->direction.x;
            tmp.y = player->head->y + player->direction.y;

            if (tmp.x >= 0 && tmp.x <= (WIDTH/SQ) &&\
                tmp.y >= 0 && tmp.y <= (HEIGHT/SQ) &&\
                !collidesWith(player, &tmp))

                stepSnake(player);

        }

        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // CLEANUP -----------------------------------------------------------------

    freeSnake(player);
    for (i = 0; i < 3; i++) glDeleteProgram(shaders[i]);
    for (i = 0; i < 2; i++) destroyCanvas(c[i]);

    glfwDestroyWindow(window);
    glfwTerminate();

    return NoError;

}
