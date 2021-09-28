#include "sprite.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

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

void handle_menu_click(Menu *menu, GLFWwindow *window)
{
	static int old_state = GLFW_RELEASE;
	int new_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (new_state == GLFW_PRESS && old_state == GLFW_RELEASE) {
		Vector2D mouse_pos;
		get_normalized_mouse_pos(&mouse_pos, window);
		// todo check for highest z index
		for (int i = 0; i < menu->scene->sprite_count; i++) {
			Sprite curr_sprite = menu->scene->sprites[i];
			if (curr_sprite.on_click != NULL &&
			    is_sprite_within_bounds(curr_sprite, mouse_pos)) {
				curr_sprite.on_click();
			}
		}
	}
	old_state = new_state;
}
