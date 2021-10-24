#pragma once
#include "state.h"
#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

typedef struct {
	ResourceCache *resource_cache;
	GLFWwindow *main_window;
	Menu *menu;
	GameState *game_state;
} Engine;

int engine_init(Engine *engine);
void engine_run(Engine *engine);
