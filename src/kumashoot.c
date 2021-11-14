#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "animations.h"
#include "cvector.h"
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

inline static BearType get_random_bear_type()
{
	return random_int_in_range(0, 2) == 1;
}

static CharacterType get_random_character_type(GameState *game_state,
					       BearType bear_type)
{
	int rand_percent = random_int_in_range(0, 101);

	switch (bear_type) {
	case BROWN_BEAR: {
		if (rand_percent <= 19) {
			return YASUO;
		} else if (rand_percent <= 31) {
			return MIHO;
		} else if (rand_percent <= 38) {
			return MIKA;
		} else if (rand_percent <= 43) {
			return SCHOOL_LAIN;
		} else if (rand_percent <= 50) {
			// todo check if holding screwdriver
			return DEFAULT_LAIN;
		} else {
			return NO_CHARACTER;
		}
	}
	case WHITE_BEAR: {
		if (rand_percent <= 25) {
			return YASUO;
		} else if (rand_percent <= 38) {
			return MIHO;
		} else if (rand_percent <= 50) {
			return MIKA;
		} else if (rand_percent <= 56) {
			return SCHOOL_LAIN;
		} else if (rand_percent <= 69) {
			// todo check if holding screwdriver
			return DEFAULT_LAIN;
		} else {
			return NO_CHARACTER;
		}
	}
	}
}

static void create_character(Resources *resources, GameState *game_state,
			     BearType bear_type, Character *character)
{
	character->type = get_random_character_type(game_state, bear_type);

	Texture *textures = resources->textures;
	Animation *animations = resources->animations;

	Sprite *sprite = &character->sprite;
	*sprite = (Sprite){.size = {96.0f, 128.0f},
			   .visible = false,
			   .pivot_centered = true,
			   .is_spritesheet = true,
			   .z_index = 3};

	switch (character->type) {
	case NO_CHARACTER:
		character->score_value = bear_type == WHITE_BEAR ? 10 : 5;
		break;
	case MIHO: {
		sprite->texture = &textures[KUMA_SHOOT_MIHO];
		sprite->max_frame = 6;
		sprite_set_animation(sprite,
				     &animations[KUMA_SHOOT_MIHO_ANIMATION]);

		character->score_value = -5;
		break;
	}
	case YASUO: {
		sprite->texture = &textures[KUMA_SHOOT_YASUO];
		sprite->max_frame = 2;
		sprite_set_animation(sprite,
				     &animations[KUMA_SHOOT_YASUO_ANIMATION]);

		character->score_value = -1;
		break;
	}
	case MIKA: {
		sprite->texture = &textures[KUMA_SHOOT_MIKA];
		sprite->max_frame = 2;
		sprite_set_animation(sprite,
				     &animations[KUMA_SHOOT_MIKA_ANIMATION]);

		character->score_value = -10;
		break;
	}
	case SCHOOL_LAIN:
		sprite->texture = &textures[KUMA_SHOOT_SCHOOL_LAIN];
		sprite->max_frame = 12;
		sprite_set_animation(
		    sprite, &animations[KUMA_SHOOT_SCHOOL_LAIN_ANIMATION]);

		character->score_value = 0;
		break;
	case DEFAULT_LAIN: {
		sprite->texture = &textures[KUMA_SHOOT_DEFAULT_LAIN];
		sprite->max_frame = 7;
		sprite->size = (Vector2D){64.0f, 120.0f};
		sprite_set_animation(
		    sprite, &animations[KUMA_SHOOT_DEFAULT_LAIN_ANIMATION]);

		character->score_value = 0;
		break;
	}
	default:
		break;
	}

	if (character->type != NO_CHARACTER) {
		initialize_sprite(sprite);
	}
}

static void set_random_pos(Bear *bear)
{
	int top = HH - 10;
	int bottom = KUMASHOOT_HEIGHT - (HH + 10);
	int right = KUMASHOOT_WIDTH - HW;
	int left = HW;

	bear->sprite.pos.x = random_int_in_range(left, right);
	bear->sprite.pos.y = random_int_in_range(top, bottom);
}

