#include <stdio.h>
#include <stdlib.h>

#include "kumashoot.h"
#include "texture.h"
#include "window.h"

static float float_rand(float min, float max)
{
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	return min + scale * (max - min);	/* [min, max] */
}

static void spawn_bear(KumaShootSprite *character, Texture *textures)
{

	KumaShootSpriteType bear_type;
	Texture *texture;

	// todo randomness here is bad
	// todo we havent reversed the rate of brown/white bear spawns
	if (rand() < RAND_MAX / 2) {
		bear_type = WHITE_BEAR;
		texture = &textures[WHITE_BEAR_MOVE_RIGHT];
	} else {
		bear_type = BROWN_BEAR;
		texture = &textures[BROWN_BEAR_MOVE_RIGHT];
	}

	GLfloat pos_x = float_rand(150.0f, KUMASHOOT_WIDTH - 150.0f);
	GLfloat pos_y = float_rand(100.0f, KUMASHOOT_HEIGHT - 100.0f);

	character->sprite = (Sprite){.pos = {pos_x, pos_y},
				     .size = {96.0f, 128.0f},
				     .texture = texture,
				     .visible = true,
				     .is_spritesheet = true,
				     .max_frame = 7,
				     .z_index = 1};
	character->type = bear_type;
	character->vaporized = false;
}

static void init_kumashoot_sprites(KumaShoot *kumashoot, Texture *textures)
{
	kumashoot->background =
	    (Sprite){.pos = {0.0f, 0.0f},
		     .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
		     .texture = &textures[KUMA_SHOOT_BG],
		     .visible = true,
		     .z_index = 0};

	kumashoot->bush_overlay = (Sprite){
	    .pos = {0.0f, 0.0f},
	    .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
	    .texture = &textures[KUMA_SHOOT_BUSH_OVERLAY],
	    .visible = true,
	    .z_index = 2,
	};

	for (int i = 0; i < 3; i++) {
		spawn_bear(&kumashoot->characters[i], textures);
	}
}

static void animate_bear(KumaShootSprite *bear)
{
	Sprite *sprite = &bear->sprite;
	if (sprite->current_frame == sprite->max_frame) {
		sprite->current_frame = 0;
	}
	sprite->current_frame++;
}

static void init_kumashoot_scene(Scene *scene, KumaShoot *kumashoot,
				 Texture *textures)
{
	init_kumashoot_sprites(kumashoot, textures);

	Sprite *sprites[] = {
	    &kumashoot->background,	      &kumashoot->bush_overlay,
	    &kumashoot->characters[0].sprite, &kumashoot->characters[1].sprite,
	    &kumashoot->characters[2].sprite,
	};

	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	SpriteBehavior sprite_behaviors[3];

	for (int i = 0; i < 3; i++) {
		sprite_behaviors[i] =
		    (SpriteBehavior){.sprite = &kumashoot->characters[i].sprite,
				     .click_event = CHARACTER_CLICK,
				     .object = &kumashoot->characters[i]};
	}
	uint8_t sprite_behavior_count =
	    sizeof(sprite_behaviors) / sizeof(sprite_behaviors[0]);

	init_scene(scene, sprites, sprite_count, sprite_behaviors,
		   sprite_behavior_count, NULL, 0);
}

static void update_kumashoot(void *minigame_struct, GameState *game_state)
{
	KumaShoot *kumashoot = (KumaShoot *)minigame_struct;

	for (int i = 0; i < 3; i++) {
		KumaShootSprite *curr = &kumashoot->characters[i];
		if (curr->vaporized) {
			if (curr->sprite.current_frame ==
			    curr->sprite.max_frame) {
				curr->sprite.visible = false;
			} else {
				curr->sprite.current_frame++;
			}
		} else {
			switch (curr->type) {
			case BROWN_BEAR:
			case WHITE_BEAR:
				animate_bear(&kumashoot->characters[i]);
				break;
			}
		}
	}

	update_scene(&kumashoot->scene);
}

static KumaShoot *create_kumashoot(Texture *textures)
{
	KumaShoot *kumashoot = malloc(sizeof(KumaShoot));
	if (kumashoot == NULL) {
		printf("Failed to allocate memory kuma shoot struct.\n");
		exit(1);
	}

	init_kumashoot_scene(&kumashoot->scene, kumashoot, textures);

	return kumashoot;
}

void start_kumashoot(Minigame *minigame, GLFWwindow **minigame_window,
		     GLFWwindow *main_window, Texture *textures)
{
	if (!(make_window(minigame_window, KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT,
			  "lain kuma shoot", main_window, true))) {
		printf("Failed to create kuma shoot window.\n");
		exit(1);
	}

	KumaShoot *kumashoot = create_kumashoot(textures);

	minigame->current = kumashoot;
	minigame->update = update_kumashoot;
	minigame->scene = &kumashoot->scene;
	minigame->type = KUMASHOOT;
}

static void vaporize_character(Scene *scene, KumaShootSprite *character,
			       Texture *textures)
{
	Sprite smoke = (Sprite){.pos = character->sprite.pos,
				.size = {96.0f, 128.0f},
				.texture = &textures[SMOKE],
				.visible = true,
				.is_spritesheet = true,
				.max_frame = 3,
				.z_index = 1};

	initialize_sprite(&smoke);

	character->sprite = smoke;
	character->vaporized = true;
}

void handle_kumashoot_event(KumaShootEvent event, KumaShootSprite *character,
			    Engine *engine)
{
	switch (event) {
	case CHARACTER_CLICK: {
		if (character->type == WHITE_BEAR ||
		    character->type == BROWN_BEAR) {
			vaporize_character(engine->minigame.scene, character,
					   engine->textures);
			break;
		}
	}
	}
}
