#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "animations.h"
#include "cvector.h"
#include "kumashoot.h"
#include "minigame.h"
#include "random.h"
#include "resources.h"
#include "scene.h"
#include "sprite.h"
#include "state.h"
#include "text.h"
#include "texture.h"
#include "vector2d.h"
#include "window.h"

inline static BearType get_random_bear_type()
{
	return random_int_in_range(0, 2) == 1;
}

inline static float half_width(Bear *bear)
{
	return bear->sprite.hitbox_size.x / 2.0f;
}

inline static float half_height(Bear *bear)
{
	return bear->sprite.hitbox_size.y / 2.0f;
}

static CharacterType get_random_character_type(GameState *game_state,
					       BearType bear_type)
{
	int rand_percent = random_int_in_range(0, 101);

	switch (bear_type) {
	case BEAR_BROWN: {
		if (rand_percent <= 19) {
			return YASUO;
		} else if (rand_percent <= 31) {
			return MIHO;
		} else if (rand_percent <= 38) {
			return MIKA;
		} else if (rand_percent <= 43) {
			return SCHOOL_LAIN_STANDING;
		} else if (rand_percent <= 50) {
			if (game_state->lain.tool_state ==
			    HOLDING_SCREWDRIVER) {
				return DEFAULT_LAIN;
			} else {
				return SCHOOL_LAIN_STANDING;
			}
		} else {
			return NO_CHARACTER;
		}
	}
	case BEAR_WHITE: {
		if (rand_percent <= 25) {
			return YASUO;
		} else if (rand_percent <= 38) {
			return MIHO;
		} else if (rand_percent <= 50) {
			return MIKA;
		} else if (rand_percent <= 56) {
			return SCHOOL_LAIN_STANDING;
		} else if (rand_percent <= 69) {
			if (game_state->lain.tool_state ==
			    HOLDING_SCREWDRIVER) {
				return DEFAULT_LAIN;
			} else {
				return SCHOOL_LAIN_STANDING;
			}
		} else {
			return NO_CHARACTER;
		}
	}
	}
}

static void create_character(Resources *resources, GameState *game_state,
			     BearType bear_type, Character *character)
{
	double now = game_state->time;

	*character = (Character){0};

	character->type = get_random_character_type(game_state, bear_type);
	character->additional_sprite = (Sprite){0};

	Sprite *sprite = &character->sprite;
	make_sprite(
	    sprite,
	    (Sprite){.visible = false, .pivot_centered = true, .z_index = 4});

	switch (character->type) {
	case NO_CHARACTER:
		character->score_value = bear_type == BEAR_WHITE ? 10 : 5;
		break;
	case MIHO: {
		sprite_set_animation(resources, now, sprite,
				     KUMA_SHOOT_MIHO_ANIMATION);

		character->score_value = -5;
		break;
	}
	case YASUO: {
		sprite_set_animation(resources, now, sprite,
				     KUMA_SHOOT_YASUO_ANIMATION);

		character->score_value = -1;
		break;
	}
	case MIKA: {
		sprite_set_animation(resources, now, sprite,
				     KUMA_SHOOT_MIKA_ANIMATION);

		character->score_value = -10;
		break;
	}
	case SCHOOL_LAIN_STANDING:
		sprite_set_animation(resources, now, sprite,
				     KUMA_SHOOT_SCHOOL_LAIN_1_ANIMATION);

		character->score_value = 0;
		break;
	case DEFAULT_LAIN: {
		sprite_set_animation(resources, now, sprite,
				     KUMA_SHOOT_DEFAULT_LAIN_ANIMATION);

		character->score_value = 0;
		break;
	}
	default:
		break;
	}

	if (sprite->animation != NULL) {
		sprite->animation->looped = true;
	}
}

