#include "input.h"
#include "engine.h"
#include "util.h"

#include <stdio.h>

void handle_menu_click(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		Vector2D mouse_pos = (Vector2D){x, y};

		Scene scene = engine->menu.scene;

		for (int i = 0; i < cvector_size(scene.sprite_behaviors); i++) {
			Sprite *sprite = scene.sprite_behaviors[i].sprite;
			OnClickFunc on_click =
			    scene.sprite_behaviors[i].on_click;

			if (is_sprite_within_bounds(sprite, mouse_pos)) {
				on_click(engine, sprite, mouse_pos);
			}
		}
	}
}
