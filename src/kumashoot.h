#pragma once

#include "scene.h"
#include "sprite.h"

typedef struct {
	Sprite *background;
	Sprite *bush_overlay;
} KumaShootSprites;

typedef struct {
	KumaShootSprites *sprites;
} KumaShoot;

void start_kumashoot(void *ctx, Sprite *clicked_sprite, Vector2D click_pos);
