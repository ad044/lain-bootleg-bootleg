#pragma once

#include "menu.h"
#include "minigame.h"
#include "resources.h"
#include "scene.h"
#include "state.h"

typedef struct {
	Resources resources;

	GLFWwindow *main_window;
	Menu menu;

	GLFWwindow *minigame_window;
	Minigame minigame;

	GameState game_state;
} Engine;

int engine_init(Engine *engine);
void engine_run(Engine *engine);
