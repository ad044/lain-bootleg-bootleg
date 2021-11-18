#pragma once

#include "dressup.h"
#include "engine.h"
#include "minigame.h"

typedef enum { ITEM_GRAB, ITEM_RELEASE, LAIN_SWAP_CLOTHING } DressUpEvent;

typedef enum { STANDING, ENTERING, LEAVING } LainMoveState;

typedef enum { SCREWDRIVER, NAVI, CLOTHING } DressUpObjectType;

typedef struct {
	LainMoveState move_state;
	Texture *standing_texture;
	Texture *walking_texture;
	Texture *leave_texture;
	Sprite sprite;
} DressUpLain;

typedef struct {
	Sprite sprite;
	DressUpObjectType type;
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

void start_dressup(Menu *menu, Resources *resources, GameState *game_state,
		   Minigame *minigame, GLFWwindow **minigame_window,
		   GLFWwindow *main_window);
void handle_dressup_event(DressUpEvent event, void *object, Engine *engine);
