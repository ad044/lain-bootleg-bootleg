#pragma once

#include "resource_cache.h"
#include "scene.h"
#include "text.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct {
	Sprite *main_ui;
	Sprite *main_ui_bar;
	Sprite *lain;
	Sprite *dressup_button;
} MenuSprites;

typedef struct {
	_Bool expanded;
	MenuSprites *sprites;
	Scene *scene;
	Text *clock;
} Menu;

int init_menu(ResourceCache *resource_cache, Menu **menu);
void update_menu(Menu *menu);
int load_shrinked_menu_scene(Scene **scene, ResourceCache *resource_cache);
int load_expanded_menu_scene(Scene **scene, ResourceCache *resource_cache);
void draw_menu(Menu *menu, GLFWwindow *window);
