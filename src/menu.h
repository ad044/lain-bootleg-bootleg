#pragma once

#include <stdbool.h>

#include "resource_cache.h"
#include "scene.h"
#include "text.h"

typedef struct {
	_Bool expanded;
	unsigned char *current_time;
	Scene *scene;
	Text *clock;
} Menu;

int init_menu(ResourceCache *resource_cache, Menu **menu);
void update_menu(Menu *menu);
void draw_menu(ResourceCache *resource_cache, Menu *menu);
