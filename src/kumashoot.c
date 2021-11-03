#include <stdio.h>
#include <stdlib.h>

#include "kumashoot.h"
#include "texture.h"
#include "window.h"

static float float_rand(float min, float max)
{
	float scale = rand() / (float)RAND_MAX;
	return min + scale * (max - min);
}

static void vaporize_character(Texture *textures, KumaShootSprite *character)
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

static void spawn_character(Texture *textures, KumaShootSprite *character,
			    Vector2D pos)
{
	Texture *texture;
	uint8_t spritesheet_max_frame;

	switch (character->type) {
	case WHITE_BEAR: {
		texture = &textures[WHITE_BEAR_MOVE_RIGHT];
		spritesheet_max_frame = 7;
		break;
	}
	case BROWN_BEAR: {
		texture = &textures[BROWN_BEAR_MOVE_RIGHT];
		spritesheet_max_frame = 7;
		break;
	}
	case MIHO: {
		texture = &textures[KUMA_SHOOT_MIHO];
		spritesheet_max_frame = 6;
		break;
	}
	case YASUO: {
		texture = &textures[KUMA_SHOOT_YASUO];
		spritesheet_max_frame = 2;
		break;
	}

	case MIKA: {
		texture = &textures[KUMA_SHOOT_MIKA];
		spritesheet_max_frame = 2;
		break;
	}
	// todo lain here is placeholder
	case LAIN:
	case LAIN_WITH_SCREWDRIVER: {
		texture = &textures[KUMA_SHOOT_SCREWDRIVER_LAIN];
		spritesheet_max_frame = 13;
		break;
	}
	}

	printf("%d", character->type);

	character->sprite = (Sprite){.pos = pos,
				     .size = {96.0f, 128.0f},
				     .texture = texture,
				     .visible = true,
				     .is_spritesheet = true,
				     .max_frame = spritesheet_max_frame,
				     .z_index = 1};

	initialize_sprite(&character->sprite);
}

// TODO RE properly
static void get_random_pos(Vector2D *pos)
{
	pos->x = float_rand(150.0f, KUMASHOOT_WIDTH - 150.0f);
	pos->y = float_rand(100.0f, KUMASHOOT_HEIGHT - 100.0f);
}

// TODO RE properly
inline static KumaShootSpriteType get_random_bear_type()
{
	return rand() < RAND_MAX / 2 ? WHITE_BEAR : BROWN_BEAR;
}

static void kill_bear(Scene *scene, KumaShootSprite *character,
		      GameState *game_state, Texture *textures)
{
	vaporize_character(textures, character);

	float chance = float_rand(0.0f, 1.0f);
	if (character->type == BROWN_BEAR) {
		if (chance <= 3.0f / 16.0f) {
			character->type = YASUO;
		} else if (chance <= 5.0f / 16.0f) {
			character->type = MIHO;
		} else if (chance <= 6.0f / 16.0f) {
			character->type = MIKA;
		} else if (chance <= 7.0f / 16.0f) {
			character->type = LAIN;
		} else if (chance <= 8.0f / 16.0f) {
			// todo check if holding screwdriver
			character->type = LAIN_WITH_SCREWDRIVER;
		} else {
			// todo give 5 points
			character->type = BROWN_BEAR;
		}
	} else if (character->type == WHITE_BEAR) {
		if (chance <= 4.0f / 16.0f) {
			character->type = YASUO;
		} else if (chance <= 6.0f / 16.0f) {
			character->type = MIHO;
		} else if (chance <= 8.0f / 16.0f) {
			character->type = MIKA;
		} else if (chance <= 9.0f / 16.0f) {
			character->type = LAIN;
		} else if (chance <= 11.0f / 16.0f) {
			// todo check if holding screwdriver
			character->type = LAIN_WITH_SCREWDRIVER;
		} else {
			// todo give 10 points
			character->type = WHITE_BEAR;
		}
	}
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
		Vector2D pos;
		get_random_pos(&pos);

		kumashoot->characters[i].type = get_random_bear_type();
		kumashoot->characters[i].vaporized = false;

		spawn_character(textures, &kumashoot->characters[i], pos);
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

static void update_kumashoot(Texture *textures, void *minigame_struct,
			     GameState *game_state)
{
	KumaShoot *kumashoot = (KumaShoot *)minigame_struct;

	for (int i = 0; i < 3; i++) {
		KumaShootSprite *curr = &kumashoot->characters[i];
		if (curr->vaporized) {
			if (curr->sprite.current_frame ==
			    curr->sprite.max_frame) {
				curr->sprite.visible = false;
				curr->vaporized = false;
				spawn_character(textures, curr,
						curr->sprite.pos);
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

void handle_kumashoot_event(KumaShootEvent event, KumaShootSprite *character,
			    Engine *engine)
{
	if (event == CHARACTER_CLICK &&
	    (character->type == WHITE_BEAR || character->type == BROWN_BEAR)) {
		kill_bear(engine->minigame.scene, character,
			  &engine->game_state, engine->textures);
	}
}
