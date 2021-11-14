#pragma once

#include "menu.h"
#include "scene.h"
#include "state.h"

typedef enum {
	NONE,
	KUMASHOOT,
} MinigameType;

typedef void (*UpdateMinigameFunc)(Resources *resources, void *minigame_struct,
				   GameState *game_state);

typedef struct {
	MinigameType type;
	_Bool running;
	UpdateMinigameFunc update;
	Scene *scene;
	void *current;

	double last_updated;
	double refresh_rate;
} Minigame;

void kill_minigame(Menu *menu, Minigame *minigame, GLFWwindow **minigame_window,
		   Texture *textures);
_Bool can_refresh(double time, Minigame *minigame);
