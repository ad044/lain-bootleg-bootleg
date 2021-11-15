#pragma once

#include "engine.h"
#include "minigame.h"

typedef enum { ITEM_GRAB, ITEM_RELEASE } DressUpEvent;

typedef enum { STANDING, ENTERING, LEAVING } LainMoveState;

typedef struct {
	LainMoveState move_state;
	Texture *standing_texture;
	Texture *walking_texture;
	Texture *leave_texture;
	Sprite sprite;
} DressupLain;

typedef struct {
	Scene scene;

	DressupLain lain;

	Sprite *currently_grabbed;

	Sprite school_outfit;
	Sprite bear_outfit;
	Sprite pajama_outfit;
	Sprite cyberia_outfit;
	Sprite ufo;
	Sprite navi;
	Sprite screwdriver;
	Sprite background;
} DressUp;

void start_dressup(Resources *resources, GameState *game_state,
		   Minigame *minigame, GLFWwindow **minigame_window,
		   GLFWwindow *main_window);
void handle_dressup_event(DressUpEvent event, Sprite *sprite, Engine *engine);
