#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kumashoot.h"
#include "random.h"
#include "sprite.h"
#include "texture.h"
#include "vector2d.h"
#include "window.h"

// TOOD macros for now
// should be sprite hitbox/2
#define HW 24
#define HH 40

inline static _Bool is_bear(KumaShootSprite *character)
{
	return character->type == WHITE_BEAR || character->type == BROWN_BEAR;
}

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
				.pivot_centered = true,
				.is_spritesheet = true,
				.max_frame = 3,
				.z_index = 1};

	initialize_sprite(&smoke);

	character->sprite = smoke;
	character->vaporized = true;
}

static void spawn_character(Texture *textures, KumaShootSprite *character)
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

	character->sprite = (Sprite){.pos = character->sprite.pos,
				     .size = {96.0f, 128.0f},
				     .texture = texture,
				     .visible = true,
				     .pivot_centered = true,
				     .is_spritesheet = true,
				     .max_frame = spritesheet_max_frame,
				     .z_index = is_bear(character) ? 1 : 3};

	initialize_sprite(&character->sprite);

	character->animation_start_time = glfwGetTime();
}

static void set_random_pos(KumaShootSprite *character)
{
	int iVar1;
	int iVar2;

	int top = HH - 10;
	int bottom = KUMASHOOT_HEIGHT - (HH + 10);
	int right = KUMASHOOT_WIDTH - HW;
	int left = HW;

	do {
		do {
			iVar1 = random_int();
			iVar1 = iVar1 % (right - left);
			character->sprite.pos.x = iVar1;
		} while (iVar1 < left);
	} while (right < iVar1);

	do {
		do {
			iVar1 = random_int();
			iVar2 = iVar1 % (bottom - top);
			character->sprite.pos.y = iVar2;
		} while (iVar2 < top);
	} while (bottom < iVar2);
}

static void set_random_velocity(KumaShootSprite *character)
{
	unsigned int uVar1;
	unsigned int uVar2;

	uVar1 = random_int();
	uVar2 = (int)uVar1 >> 0x1f;
	character->dx = (((uVar1 ^ uVar2) - uVar2 & 7) ^ uVar2) - uVar2;

	uVar1 = random_int();
	uVar2 = (int)uVar1 >> 0x1f;
	character->dy = (((uVar1 ^ uVar2) - uVar2 & 7) ^ uVar2) - uVar2;
}

static void update_bear_position(KumaShootSprite *bear)
{
	int next_x = bear->dx + bear->sprite.pos.x;
	int next_y = bear->dy + bear->sprite.pos.y;

	if ((next_x < HW) || (KUMASHOOT_WIDTH - HW < next_x)) {
		next_x = bear->sprite.pos.x;
		bear->dx = -bear->dx;
	}

	if ((next_y < (HH - 10)) || (KUMASHOOT_HEIGHT - (HH + 10) < next_y)) {
		next_y = bear->sprite.pos.y;
		bear->dy = -bear->dy;
	}

	if ((random_int() & 0x3f) == 0) {
		set_random_velocity(bear);
	}

	bear->sprite.mirrored = bear->sprite.pos.x > next_x;

	bear->sprite.pos.x = next_x;
	bear->sprite.pos.y = next_y;
}

// TODO RE properly
inline static KumaShootSpriteType get_random_bear_type()
{
	return rand() < RAND_MAX / 2 ? WHITE_BEAR : BROWN_BEAR;
}

static void shoot_bear(Scene *scene, KumaShootSprite *character,
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

static void update_character(Texture *textures, Scene *scene,
			     KumaShootSprite *character)
{
	Sprite *sprite = &character->sprite;

	if (character->vaporized) {
		if (sprite->current_frame == sprite->max_frame) {
			character->vaporized = false;
			if (is_bear(character)) {
				set_random_pos(character);
			}
			spawn_character(textures, character);

			depth_sort(scene->sprites,
				   cvector_size(scene->sprites));

		} else {
			sprite->current_frame++;
		}
		return;
	}

	if (is_bear(character)) {
		if (sprite->current_frame == sprite->max_frame) {
			sprite->current_frame = 0;
		}
		sprite->current_frame++;
		update_bear_position(character);
	} else {
		if (sprite->current_frame == sprite->max_frame) {
			double delta =
			    glfwGetTime() - character->animation_start_time;
			if (delta < 2.0f) {
				sprite->current_frame = 0;
			} else {
				vaporize_character(textures, character);
				character->type = get_random_bear_type();
			}
		}
		sprite->current_frame++;
	}
}

static void update_kumashoot(Texture *textures, void *minigame_struct,
			     GameState *game_state)
{
	KumaShoot *kumashoot = (KumaShoot *)minigame_struct;
	Scene *scene = &kumashoot->scene;

	for (int i = 0; i < 3; i++) {
		update_character(textures, scene, &kumashoot->characters[i]);
	}

	update_scene(scene);
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
		KumaShootSprite *character = &kumashoot->characters[i];

		character->type = get_random_bear_type();
		character->vaporized = false;

		spawn_character(textures, character);

		set_random_pos(character);
		set_random_velocity(character);
	}
}

static void init_kumashoot_scene(Scene *scene, KumaShoot *kumashoot,
				 Texture *textures)
{
	init_kumashoot_sprites(kumashoot, textures);

	// TODO if "keep bugs" flag enabled barrier 8 shouldnt exist
	Sprite *sprites[] = {&kumashoot->background, &kumashoot->bush_overlay,
			     &kumashoot->characters[0].sprite,
			     &kumashoot->characters[1].sprite,
			     &kumashoot->characters[2].sprite};

	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	Sprite click_barriers[] = {
	    make_click_barrier(76, 28, 164, 83),
	    make_click_barrier(48, 82, 295, 113),
	    make_click_barrier(240, 62, 288, 82),
	    make_click_barrier(81, 222, 167, 246),
	    make_click_barrier(57, 246, 208, 301),
	    make_click_barrier(485, 88, 557, 128),
	    make_click_barrier(466, 128, 563, 170),
	    make_click_barrier(563, 110, 582, 170),
	    make_click_barrier(446, 321, 600, 400),
	};
	uint8_t click_barrier_count =
	    sizeof(click_barriers) / sizeof(click_barriers[0]);

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
		   sprite_behavior_count, NULL, 0, click_barriers,
		   click_barrier_count);
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
	if (event == CHARACTER_CLICK && is_bear(character) &&
	    !character->vaporized) {
		shoot_bear(engine->minigame.scene, character,
			   &engine->game_state, engine->textures);
	}
}
