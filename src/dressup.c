#include "sprite.h"
#include "texture.h"
#include "window.h"

#include "dressup.h"

#include <stdio.h>

static Texture *get_texture_for_outfit(Texture *textures, LainOutfit outfit,
				       DressupLain *lain)
{
	switch (outfit) {
	case DEFAULT_OUTFIT: {
		break;
	}

	case PAJAMA_OUTFIT: {
		lain->standing_texture = &textures[LAIN_PAJAMA_STANDING];
		lain->walking_texture = &textures[LAIN_PAJAMA_MOVE_LEFT];
		lain->leave_texture = &textures[LAIN_PAJAMA_LEAVE];

		break;
	}

	case CYBERIA_OUTFIT: {
		lain->standing_texture = &textures[LAIN_CYBERIA_STANDING];
		lain->walking_texture = &textures[LAIN_CYBERIA_MOVE_LEFT];
		lain->leave_texture = &textures[LAIN_CYBERIA_LEAVE];

		break;
	}

	case ALIEN_OUTFIT: {
		lain->standing_texture = &textures[LAIN_ALIEN_STANDING];
		lain->walking_texture = &textures[LAIN_ALIEN_MOVE_LEFT];
		lain->leave_texture = &textures[LAIN_ALIEN_LEAVE];

		break;
	}

	case BEAR_OUTFIT: {
		lain->standing_texture = &textures[LAIN_BEAR_STANDING];
		lain->walking_texture = &textures[LAIN_BEAR_MOVE_LEFT];
		lain->leave_texture = &textures[LAIN_BEAR_LEAVE];

		break;
	}

	case SCHOOL_OUTFIT: {
		lain->standing_texture = &textures[LAIN_SCHOOL_STANDING];
		lain->walking_texture = &textures[LAIN_SCHOOL_MOVE_LEFT];
		lain->leave_texture = &textures[LAIN_SCHOOL_LEAVE];

		break;
	}
	}
};

