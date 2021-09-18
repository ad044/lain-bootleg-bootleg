#pragma once

#include <stdbool.h>

#include "sprite.h"
#include "timeutil.h"

typedef struct {
	SpriteMap *sprite_map;
	_Bool expanded;
	GameTime *current_time;
} Menu;

int init_menu(ResourceCache *resource_cache, Menu **menu);
void update_menu(Menu *menu);
void draw_menu(Menu *menu, GLuint *VAO);
