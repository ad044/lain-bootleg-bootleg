#pragma once

#include "resource_cache.h"
#include "scene.h"
#include "state.h"
#include "text.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef enum {
	CLASSROOM,
	SCHOOL,
	LAIN_ROOM_NIGHT,
	ALICE_ROOM,
	CYBERIA,
	STREET,
	BRIDGE
} TheaterScenes;

typedef enum {
	NOT_BLINKING,
	HAS_BLINKED,
	BLINK_OPENING,
	BLINK_CLOSING
} BlinkState;

typedef struct {
	struct tm *current_time;

	_Bool expanded;
	_Bool animating;

	BlinkState lain_blink_state;

	Scene scene;

	Sprite ui_lain;
	Sprite main_ui;
	Sprite main_ui_bar;
	Sprite dressup_button;
	Sprite theater_button;
	Sprite bear_icon;
	Sprite screwdriver_icon;
	Sprite paw_icon;
	Sprite theater_preview;
	Sprite score_preview;

	Text clock;
	Text score_text;
} Menu;

void init_menu(ResourceCache *resource_cache, GameState *game_state,
	       Menu *menu);
void update_menu(Menu *menu, const GameState *game_state, GLFWwindow *window,
		 const ResourceCache *resource_cache);
