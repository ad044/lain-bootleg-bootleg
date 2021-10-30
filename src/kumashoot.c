#include <stdio.h>

#include "kumashoot.h"
#include "window.h"

static void init_kumashoot_sprites(KumaShoot *kumashoot,
				   const ResourceCache *resource_cache)
{
	kumashoot->background =
	    (Sprite){.pos = {0.0f, 0.0f},
		     .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
		     .texture = texture_cache_get(resource_cache->textures,
						  "kumashoot_bg"),
		     .visible = true,
		     .z_index = 0};

	kumashoot->bush_overlay = (Sprite){
	    .pos = {0.0f, 0.0f},
	    .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
	    .texture =
		texture_cache_get(resource_cache->textures, "bush_overlay"),
	    .visible = true,
	    .z_index = 1,
	};
}

static void init_kumashoot_scene(Scene *scene, KumaShoot *kumashoot,
				 const ResourceCache *resource_cache)
{
	init_kumashoot_sprites(kumashoot, resource_cache);

	Sprite *sprites[] = {&kumashoot->background, &kumashoot->bush_overlay};
	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	init_scene(scene, sprites, sprite_count, NULL, 0, NULL, 0,
		   resource_cache->shaders[QUAD_SHADER]);
}

static void update_kumashoot(void *minigame_struct, GameState *game_state) {}

void start_kumashoot(void *ctx, Sprite *clicked_sprite, Vector2D click_pos)
{
	Engine *engine = (Engine *)ctx;
	Minigame *minigame = &engine->minigame;

	if (minigame->running) {
		kill_minigame(minigame, &engine->minigame_window);
	}

	KumaShoot *kumashoot = malloc(sizeof(KumaShoot));
	if (kumashoot == NULL) {
		printf("Failed to allocate memory kuma shoot struct.\n");
		exit(1);
	}

	if (!(make_window(&engine->minigame_window, KUMASHOOT_WIDTH,
			  KUMASHOOT_HEIGHT, "lain kuma shoot",
			  engine->main_window, true))) {
		printf("Failed to create kuma shoot window.\n");
		exit(1);
	}

	init_kumashoot_scene(&kumashoot->scene, kumashoot,
			     &engine->resource_cache);

	minigame->running = true;
	minigame->current = kumashoot;
	minigame->update = update_kumashoot;
	minigame->scene = &kumashoot->scene;
}
