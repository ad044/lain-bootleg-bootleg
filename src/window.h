#pragma once
#include "engine.h"
#include <GLFW/glfw3.h>

int make_window(GLFWwindow **window, int width, int height, char *name);
void expand_main_window(Engine *engine);
void shrink_main_window(Engine *engine);
int make_menu_window(GLFWwindow **window);