static void set_random_pos(Bear *bear)
{
	int top = half_height(bear) - 10;
	int bottom = MINIGAME_HEIGHT - (half_height(bear) + 10);
	int right = MINIGAME_WIDTH - half_width(bear);
	int left = half_width(bear);

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

static void set_sprite_to_smoke(Resources *resources, double now,
				Sprite *sprite)
{
	sprite->z_index = 3;

	sprite_set_animation(resources, now, sprite,
			     KUMA_SHOOT_SMOKE_ANIMATION);
}

static void spawn_bear(Resources *resources, GameState *game_state, Bear *bear)
{
	*bear = (Bear){0};

	bear->type = get_random_bear_type();

	make_sprite(&bear->sprite, (Sprite){.hitbox_size = {48.0f, 80.0f},
					    .visible = true,
					    .pivot_centered = true,
					    .z_index = 1});

	sprite_set_animation(resources, game_state->time, &bear->sprite,
			     bear->type == BEAR_BROWN
				 ? KUMA_SHOOT_BROWN_BEAR_WALK_ANIMATION
				 : KUMA_SHOOT_WHITE_BEAR_WALK_ANIMATION);
	bear->sprite.animation->looped = true;

	bear->sprite.mirrored = bear->vel_x < 0;

	set_random_pos(bear);
	set_random_velocity(bear);

	create_character(resources, game_state, bear->type,
			 &bear->hidden_character);
}

static void update_bear_position(Bear *bear)
{
	int next_x = bear->vel_x + bear->sprite.pos.x;
	int next_y = bear->vel_y + bear->sprite.pos.y;

	if ((next_x < half_width(bear)) ||
	    (MINIGAME_WIDTH - half_width(bear) < next_x)) {
		next_x = bear->sprite.pos.x;
		bear->vel_x *= -1.0f;
	}

	if ((next_y < (half_height(bear) - 10)) ||
	    (MINIGAME_HEIGHT - (half_height(bear) + 10) < next_y)) {
		next_y = bear->sprite.pos.y;
		bear->vel_y *= -1.0f;
	}

	bear->sprite.pos.x = next_x;
	bear->sprite.pos.y = next_y;
}

static void explode_scene(GameState *game_state, Resources *resources,
			  Bear bears[3])
{
	for (int i = 0; i < 3; i++) {
		Bear *bear = &bears[i];
		if (!bear->revealed && !bear->is_smoke) {
			bear->hidden_character.score_value *= 10;

			set_sprite_to_smoke(resources, game_state->time,
					    &bear->sprite);
			bear->is_smoke = true;
		}
	}
}

static void update_progress(GameState *game_state, int by)
{
	int item_unlock_points[] = {10, 100, 200, 500, 900, 1400, 2000};

	game_state->score += by;
	_Bool new_item_unlocked = false;
	for (int i = 0; i < 7; i++) {
		if (game_state->score >= item_unlock_points[i]) {
			switch (item_unlock_points[i]) {
			case 10: {
				if (!game_state->screwdriver_unlocked) {
					game_state->screwdriver_unlocked = true;
					new_item_unlocked = true;
				}
				break;
			}
			case 100: {
				if (!game_state->school_outfit_unlocked) {
					game_state->school_outfit_unlocked =
					    true;
					new_item_unlocked = true;
				}
				break;
			}
			case 200: {
				if (!game_state->cyberia_outfit_unlocked) {
					game_state->cyberia_outfit_unlocked =
					    true;
					new_item_unlocked = true;
				}
				break;
			}
			case 500: {
				if (!game_state->bear_outfit_unlocked) {
					game_state->bear_outfit_unlocked = true;
					new_item_unlocked = true;
				}
				break;
			}
			case 900: {
				if (!game_state->navi_unlocked) {
					game_state->navi_unlocked = true;
					new_item_unlocked = true;
				}
				break;
			}
			case 1400: {
				if (!game_state->alien_outfit_unlocked) {
					game_state->alien_outfit_unlocked =
					    true;
					new_item_unlocked = true;
				}
				break;
			}
			case 2000: {
				if (!game_state->sweater_outfit_unlocked) {
					game_state->sweater_outfit_unlocked =
					    true;
					new_item_unlocked = true;
				}
				break;
			}
			}
		}
	}

	if (new_item_unlocked) {
		enqueue_sound(&game_state->queued_sounds, SND_120);
	}
}

static void update_character(Scene *scene, GameState *game_state,
			     Resources *resources, KumaShoot *kumashoot,
			     Bear *bear)
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
		if (sprite_animation_is_last_frame(sprite)) {
			character->scored = true;
			character->time_scored = now;
			update_progress(game_state, character->score_value);
		} else {
			sprite_try_next_frame(resources, now, sprite);
		}

		return;
	}

	switch (character->type) {
	case SCREWDRIVER_LAIN: {
		if (sprite->animation_frame->index == 20 &&
		    !character->exploded) {
			enqueue_sound(&game_state->queued_sounds, SND_119);
			character->exploded = true;
		}

		if (sprite->animation_frame->index == 6 &&
		    !character->has_additional_sprite) {
			Sprite screws;
			make_sprite(&screws, (Sprite){.visible = true,
						      .pivot_centered = true,
						      .z_index = 3,
						      .pos = sprite->pos});
			sprite_set_animation(resources, now, &screws,
					     KUMA_SHOOT_SCREW_ANIMATION);

			character->additional_sprite = screws;
			character->additional_sprite.animation->looped = true;
			character->has_additional_sprite = true;
			depth_sort(scene->sprites,
				   cvector_size(scene->sprites));
		}
		if (sprite_animation_is_last_frame(sprite)) {
			sprite->mirrored = false;

			bear->needs_reset = true;
			explode_scene(game_state, resources, kumashoot->bears);
		} else {
			if (character->additional_sprite.animation != NULL) {
				sprite_try_next_frame(
				    resources, now,
				    &character->additional_sprite);
			}
			sprite_try_next_frame(resources, now, sprite);
		}
		break;
	}
	case DEFAULT_LAIN: {
		double delta = now - character->time_revealed;
		if (delta > 2.0) {
			sprite->mirrored = false;

			sprite_set_animation(
			    resources, now, sprite,
			    KUMA_SHOOT_SCREWDRIVER_LAIN_ANIMATION);

			character->type = SCREWDRIVER_LAIN;
			return;
		}

		update_bear_position(bear);

		bear->sprite.mirrored = bear->vel_x < 0;

		if ((random_int() & 0x3f) == 0) {
			set_random_velocity(bear);
		}

		sprite_try_next_frame(resources, now, sprite);

		break;
	}
	case SCHOOL_LAIN_STANDING:
		if (sprite_animation_is_last_frame(sprite)) {
			character->type = SCHOOL_LAIN;
			sprite_set_animation(
			    resources, now, sprite,
			    KUMA_SHOOT_SCHOOL_LAIN_2_ANIMATION);
			sprite->animation->looped = true;
			sprite->mirrored = bear->vel_x < 0;
		} else {
			sprite_try_next_frame(resources, now, sprite);
		}
		break;
	case SCHOOL_LAIN:
		if ((sprite->pos.x < half_width(bear)) ||
		    (MINIGAME_WIDTH - half_width(bear) < sprite->pos.x)) {
			// walked off screen
			bear->needs_reset = true;
			return;
		}

		sprite_try_next_frame(resources, now, sprite);

		sprite->pos.x += bear->vel_x;

		break;
	default: {
		double delta = now - character->time_revealed;
		if (delta > 2.0) {
			set_sprite_to_smoke(resources, now, sprite);
			character->is_smoke = true;
		} else {
			sprite_try_next_frame(resources, now, sprite);
		}
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

		update_progress(game_state, hidden_character->score_value);

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
	case SCHOOL_LAIN_STANDING: {
		unsigned int uVar3 = random_int();
		bear->vel_x = (-(unsigned int)((uVar3 & 1) != 0) & 10) - 5;
		bear->sprite.mirrored = bear->vel_x < 0;
		break;
	}
	case DEFAULT_LAIN: {
		bear->sprite.mirrored = bear->vel_x < 0;
		break;
	}
	default:
		break;
	}

	bear->sprite.animation_start_time = game_state->time;
}

