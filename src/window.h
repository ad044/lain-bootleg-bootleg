#pragma once
#include "engine.h"
#include <GLFW/glfw3.h>

int make_window(GLFWwindow **window, int width, int height, char *name);
void toggle_main_window_expanded(Engine *engine);
int make_main_window(GLFWwindow **window);
