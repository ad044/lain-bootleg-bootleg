#pragma once
#include "engine.h"
#include <GLFW/glfw3.h>

int make_window(GLFWwindow **window, int width, int height, char *name);
void shrink_main_window(GLFWwindow *window);
void expand_main_window(GLFWwindow *window);
int make_main_window(GLFWwindow **window);
