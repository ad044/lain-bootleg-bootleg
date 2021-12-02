#pragma once

#include "engine.h"

#define COLLAPSED_MENU_WIDTH 161
#define COLLAPSED_MENU_HEIGHT 74

#define EXPANDED_MENU_WIDTH 196
#define EXPANDED_MENU_HEIGHT 196

#define MINIGAME_WIDTH 600
#define MINIGAME_HEIGHT 400

int make_window(GLFWwindow **window, int width, int height, char *name,
		GLFWwindow *shared_ctx, _Bool centered);
