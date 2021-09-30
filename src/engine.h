#pragma once
#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

typedef struct {
	ResourceCache *resource_cache;
	Menu *menu;
	GLFWwindow *main_window;
} Engine;

int engine_init(Engine *engine);
void engine_run(Engine *engine);
