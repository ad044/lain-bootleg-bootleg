#include "input.h"
#include "engine.h"

#include <stdio.h>

// get current mouse coords and normalizes to range -1 to 1
static void get_normalized_mouse_pos(Vector2D *pos, GLFWwindow *window)
{

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	double x_pos, y_pos;
	glfwGetCursorPos(window, &x_pos, &y_pos);

	pos->x = (width / 2.0f * 2) * (x_pos / width) - width / 2.0f;
	pos->y = -(-height / 2.0f * 2) * (y_pos / height) - height / 2.0f;
}

void handle_menu_click(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Vector2D mouse_pos;
		get_normalized_mouse_pos(&mouse_pos, window);

		Scene *scene = engine->menu->scene;

		for (int i = 0; i < cvector_size(scene->sprite_behaviors);
		     i++) {
			Sprite *sprite = *scene->sprite_behaviors[i].sprite;
			OnClickFunc on_click =
			    scene->sprite_behaviors[i].on_click;

			if (is_sprite_within_bounds(sprite, mouse_pos)) {
				on_click(engine, sprite, mouse_pos);
			}
		}
	}
}
