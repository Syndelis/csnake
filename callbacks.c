#include "callbacks.h"
#include "snake.h"
#include <GLFW/glfw3.h>

__uint128_t _keypress = 0ULL;
void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (action)
        _keypress |= ((__uint128_t)1ULL) << key;

    else _keypress &= (~(((__uint128_t)1ULL) << key));

}

int keyPressed(char key) {

    return (_keypress & (((__uint128_t)1ULL) << key)) > 0ULL;

}

void keyRelease(char key) {

    _keypress &= (~(((__uint128_t)1ULL) << key));

}