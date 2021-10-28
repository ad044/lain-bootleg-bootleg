#include <stdio.h>

#include "kumashoot.h"
#include "window.h"

static int init_kumashoot_scene(Scene *scene, KumaShoot *kumashoot,
				ResourceCache *resource_cache)
{
	SceneSprite sprites[] = {
	    (SceneSprite){
		.loc = &kumashoot->sprites->background,
		.sprite =
		    (Sprite){.pos = {0.0f, 0.0f},
			     .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
			     .texture = texture_cache_get(
				 resource_cache->textures, "kumashoot_bg"),
			     .visible = true,
			     .z_index = 0}},
	    (SceneSprite){.loc = &kumashoot->sprites->bush_overlay,
			  .sprite =
			      (Sprite){
				  .pos = {0.0f, 0.0f},
				  .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
				  .texture = texture_cache_get(
				      resource_cache->textures, "bush_overlay"),
				  .visible = true,
				  .z_index = 1,
			      }},

	};
	unsigned int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	SceneDefinition kumashoot_scene_def = {
	    .sprites = sprites,
	    .sprite_count = sprite_count,
	};

	if (!(init_scene(scene, &kumashoot_scene_def, resource_cache))) {
		printf("Failed to initialize kuma shoot scene.\n");
		return 0;
	};

	return 1;
}

static void test(GameState *game_state, void *minigame_struct)
{
	printf("here\n");
}

void start_kumashoot(void *ctx, Sprite *clicked_sprite, Vector2D click_pos)
{
	Engine *engine = (Engine *)ctx;

	if (!(make_window(&engine->minigame_window, KUMASHOOT_WIDTH,
			  KUMASHOOT_HEIGHT, "lain kuma shoot",
			  engine->main_window))) {
		printf("Failed to start kuma shoot.\n");
		exit(1);
	}

	KumaShoot *kumashoot = malloc(sizeof(KumaShoot));
	if (kumashoot == NULL) {
		printf("Failed to allocate memory kuma shoot struct.\n");
		exit(1);
	}

	kumashoot->sprites = malloc(sizeof(KumaShootSprites));
	if (kumashoot->sprites == NULL) {
		printf("Failed to allocate memory for kuma shoot sprites.\n");
		exit(1);
	}

	Minigame *minigame = engine->minigame;

	minigame->scene = malloc(sizeof(Scene));
	if (minigame->scene == NULL) {
		printf("Failed to allocate memory for menu scene.\n");
		exit(1);
	}
	if (!init_kumashoot_scene(minigame->scene, kumashoot,
				  engine->resource_cache)) {
		printf("Failed to initialize kuma shoot scene.\n");
		exit(1);
	};

	minigame->minigame_struct = kumashoot;
	minigame->updater = test;
}
