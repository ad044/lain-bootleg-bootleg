#include "dressup.h"
#include "engine.h"
#include "kumashoot.h"
#include "minigame.h"
#include "scene.h"
#include "sprite.h"
#include "vector2d.h"

#include <float.h>
#include <stdint.h>
#include <stdio.h>

static _Bool get_behavior(SpriteBehavior *behavior,
			  SpriteBehavior *sprite_behaviors, Vector2D click_pos)
{
	_Bool found = false;
	float lowest_dist = FLT_MAX;
	uint8_t curr_z_index = 0;

	for (int i = 0; i < cvector_size(sprite_behaviors); i++) {
		SpriteBehavior curr_behavior = sprite_behaviors[i];
		Sprite *sprite = sprite_behaviors[i].sprite;

		if (!sprite->visible) {
			continue;
		}

		if (is_sprite_within_bounds(sprite, click_pos)) {
			Vector2D sprite_center =
			    get_sprite_center_coords(sprite);

			float curr_dist =
			    dist_between(sprite_center, click_pos);

			if (curr_dist < lowest_dist ||
			    sprite->z_index > curr_z_index) {
				found = true;
				*behavior = curr_behavior;
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

void handle_minigame_event(GLFWwindow *window, int button, int action, int mods)
{
	Engine *engine = (Engine *)glfwGetWindowUserPointer(window);

	if (!(button == GLFW_MOUSE_BUTTON_LEFT) ||
	    (action == GLFW_RELEASE && engine->minigame.type != DRESSUP)) {
		return;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	Vector2D click_pos = (Vector2D){x, y};

	Scene scene = *engine->minigame.scene;

	for (int i = 0; i < cvector_size(scene.click_barriers); i++) {
		ClickBarrier barrier = scene.click_barriers[i];
		_Bool is_blocked = (barrier.left <= click_pos.x &&
				    click_pos.x <= barrier.right) &&
				   (barrier.top <= click_pos.y &&
				    click_pos.y <= barrier.bottom);
		if (is_blocked) {
			return;
		}
	}

	SpriteBehavior behavior;
	_Bool behavior_found =
	    get_behavior(&behavior, scene.sprite_behaviors, click_pos);

	if (behavior_found) {
		int event = behavior.click_event;
		void *obj = behavior.object;

		switch (engine->minigame.type) {
		case KUMASHOOT: {
			handle_kumashoot_event(event, obj, engine);
			break;
		}
		case DRESSUP: {
			if (action == GLFW_RELEASE) {
				handle_dressup_event(ITEM_RELEASE, NULL,
						     engine);
			} else {
				handle_dressup_event(event, obj, engine);
			}
			break;
		}
		default:
			break;
		}
	}
}
