#pragma once
#include "engine.h"
#include <GLFW/glfw3.h>

int make_window(GLFWwindow **window, int width, int height, char *name);
void toggle_main_window_expanded(void *ctx, Sprite *clicked_sprite,
				 Vector2D click_pos);
int make_main_window(GLFWwindow **window);