static void update_bear(Resources *resources, GameState *game_state,
			Scene *scene, Bear *bear)
{
	if (bear->is_smoke) {
		if (sprite_animation_is_last_frame(&bear->sprite)) {
			reveal_bear(game_state, bear);
			depth_sort(scene->sprites,
				   cvector_size(scene->sprites));
		} else {
			sprite_try_next_frame(resources, game_state->time,
					      &bear->sprite);
		}
	} else {
		update_bear_position(bear);

		bear->sprite.mirrored = bear->vel_x < 0;

		sprite_try_next_frame(resources, game_state->time,
				      &bear->sprite);

		if ((random_int() & 0x3f) == 0) {
			set_random_velocity(bear);
		}
	}
}

void update_kumashoot(Resources *resources, Menu *menu, GameState *game_state,
		      GLFWwindow *window, Minigame *minigame)
{
	KumaShoot *kumashoot = &minigame->current.kumashoot;
	Scene *scene = &kumashoot->scene;

	if (glfwWindowShouldClose(window)) {
		destroy_minigame(resources->textures, menu, minigame, window);
		return;
	}

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

			update_character(scene, game_state, resources,
					 kumashoot, bear);
		} else {
			update_bear(resources, game_state, scene, bear);
		}
	}

	update_scene(scene);
}