static void set_random_velocity(Bear *bear)
{
	unsigned int uVar1;
	unsigned int uVar2;

	uVar1 = random_int();
	uVar2 = (int)uVar1 >> 0x1f;
	bear->vel_x = (((uVar1 ^ uVar2) - uVar2 & 7) ^ uVar2) - uVar2;

	uVar1 = random_int();
	uVar2 = (int)uVar1 >> 0x1f;
	bear->vel_y = (((uVar1 ^ uVar2) - uVar2 & 7) ^ uVar2) - uVar2;
}

static void set_sprite_to_smoke(Resources *resources, Sprite *sprite)
{
	Sprite smoke =
	    (Sprite){.pos = sprite->pos,
		     .size = {96.0f, 128.0f},
		     .texture = &resources->textures[KUMA_SHOOT_SMOKE],
		     .visible = true,
		     .pivot_centered = true,
		     .is_spritesheet = true,
		     .max_frame = 4,
		     .z_index = 3};

	sprite_set_animation(
	    &smoke, &resources->animations[KUMA_SHOOT_SMOKE_ANIMATION]);

	initialize_sprite(&smoke);

	*sprite = smoke;
}

static void spawn_bear(Resources *resources, GameState *game_state, Bear *bear)
{
	BearType type = get_random_bear_type();

	*bear = (Bear){
	    .type = type,
	    .sprite = (Sprite){
		.size = {96.0f, 128.0f},
		.visible = true,
		.pivot_centered = true,
		.is_spritesheet = true,
		.max_frame = 7,
		.texture = type == WHITE_BEAR
			       ? &resources->textures[WHITE_BEAR_MOVE_RIGHT]
			       : &resources->textures[BROWN_BEAR_MOVE_RIGHT],
		.z_index = 1}};

	sprite_set_animation(&bear->sprite,
			     &resources->animations[BEAR_ANIMATION]);
	bear->sprite.mirrored = bear->vel_x < 0;
	initialize_sprite(&bear->sprite);

	set_random_pos(bear);
	set_random_velocity(bear);

	create_character(resources, game_state, bear->type,
			 &bear->hidden_character);
}

static void update_bear_position(Bear *bear)
{
	int next_x = bear->vel_x + bear->sprite.pos.x;
	int next_y = bear->vel_y + bear->sprite.pos.y;

	if ((next_x < HW) || (KUMASHOOT_WIDTH - HW < next_x)) {
		next_x = bear->sprite.pos.x;
		bear->vel_x *= -1.0f;
	}

	if ((next_y < (HH - 10)) || (KUMASHOOT_HEIGHT - (HH + 10) < next_y)) {
		next_y = bear->sprite.pos.y;
		bear->vel_y *= -1.0f;
	}

	bear->sprite.pos.x = next_x;
	bear->sprite.pos.y = next_y;
}

static void explode_scene(Resources *resources, Bear bears[3])
{

	for (int i = 0; i < 3; i++) {
		Bear *bear = &bears[i];
		if (!bear->revealed && !bear->is_smoke) {
			bear->hidden_character.score_value *= 10;

			set_sprite_to_smoke(resources, &bear->sprite);
			bear->is_smoke = true;
		}
	}
}

