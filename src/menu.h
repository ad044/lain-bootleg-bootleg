#pragma once

#include "resources.h"
#include "scene.h"
#include "state.h"

typedef enum { LAUGH, BLINK, LAUGH_BLINK } MenuLainAnimation;

typedef struct {
	_Bool recently_changed_laugh;
	_Bool laughing;
	_Bool blinking;
	int laugh_quarter;
	Sprite sprite;
} MenuLain;

// i think ideally youd have a separate contextual processor
// for _CLICK events and then dispatch actions accordingly
// but in our case its so simple might aswell merge them together
typedef enum {
	MAIN_UI_BAR_CLICK,
	TOGGLE_THEATER_PREVIEW,
	TOGGLE_SCORE_PREVIEW,
	BEAR_ICON_CLICK,
	DRESSUP_TOGGLE,
	THEATER_TOGGLE
} MenuEvent;

typedef struct {
	struct tm *current_time;

	_Bool collapsed;

	Scene scene;

	MenuLain ui_lain;

	Sprite main_ui;
	Sprite main_ui_bar;
	Sprite dressup_button;
	Sprite theater_button;
	Sprite bear_icon;
	Sprite screwdriver_icon;
	Sprite paw_icon;
	Sprite theater_preview;
	Sprite score_preview;
	Sprite background;

	Text clock;
	Text score_text;
} Menu;

void init_menu(Menu *menu, GameState *game_state, Resources *resources);
void update_menu(Menu *menu, GameState *game_state, GLFWwindow *window,
		 Resources *resources);
// void * game here is Engine but because header files are cancer
// i cant pass it directly without creating circular dependencies
// and forward declaring would likely be messy, perhaps ill take a look
void handle_menu_event(MenuEvent event, void *game);
