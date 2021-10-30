#pragma once

#include "scene.h"
#include "state.h"

typedef void (*UpdateMinigameFunc)(void *minigame_struct,
				   GameState *game_state);

typedef struct {
	_Bool running;
	UpdateMinigameFunc update;
	Scene *scene;
	void *current;
} Minigame;

void kill_minigame(Minigame *minigame, GLFWwindow **window);