static void init_kumashoot_sprites(Resources *resources, GameState *game_state,
				   KumaShoot *kumashoot)
{
	make_sprite(&kumashoot->background,
		    (Sprite){.pos = {0.0f, 0.0f},
			     .texture = texture_get(resources, KUMA_SHOOT_BG),
			     .visible = true,
			     .z_index = 0});

	make_sprite(
	    &kumashoot->bush_overlay,
	    (Sprite){
		.pos = {0.0f, 0.0f},
		.texture = texture_get(resources, KUMA_SHOOT_BUSH_OVERLAY),
		.visible = true,
		.z_index = 2,
	    });

	for (int i = 0; i < 3; i++) {
		spawn_bear(resources, game_state, &kumashoot->bears[i]);
	}
}

static void init_kumashoot_scene(Resources *resources, GameState *game_state,
				 Scene *scene, KumaShoot *kumashoot)
{
	init_kumashoot_sprites(resources, game_state, kumashoot);

	Bear *bear_1 = &kumashoot->bears[0];
	Bear *bear_2 = &kumashoot->bears[1];
	Bear *bear_3 = &kumashoot->bears[2];

	Sprite *sprites[] = {
	    &kumashoot->background,
	    &kumashoot->bush_overlay,
	    &bear_1->sprite,
	    &bear_2->sprite,
	    &bear_3->sprite,
	    &bear_1->hidden_character.additional_sprite,
	    &bear_2->hidden_character.additional_sprite,
	    &bear_3->hidden_character.additional_sprite,
	};

	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	ClickBarrier click_barriers[] = {
	    (ClickBarrier){76, 28, 164, 83},
	    (ClickBarrier){48, 82, 295, 113},
	    (ClickBarrier){240, 62, 288, 82},
	    (ClickBarrier){81, 222, 167, 246},
	    (ClickBarrier){57, 246, 208, 301},
	    (ClickBarrier){485, 88, 557, 128},
	    (ClickBarrier){466, 128, 563, 170},
	    (ClickBarrier){563, 110, 582, 170},
	    (ClickBarrier){446, 321, 600, 400},
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
			   .font = &resources->fonts[FONT_RED]};
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

int start_kumashoot(Menu *menu, Resources *resources, GameState *game_state,
		    Minigame *minigame, GLFWwindow **minigame_window,
		    GLFWwindow *main_window)
{
	if (!(make_window(minigame_window, MINIGAME_WIDTH, MINIGAME_HEIGHT,
			  "lain kuma shoot", main_window, true))) {
		printf("Failed to create kuma shoot window.\n");
		return 0;
	}

	init_kumashoot_scene(resources, game_state,
			     &minigame->current.kumashoot.scene,
			     &minigame->current.kumashoot);

	minigame->type = KUMASHOOT;
	minigame->last_updated = game_state->time;

	menu->bear_icon.texture = texture_get(resources, BEAR_ICON_ACTIVE);

	return 1;
}

void handle_kumashoot_event(KumaShootEvent event, Bear *bear, Engine *engine)
{
	if (event == CHARACTER_CLICK && !bear->revealed && !bear->is_smoke) {
		enqueue_sound(&engine->game_state.queued_sounds, SND_118);

		KumaShoot *kumashoot = &engine->minigame.current.kumashoot;

		set_sprite_to_smoke(&engine->resources, engine->game_state.time,
				    &bear->sprite);
		bear->is_smoke = true;

		depth_sort(kumashoot->scene.sprites,
			   cvector_size(kumashoot->scene.sprites));
	}
}
