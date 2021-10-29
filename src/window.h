#pragma once

#include "engine.h"

#define SHRINKED_MENU_WIDTH 161
#define SHRINKED_MENU_HEIGHT 74

#define EXPANDED_MENU_WIDTH 196
#define EXPANDED_MENU_HEIGHT 196

#define KUMASHOOT_WIDTH 600
#define KUMASHOOT_HEIGHT 400

void shrink_main_window(GLFWwindow *window);
void expand_main_window(GLFWwindow *window);
int make_window(GLFWwindow **window, int width, int height, char *name,
		GLFWwindow *shared_ctx, _Bool centered);
