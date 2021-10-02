#include "sprite.h"

#include <stdio.h>

#include "engine.h"
#include "input.h"

static void get_normalized_mouse_pos(Vector2D *pos, GLFWwindow *window);

// get current mouse coords and normalizes to range -1 to 1
static void get_normalized_mouse_pos(Vector2D *pos, GLFWwindow *window)
{

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	double x_pos, y_pos;
	glfwGetCursorPos(window, &x_pos, &y_pos);

	pos->x = 2 * (x_pos / width) - 1;
	pos->y = -(2 * (y_pos / height) - 1);
}

void handle_menu_click(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Vector2D mouse_pos;
		Scene *scene = engine->menu->scene;

		get_normalized_mouse_pos(&mouse_pos, window);

		for (int i = 0; i < scene->sprite_count; i++) {
			Sprite curr_sprite = *scene->sprites[i];
			if (curr_sprite.on_click != NULL &&
			    is_sprite_within_bounds(&curr_sprite, &mouse_pos)) {
				curr_sprite.on_click(engine);
			}
		}
	}
}
