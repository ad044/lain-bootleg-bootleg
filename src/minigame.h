#pragma once

#include "menu.h"
#include "scene.h"
#include "state.h"

typedef enum {
	NONE,
	KUMASHOOT,
} MinigameType;

typedef void (*UpdateMinigameFunc)(Texture *textures, void *minigame_struct,
				   GameState *game_state);

typedef struct {
	MinigameType type;
	_Bool running;
	UpdateMinigameFunc update;
	Scene *scene;
	void *current;
} Minigame;

void kill_minigame(Menu *menu, Minigame *minigame, GLFWwindow **minigame_window,
		   Texture *textures);
