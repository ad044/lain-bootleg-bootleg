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
} DressUpLain;

typedef struct {
	Sprite sprite;
	LainOutfit outfit;
} DressUpObject;

typedef struct {
	Scene scene;

	DressUpLain lain;

	DressUpObject *currently_grabbed;

	DressUpObject school_outfit;
	DressUpObject bear_outfit;
	DressUpObject pajama_outfit;
	DressUpObject cyberia_outfit;
	DressUpObject ufo;
	DressUpObject navi;
	DressUpObject screwdriver;

	Sprite background;
} DressUp;

void start_dressup(Resources *resources, GameState *game_state,
		   Minigame *minigame, GLFWwindow **minigame_window,
		   GLFWwindow *main_window);
void handle_dressup_event(DressUpEvent event, DressUpObject *object,
			  Engine *engine);