static void update_character(GameState *game_state, Resources *resources,
			     KumaShoot *kumashoot, Bear *bear)
{
	Sprite *sprite = &bear->sprite;
	Character *character = &bear->hidden_character;
	double now = game_state->time;

	if (character->scored) {
		double delta = now - character->time_scored;
		if (delta > 0.5) {
			bear->needs_reset = true;
		}

		return;
	}

	if (character->is_smoke) {
		if (sprite_is_max_frame(sprite)) {
			character->scored = true;
			character->time_scored = now;
		} else {
			sprite_try_next_frame(now, sprite);
		}

		return;
	}

	if (character->exploding) {
		if (sprite_is_max_frame(sprite)) {
			bear->needs_reset = true;
		} else {
			sprite_try_next_frame(now, sprite);
		}

		return;
	}

	switch (character->type) {
	case SCREWDRIVER_LAIN: {
		if (sprite_is_max_frame(sprite)) {
			sprite->size = (Vector2D){128.0f, 128.0f};
			sprite->mirrored = false;
			sprite->frame_index = 0;
			sprite->max_frame = 4;
			sprite->texture =
			    &resources->textures[KUMA_SHOOT_EXPLOSION];

			initialize_sprite(sprite);

			character->exploding = true;

			sprite_set_animation(
			    sprite,
			    &resources
				 ->animations[KUMA_SHOOT_EXPLOSION_ANIMATION]);

			explode_scene(resources, kumashoot->bears);
		} else {
			sprite_try_next_frame(now, sprite);
		}
		break;
	}
	case DEFAULT_LAIN: {
		double delta = now - character->time_revealed;
		if (delta > 2.0) {
			sprite->size = (Vector2D){96.0f, 128.0f};
			// 0th frame doesnt seem to be used in the
			// original
			sprite->mirrored = false;
			sprite->frame_index = 1;
			sprite->max_frame = 8;
			sprite->texture =
			    &resources->textures[KUMA_SHOOT_SCREWDRIVER_LAIN];
			// for some reason this spritesheet is offset
			// by about a quarter of its width to the right
			// we adjust for that here.
			sprite->pos.x -= 20.0f;

			initialize_sprite(sprite);

			sprite_set_animation(
			    sprite,
			    &resources->animations
				 [KUMA_SHOOT_SCREWDRIVER_LAIN_ANIMATION]);

			character->type = SCREWDRIVER_LAIN;
			return;
		}

		update_bear_position(bear);

		bear->sprite.mirrored = bear->vel_x < 0;

		if ((random_int() & 0x3f) == 0) {
			set_random_velocity(bear);
		}

		sprite_try_next_frame(now, sprite);

		break;
	}
	case SCHOOL_LAIN:
		if ((sprite->pos.x < HW) ||
		    (KUMASHOOT_WIDTH - HW < sprite->pos.x)) {
			// walked off screen
			bear->needs_reset = true;
			return;
		}

		sprite_try_next_frame(now, sprite);

		if (sprite->frame_index > 3) {
			sprite->pos.x += bear->vel_x;
		}

		break;
	default:
		double delta = now - character->time_revealed;
		if (sprite_is_max_frame(sprite) && delta > 2.0) {
			set_sprite_to_smoke(resources, sprite);
			character->is_smoke = true;
		} else {
			sprite_try_next_frame(now, sprite);
		}
	}
}

static void reveal_bear(GameState *game_state, Bear *bear)
{
	Character *hidden_character = &bear->hidden_character;
	if (hidden_character->type == NO_CHARACTER) {
		hidden_character->time_scored = game_state->time;
		hidden_character->scored = true;
		hidden_character->sprite.visible = false;

		bear->sprite.visible = false;
		bear->revealed = true;
	} else {
		hidden_character->time_revealed = game_state->time;
		hidden_character->sprite.pos = bear->sprite.pos;
		hidden_character->sprite.visible = true;

		bear->sprite = hidden_character->sprite;
		bear->revealed = true;
	}

	switch (hidden_character->type) {
	case SCHOOL_LAIN: {
		unsigned int uVar3 = random_int();
		bear->vel_x = (-(unsigned int)((uVar3 & 1) != 0) & 10) - 5;
		bear->sprite.mirrored = bear->vel_x < 0;
		break;
	}
	case DEFAULT_LAIN: {
		set_random_velocity(bear);
		break;
	}
	default:
		break;
	}
}

static void update_bear(GameState *game_state, Scene *scene, Bear *bear)
{
	if (bear->is_smoke) {
		if (sprite_is_max_frame(&bear->sprite)) {
			reveal_bear(game_state, bear);
			depth_sort(scene->sprites,
				   cvector_size(scene->sprites));
		} else {
			sprite_try_next_frame(game_state->time, &bear->sprite);
		}
	} else {
		update_bear_position(bear);

		bear->sprite.mirrored = bear->vel_x < 0;

		sprite_try_next_frame(game_state->time, &bear->sprite);

		if ((random_int() & 0x3f) == 0) {
			set_random_velocity(bear);
		}
	}
}

static void update_kumashoot(Resources *resources, void *minigame_struct,
			     GameState *game_state)
{
	KumaShoot *kumashoot = (KumaShoot *)minigame_struct;
	Scene *scene = &kumashoot->scene;

	for (int i = 0; i < 3; i++) {
		Bear *bear = &kumashoot->bears[i];
		Text *score_text = &kumashoot->score_displays[i];

		if (bear->needs_reset) {
			spawn_bear(resources, game_state, &kumashoot->bears[i]);
			depth_sort(scene->sprites,
				   cvector_size(scene->sprites));

			score_text->visible = false;
		} else if (bear->revealed) {
			Character *character = &bear->hidden_character;

			if (character->scored) {
				sprintf(score_text->current_text, "%d",
					character->score_value);

				score_text->pos = bear->sprite.pos;
				score_text->visible = true;
			}

			update_character(game_state, resources, kumashoot,
					 bear);
		} else {
			update_bear(game_state, scene, bear);
		}
	}

	update_scene(scene);
}

