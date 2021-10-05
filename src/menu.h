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
	_Bool animating;
	MenuSprites *sprites;
	Scene *scene;
	Text *clock;
} Menu;

int init_menu(ResourceCache *resource_cache, Menu **menu);
void update_menu(Menu *menu, GLFWwindow *window);
void toggle_menu_expand(void *ctx, Sprite *clicked_sprite, Vector2D click_pos);
void draw_menu(Menu *menu, GLFWwindow *window);
