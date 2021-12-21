#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "animations.h"
#include "minigame.h"
#include "resources.h"
#include "scene.h"
#include "sound.h"
#include "sprite.h"
#include "state.h"
#include "texture.h"
#include "theater.h"
#include "vector2d.h"
#include "window.h"

static inline int get_lain_walk_animation(LainOutfit outfit)
{
	switch (outfit) {
	case OUTFIT_SCHOOL:
		return SCHOOL_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_DEFAULT:
		return DEFAULT_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_CYBERIA:
		return CYBERIA_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_ALIEN:
		return ALIEN_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_SWEATER:
		return SWEATER_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_BEAR:
		return BEAR_LAIN_THEATER_WALK_ANIMATION;
	}
}

static void load_theater_animation(GameState *game_state, Resources *resources,
				   Theater *theater,
				   TheaterAnimationID animation_id)
{
	TheaterAnimation *animation =
	    theater_animation_get(resources, animation_id);

	for (int i = 0; i < animation->layer_count; i++) {
		Animation *layer_animation = &animation->layers[i];
		Sprite *layer = &theater->layers[i];

		make_sprite(&theater->layers[i], (Sprite){
						     .z_index = i,
						 });

		sprite_set_animation_direct(resources, game_state->time, layer,
					    layer_animation);
	}

	theater->layer_count = animation->layer_count;
}

static void init_theater(Resources *resources, GameState *game_state,
			 Theater *theater, int preview)
{
	LainOutfit lain_outfit = game_state->lain.outfit;
	LainToolState lain_tool_state = game_state->lain.tool_state;

	Sprite *sprites[5] = {&theater->layers[0], &theater->layers[1],
			      &theater->layers[2], &theater->layers[3],
			      &theater->layers[4]};

	switch (preview) {
	case CLASSROOM_PREVIEW: {
		theater->type = THEATER_CLASSROOM;

		if (lain_outfit == OUTFIT_SCHOOL) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_CLASSROOM_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, CLASSROOM_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 2,
					     .texture = texture_get(
						 resources, CLASSROOM_TABLES)});

			make_sprite(&theater->layers[2],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[2],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 3;
		}
		break;
	}
	case SCHOOL_PREVIEW: {
		theater->type = THEATER_SCHOOL;

		if (lain_outfit == OUTFIT_SCHOOL &&
		    lain_tool_state == HOLDING_NAVI) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_SCHOOL_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, SCHOOL_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case LAIN_ROOM_NIGHT_PREVIEW: {
		theater->type = THEATER_LAIN_ROOM_NIGHT;

		if (lain_outfit == OUTFIT_BEAR) {
			load_theater_animation(
			    game_state, resources, theater,
			    THEATER_LAIN_ROOM_NIGHT_ANIMATION);
		} else {
			make_sprite(
			    &theater->layers[0],
			    (Sprite){.visible = true,
				     .pos = {0.0f, 0.0f},
				     .z_index = 0,
				     .texture = texture_get(
					 resources, LAIN_ROOM_NIGHT_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case ARISU_ROOM_PREVIEW: {
		theater->type = THEATER_ARISU_ROOM;

		if (lain_outfit == OUTFIT_ALIEN) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_ARISU_ROOM_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, ARISU_ROOM_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}

		break;
	}
	case CYBERIA_PREVIEW: {
		theater->type = THEATER_CYBERIA;

		if (lain_outfit == OUTFIT_CYBERIA) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_CYBERIA_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, CYBERIA_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case STREET_PREVIEW: {
		theater->type = THEATER_STREET;

		if (lain_outfit == OUTFIT_SWEATER) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_STREET_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, STREET_BG_1)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case BRIDGE_PREVIEW: {
		theater->type = THEATER_BRIDGE;

		load_theater_animation(game_state, resources, theater,
				       THEATER_BRIDGE_ANIMATION);

		break;
	}
	}

	init_scene(&theater->scene, sprites, theater->layer_count, NULL, 0,
		   NULL, 0, NULL, 0);
}

void update_theater(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, Minigame *minigame)
{

	Theater *theater = &minigame->current.theater;

	_Bool was_animated = false;
	if (theater->type == THEATER_MOVIE) {
		was_animated = movie_render(resources->shaders[MOVIE_SHADER],
					    &theater->movie);
	} else {
		// hacky way of detecting whether or not the theater is over
		// if it wasnt animated, that means no animation in the scene
		// has a next frame, which means its over.
		for (int i = 0; i < cvector_size(theater->scene.sprites); ++i) {
			Sprite *curr = theater->scene.sprites[i];
			if (curr->animation != NULL) {
				sprite_try_next_frame(resources,
						      game_state->time, curr);
				was_animated = true;
			}
		}
	}

	if (!was_animated && theater->type == THEATER_BRIDGE) {
		free_scene(&theater->scene);

		movie_init(&theater->movie);
		theater->type = THEATER_MOVIE;

		reset_game_state(resources, game_state);
		menu->theater_preview.texture =
		    texture_get(resources, game_state->current_theater_preview);

		was_animated = true;
	}

	if (!was_animated || glfwWindowShouldClose(window)) {
		play_sound(&resources->audio_engine, SND_111);
		destroy_minigame(resources->textures, menu, minigame, window);
		return;
	}

	if (theater->type != THEATER_MOVIE) {
		update_scene(&theater->scene);
	}
}

int start_theater(Menu *menu, Resources *resources, GameState *game_state,
		  Minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window)
{

	if (!(make_window(minigame_window, MINIGAME_WIDTH, MINIGAME_HEIGHT,
			  "lain theatre", main_window, true))) {
		printf("Failed to create theater window.\n");
		return 0;
	}

	init_theater(resources, game_state, &minigame->current.theater,
		     menu->theater_preview.texture->id);

	minigame->type = THEATER;
	minigame->last_updated = game_state->time;

	return 1;
}
