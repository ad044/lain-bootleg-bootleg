#pragma once

#include <stdbool.h>

#include "scene.h"
#include "timeutil.h"
#include "resource_cache.h"

typedef struct {
	_Bool expanded;
	GameTime *current_time;
	Scene *scene;
} Menu;

int init_menu(ResourceCache *resource_cache, Menu **menu);
void update_menu(Menu *menu);
