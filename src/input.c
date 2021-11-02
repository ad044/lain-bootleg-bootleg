#include "engine.h"
#include "util.h"

#include <stdio.h>

void handle_menu_click(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (!(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)) {
		return;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	Vector2D click_pos = (Vector2D){x, y};

	Scene scene = engine->menu.scene;

	for (int i = 0; i < cvector_size(scene.sprite_behaviors); i++) {
		Sprite *sprite = scene.sprite_behaviors[i].sprite;
		MenuEvent event = scene.sprite_behaviors[i].click_event;

		if (is_sprite_within_bounds(sprite, click_pos)) {
			handle_menu_event(event, engine);
		}
	}
}

void handle_minigame_click(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		return;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	Vector2D click_pos = (Vector2D){x, y};

	Scene *scene = engine->minigame.scene;

	for (int i = 0; i < cvector_size(scene->sprite_behaviors); i++) {
		SpriteBehavior sprite_behavior = scene->sprite_behaviors[i];
		Sprite *sprite = sprite_behavior.sprite;
		int event = sprite_behavior.click_event;
		void *obj = sprite_behavior.object;

		if (is_sprite_within_bounds(sprite, click_pos)) {
			switch (engine->minigame.type) {
			case KUMASHOOT: {
				handle_kumashoot_event(event, obj, engine);
				return;
			} break;
			case NONE:
				return;
			}
		}
	}
}
