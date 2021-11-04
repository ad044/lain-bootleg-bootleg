#include "engine.h"
#include "kumashoot.h"
#include "sprite.h"
#include "vector2d.h"

#include <float.h>
#include <stdio.h>

static _Bool get_behavior(SpriteBehavior *behavior,
			  SpriteBehavior *sprite_behaviors, Vector2D click_pos)
{
	_Bool found = false;
	float lowest_dist = FLT_MAX;

	for (int i = 0; i < cvector_size(sprite_behaviors); i++) {
		Sprite *sprite = sprite_behaviors[i].sprite;

		if (is_sprite_within_bounds(sprite, click_pos)) {

			Vector2D sprite_center =
			    get_sprite_center_coords(sprite);

			float curr_dist =
			    dist_between(sprite_center, click_pos);

			if (curr_dist < lowest_dist) {
				found = true;
				*behavior = sprite_behaviors[i];
				lowest_dist = curr_dist;
			}
		}
	}

	return found;
}

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

	SpriteBehavior behavior;
	_Bool behavior_found =
	    get_behavior(&behavior, scene.sprite_behaviors, click_pos);

	if (behavior_found) {
		handle_menu_event(behavior.click_event, engine);
	}
}

void handle_minigame_click(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (!(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)) {
		return;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	Vector2D click_pos = (Vector2D){x, y};

	Scene scene = *engine->minigame.scene;

	SpriteBehavior behavior;
	_Bool behavior_found =
	    get_behavior(&behavior, scene.sprite_behaviors, click_pos);

	if (behavior_found) {
		int event = behavior.click_event;
		void *obj = behavior.object;

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