static void init_dressup_sprites(Resources *resources, GameState *game_state,
				 DressUp *dressup

)
{
	Texture *textures = resources->textures;

	dressup->bear_outfit = (Sprite){
	    .pos = {440.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .texture = &textures[DRESSUP_BEAR_OUTFIT],
	    .visible = game_state->score > 500,
	    .z_index = 1,
	};

	dressup->school_outfit = (Sprite){
	    .pos = {32.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .texture = &textures[DRESSUP_SCHOOL_OUTFIT],
	    .visible = game_state->score > 100,
	    .z_index = 1,
	};

	dressup->pajama_outfit = (Sprite){
	    .pos = {112.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .texture = &textures[DRESSUP_PAJAMA_OUTFIT],
	    .visible = game_state->score > 2000,
	    .z_index = 1,
	};

	dressup->cyberia_outfit = (Sprite){
	    .pos = {336.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .texture = &textures[DRESSUP_CYBERIA_OUTFIT],
	    .visible = game_state->score > 200,
	    .z_index = 1,
	};

	dressup->ufo = (Sprite){
	    .pos = {216.0f, 8.0f},
	    .size = {48.0f, 40.0f},
	    .texture = &textures[DRESSUP_UFO],
	    .visible = game_state->score > 1400,
	    .z_index = 1,
	};

	dressup->navi = (Sprite){
	    .pos = {32.0f, 112.0f},
	    .size = {24.0f, 42.0f},
	    .texture = &textures[DRESSUP_NAVI],
	    .visible = game_state->score > 900,
	    .z_index = 1,
	};

	dressup->screwdriver = (Sprite){
	    .pos = {32.0f, 192.0f},
	    .size = {24.0f, 42.0f},
	    .texture = &textures[DRESSUP_SCREWDRIVER],
	    .visible = game_state->score > 10,
	    .z_index = 1,
	};

	dressup->background = (Sprite){
	    .pos = {0.0f, 0.0f},
	    .size = {600.0f, 400.0f},
	    .texture = &textures[LAIN_ROOM],
	    .visible = true,
	    .z_index = 0,
	};

	dressup->lain.sprite = (Sprite){
	    .pos = {600.0f, 104.0f},
	    .size = {160.0f, 262.0f},
	    .texture = dressup->lain.walking_texture,
	    .visible = true,
	    .z_index = 1,
	    .is_spritesheet = true,
	    .max_frame = 7,
	};

	sprite_set_animation(&dressup->lain.sprite,
			     &resources->animations[LAIN_MOVE_LEFT_ANIMATION]);
}

static void init_dressup_scene(Resources *resources, GameState *game_state,
			       Scene *scene, DressUp *dressup)
{
	init_dressup_sprites(resources, game_state, dressup);

	Sprite *sprites[] = {&dressup->school_outfit, &dressup->bear_outfit,
			     &dressup->pajama_outfit, &dressup->cyberia_outfit,
			     &dressup->ufo,	      &dressup->navi,
			     &dressup->screwdriver,   &dressup->background,
			     &dressup->lain.sprite};
	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	SpriteBehavior sprite_behaviors[] = {
	    (SpriteBehavior){.sprite = &dressup->school_outfit,
			     .click_event = ITEM_GRAB},
	    (SpriteBehavior){.sprite = &dressup->pajama_outfit,
			     .click_event = ITEM_GRAB},
	    (SpriteBehavior){.sprite = &dressup->bear_outfit,
			     .click_event = ITEM_GRAB},
	    (SpriteBehavior){.sprite = &dressup->cyberia_outfit,
			     .click_event = ITEM_GRAB},
	    (SpriteBehavior){.sprite = &dressup->ufo, .click_event = ITEM_GRAB},
	    (SpriteBehavior){.sprite = &dressup->navi,
			     .click_event = ITEM_GRAB},
	    (SpriteBehavior){.sprite = &dressup->screwdriver,
			     .click_event = ITEM_GRAB},

	};
	uint8_t sprite_behavior_count =
	    sizeof(sprite_behaviors) / sizeof(sprite_behaviors[0]);

	init_scene(scene, sprites, sprite_count, sprite_behaviors,
		   sprite_behavior_count, NULL, 0, NULL, 0);
}

static void update_dressup(Resources *resources, GameState *game_state,
			   GLFWwindow *window, void *minigame_struct)
{
	DressUp *dressup = (DressUp *)minigame_struct;

	DressupLain *lain = &dressup->lain;
	switch (lain->move_state) {
	case STANDING: {

		Sprite *currently_grabbed = dressup->currently_grabbed;
		if (currently_grabbed != NULL) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			// TODO original game puts it in different position
			// perhaps RE that
			Vector2D click_pos =
			    (Vector2D){x - currently_grabbed->size.x / 2,
				       y - currently_grabbed->size.y / 2};
			currently_grabbed->pos = click_pos;
		};

		break;
	}
	case ENTERING: {
		if (lain->sprite.pos.x == 216.0f) {
			lain->move_state = STANDING;
			lain->sprite.pos.x = 216.0f;
			lain->sprite.texture = lain->standing_texture;
			lain->sprite.is_spritesheet = false;
			lain->sprite.max_frame = 0;

			init_sprite(&lain->sprite);
		} else {
			lain->sprite.pos.x -= 12.0f;
			sprite_try_next_frame(game_state->time, &lain->sprite);
		}

		break;
	}
	case LEAVING: {

		break;
	}
	}

	update_scene(&dressup->scene);
}
void start_dressup(Resources *resources, GameState *game_state,
		   Minigame *minigame, GLFWwindow **minigame_window,
		   GLFWwindow *main_window)
{
	if (!(make_window(minigame_window, MINIGAME_WIDTH, MINIGAME_HEIGHT,
			  "lain dress up", main_window, true))) {
		printf("Failed to create dressup window.\n");
		exit(1);
	}

	DressUp *dressup = malloc(sizeof(DressUp));
	if (dressup == NULL) {
		printf("Failed to allocate memory kuma shoot struct.\n");
		exit(1);
	}

	dressup->lain.move_state = ENTERING;
	dressup->currently_grabbed = NULL;
	get_texture_for_outfit(resources->textures, game_state->lain_outfit,
			       &dressup->lain);

	init_dressup_scene(resources, game_state, &dressup->scene, dressup);

	minigame->current = dressup;
	minigame->update = update_dressup;
	minigame->scene = &dressup->scene;
	minigame->type = DRESSUP;
	minigame->refresh_rate = 30.0;
	minigame->last_updated = game_state->time;
}

void handle_dressup_event(DressUpEvent event, Sprite *sprite, Engine *engine)
{
	DressUp *dressup = (DressUp *)engine->minigame.current;
	switch (event) {
	case ITEM_GRAB: {
		dressup->currently_grabbed = sprite;

		break;
	}
	case ITEM_RELEASE: {
		if (dressup->currently_grabbed != NULL) {
			dressup->currently_grabbed->pos =
			    dressup->currently_grabbed->origin_pos;
			dressup->currently_grabbed = NULL;
		}

		break;
	}
	}
}
