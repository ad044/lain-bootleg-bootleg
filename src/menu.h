#pragma once

#include "resource_cache.h"
#include "scene.h"
#include "text.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef enum {
	NOT_BLINKING,
	HAS_BLINKED,
	BLINK_OPENING,
	BLINK_CLOSING
} BlinkState;

typedef struct {
	BlinkState blink_state;
	Sprite *sprite;
} MenuLain;

typedef struct {
	MenuLain *lain;
	Sprite *main_ui;
	Sprite *main_ui_bar;
	Sprite *dressup_button;
	Sprite *bear_icon;
	Sprite *screwdriver_icon;
	Sprite *paw_icon;
} MenuSprites;

typedef struct {
	Text *clock;
} MenuTextObjects;

typedef struct {
	_Bool expanded;
	_Bool animating;
	Scene *scene;
	struct tm *current_time;
	MenuSprites *sprites;
	MenuTextObjects *text_objs;
} Menu;

int init_menu(ResourceCache *resource_cache, Menu **menu);
void update_menu(Menu *menu, GLFWwindow *window, ResourceCache *resource_cache);
void toggle_menu_animating(void *ctx, Sprite *clicked_sprite,
			   Vector2D click_pos);
void draw_menu(Menu *menu, GLFWwindow *window);
