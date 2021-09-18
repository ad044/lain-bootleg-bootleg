#pragma once
#include <GLFW/glfw3.h>

#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "timeutil.h"

typedef struct {
	ResourceCache *resource_cache;
	Menu *menu;
	Scene current_scene;
	GLFWwindow *menu_window;
} Engine;

int engine_init(Engine *engine, GLFWwindow *menu_window);
void engine_render(Engine *engine);
