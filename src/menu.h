#pragma once

#include "scene.h"
#include "state.h"
#include "resources.h"

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

// i think ideally youd have a separate contextual processor
// for _CLICK events and then dispatch actions accordingly
// but in our case its so simple might aswell merge them together
typedef enum {
	MAIN_UI_BAR_CLICK,
	TOGGLE_THEATER_PREVIEW,
	TOGGLE_SCORE_PREVIEW,
	BEAR_ICON_CLICK,
} MenuEvent;

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

void init_menu(Menu *menu, GameState *game_state, Resources *resources);
void update_menu(Menu *menu, const GameState *game_state, GLFWwindow *window,
		 Texture *textures);

// void * game here is Engine but because header files are cancer
// i cant pass it directly without creating circular dependencies
// and forward declaring would likely be messy, perhaps ill take a look
void handle_menu_event(MenuEvent event, void *game);
