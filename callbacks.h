#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>

void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods);

int keyPressed(char key);
void keyRelease(char key);

#endif