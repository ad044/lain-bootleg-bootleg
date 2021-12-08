#pragma once

#include "dressup.h"
#include "kumashoot.h"
#include "menu.h"
#include "scene.h"
#include "state.h"
#include "theater.h"

typedef enum { NO_MINIGAME, KUMASHOOT, DRESSUP, THEATER } MinigameType;

typedef struct minigame {

	MinigameType type;

	union {
		KumaShoot kumashoot;
		DressUp dressup;
		Theater theater;
	} current;

	double last_updated;

	MinigameType queued_minigame;
} Minigame;

void destroy_minigame(Texture *textures, Menu *menu, Minigame *minigame,
		      GLFWwindow *minigame_window);
void update_minigame(Resources *resources, GameState *game_state, Menu *menu,
		     GLFWwindow *minigame_window, Minigame *minigame);
void draw_minigame(Resources *resources, GLFWwindow *minigame_window,
		   Minigame *minigame);
void start_queued_minigame(Resources *resources, GameState *game_state,
			   Menu *menu, GLFWwindow *main_window,
			   GLFWwindow **minigame_window, Minigame *minigame);
void get_minigame_scene(Minigame *minigame, Scene *target);
_Bool can_refresh(double time, Minigame *minigame);