static void init_kumashoot_sprites(Resources *resources, GameState *game_state,
				   KumaShoot *kumashoot)
{
	kumashoot->background =
	    (Sprite){.pos = {0.0f, 0.0f},
		     .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
		     .texture = &resources->textures[KUMA_SHOOT_BG],
		     .visible = true,
		     .z_index = 0};

	kumashoot->bush_overlay = (Sprite){
	    .pos = {0.0f, 0.0f},
	    .size = {KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT},
	    .texture = &resources->textures[KUMA_SHOOT_BUSH_OVERLAY],
	    .visible = true,
	    .z_index = 2,
	};

	for (int i = 0; i < 3; i++) {
		spawn_bear(resources, game_state, &kumashoot->bears[i]);
	}
}

static void init_kumashoot_scene(Resources *resources, GameState *game_state,
				 Scene *scene, KumaShoot *kumashoot)
{
	init_kumashoot_sprites(resources, game_state, kumashoot);

	Sprite *sprites[] = {&kumashoot->background, &kumashoot->bush_overlay,
			     &kumashoot->bears[0].sprite,
			     &kumashoot->bears[1].sprite,
			     &kumashoot->bears[2].sprite};

	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	// TODO if "keep bugs" flag enabled barrier 8 shouldnt exist
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
		    (SpriteBehavior){.sprite = &kumashoot->bears[i].sprite,
				     .click_event = CHARACTER_CLICK,
				     .object = &kumashoot->bears[i]};
	}
	uint8_t sprite_behavior_count =
	    sizeof(sprite_behaviors) / sizeof(sprite_behaviors[0]);

	for (int i = 0; i < 3; i++) {
		kumashoot->score_displays[i] =
		    (Text){.glyph_size = {10.0f, 16.0f},
			   .visible = false,
			   .left_aligned = true,
			   .font = &resources->fonts[RED_FONT]};
	}

	Text *score_displays[] = {
	    &kumashoot->score_displays[0],
	    &kumashoot->score_displays[1],
	    &kumashoot->score_displays[2],
	};

	uint8_t score_display_count =
	    sizeof(score_displays) / sizeof(score_displays[0]);

	init_scene(scene, sprites, sprite_count, sprite_behaviors,
		   sprite_behavior_count, score_displays, score_display_count,
		   click_barriers, click_barrier_count);
}

void start_kumashoot(Resources *resources, GameState *game_state,
		     Minigame *minigame, GLFWwindow **minigame_window,
		     GLFWwindow *main_window)
{
	if (!(make_window(minigame_window, KUMASHOOT_WIDTH, KUMASHOOT_HEIGHT,
			  "lain kuma shoot", main_window, true))) {
		printf("Failed to create kuma shoot window.\n");
		exit(1);
	}

	KumaShoot *kumashoot = malloc(sizeof(KumaShoot));
	if (kumashoot == NULL) {
		printf("Failed to allocate memory kuma shoot struct.\n");
		exit(1);
	}

	init_kumashoot_scene(resources, game_state, &kumashoot->scene,
			     kumashoot);

	minigame->current = kumashoot;
	minigame->update = update_kumashoot;
	minigame->scene = &kumashoot->scene;
	minigame->type = KUMASHOOT;
	minigame->refresh_rate = 30.0;
	minigame->last_updated = game_state->time;
}

void handle_kumashoot_event(KumaShootEvent event, Bear *bear, Engine *engine)
{
	if (event == CHARACTER_CLICK && !bear->revealed && !bear->is_smoke) {
		KumaShoot *kumashoot = (KumaShoot *)engine->minigame.current;

		set_sprite_to_smoke(&engine->resources, &bear->sprite);
		bear->is_smoke = true;

		depth_sort(kumashoot->scene.sprites,
			   cvector_size(kumashoot->scene.sprites));
	}
}
