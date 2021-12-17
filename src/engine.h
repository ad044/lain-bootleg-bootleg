#pragma once

#include "menu.h"
#include "minigame.h"
#include "scene.h"
#include "state.h"

#include <pthread.h>

typedef struct engine {
	Resources resources;

	GLFWwindow *main_window;
	Menu menu;

	GLFWwindow *minigame_window;
	Minigame minigame;

	GameState game_state;

	PaStream *audio_stream;
	pthread_t audio_thread;

	pthread_mutex_t running;
} Engine;

int engine_init(Engine *engine);
void engine_stop(Engine *engine);
void engine_run(Engine *engine);
