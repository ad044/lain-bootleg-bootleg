#pragma once

#include "dressup.h"
#include "engine.h"
#include "minigame.h"

typedef enum { ITEM_GRAB, ITEM_RELEASE, LAIN_SWAP_CLOTHING } DressUpEvent;

typedef struct {
	enum { STANDING, ENTERING, LEAVING } move_state;
	Sprite sprite;
} DressUpLain;

typedef struct {
	enum { SCREWDRIVER, NAVI, CLOTHING } type;
	Sprite sprite;
	LainOutfit outfit;
} DressUpObject;

typedef struct {
	Scene scene;

	DressUpLain lain;

	DressUpObject *currently_grabbed;

	DressUpObject school_outfit;
	DressUpObject bear_outfit;
	DressUpObject sweater_outfit;
	DressUpObject cyberia_outfit;
	DressUpObject ufo;
	DressUpObject navi;
	DressUpObject screwdriver;

	Sprite background;
} DressUp;

void lain_set_outfit(Resources *resources, LainOutfit outfit, Lain *lain);
int start_dressup(Menu *menu, Resources *resources, GameState *game_state,
		  Minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window);
void handle_dressup_event(DressUpEvent event, void *object, Engine *engine);
void update_dressup(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, Minigame *minigame);
