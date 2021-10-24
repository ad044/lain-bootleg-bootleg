#pragma once

#include "sprite.h"
#include "util.h"

#include <GLFW/glfw3.h>

typedef void (*OnClickFunc)(void *ctx, Sprite *clicked_sprite,
			    Vector2D click_pos);

void handle_menu_click(GLFWwindow *window, int button, int action, int mods);
