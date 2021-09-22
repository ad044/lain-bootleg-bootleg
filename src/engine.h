#pragma once
#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

#include <GLFW/glfw3.h>

typedef struct {
	ResourceCache *resource_cache;
	Menu *menu;
	GLFWwindow *menu_window;
} Engine;

int engine_init(Engine *engine, GLFWwindow *menu_window);
void engine_render(Engine *engine);
