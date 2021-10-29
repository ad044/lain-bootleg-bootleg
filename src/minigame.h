#pragma once

#include "state.h"
#include "scene.h"

typedef void (*UpdateMinigameFunc)(GameState *game_state,
				   void *minigame_struct);

typedef struct {
	UpdateMinigameFunc updater;
	Scene *scene;
	void *minigame_struct;
} Minigame;

void kill_minigame(GLFWwindow **window, Minigame *minigame);
