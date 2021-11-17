#include "cvector.h"
#include "kumashoot.h"
#include "sprite.h"
#include "state.h"
#include "texture.h"
#include "vector2d.h"
#include "window.h"

#include "dressup.h"

#include <stdio.h>
#include <stdlib.h>

static DressUpObject *get_object_for_outfit(DressUp *dressup, LainOutfit outfit)
{
	switch (outfit) {
	case DEFAULT_OUTFIT: {
		return NULL;
	}

	case PAJAMA_OUTFIT: {
		return &dressup->pajama_outfit;
	}

	case CYBERIA_OUTFIT: {
		return &dressup->cyberia_outfit;
	}

	case ALIEN_OUTFIT: {
		return &dressup->ufo;
	}

	case BEAR_OUTFIT: {
		return &dressup->bear_outfit;
	}

	case SCHOOL_OUTFIT: {
		return &dressup->school_outfit;
	}
	}
}

static void lain_set_outfit(Texture *textures, GameState *game_state,
			    LainOutfit outfit, DressUpLain *lain)
{
	switch (outfit) {
	case DEFAULT_OUTFIT: {
		lain->standing_texture = &textures[LAIN_DEFAULT_STANDING];
		lain->leave_texture = &textures[LAIN_DEFAULT_LEAVE];
		lain->walking_texture = &textures[LAIN_DEFAULT_MOVE_LEFT];

		break;
	}

	case PAJAMA_OUTFIT: {
		lain->standing_texture = &textures[LAIN_PAJAMA_STANDING];
		lain->leave_texture = &textures[LAIN_PAJAMA_LEAVE];
		lain->walking_texture = &textures[LAIN_PAJAMA_MOVE_LEFT];

		break;
	}

	case CYBERIA_OUTFIT: {
		lain->standing_texture = &textures[LAIN_CYBERIA_STANDING];
		lain->leave_texture = &textures[LAIN_CYBERIA_LEAVE];
		lain->walking_texture = &textures[LAIN_CYBERIA_MOVE_LEFT];

		break;
	}

	case ALIEN_OUTFIT: {
		lain->standing_texture = &textures[LAIN_ALIEN_STANDING];
		lain->leave_texture = &textures[LAIN_ALIEN_LEAVE];
		lain->walking_texture = &textures[LAIN_ALIEN_MOVE_LEFT];

		break;
	}

	case BEAR_OUTFIT: {
		lain->standing_texture = &textures[LAIN_BEAR_STANDING];
		lain->leave_texture = &textures[LAIN_BEAR_LEAVE];
		lain->walking_texture = &textures[LAIN_BEAR_MOVE_LEFT];

		break;
	}

	case SCHOOL_OUTFIT: {
		lain->standing_texture = &textures[LAIN_SCHOOL_STANDING];
		lain->leave_texture = &textures[LAIN_SCHOOL_LEAVE];
		lain->walking_texture = &textures[LAIN_SCHOOL_MOVE_LEFT];

		break;
	}
	}

	game_state->lain_outfit = outfit;
	lain->sprite.texture = lain->standing_texture;
};

static void init_dressup_sprites(Resources *resources, GameState *game_state,
				 DressUp *dressup)
{
	Texture *textures = resources->textures;

	dressup->bear_outfit.sprite = (Sprite){
	    .pos = {440.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .hitbox_size = {40.0f, 131.0f},
	    .texture = &textures[DRESSUP_BEAR_OUTFIT],
	    .visible = game_state->score > 500 &&
		       game_state->lain_outfit != BEAR_OUTFIT,
	    .z_index = 1,
	};

	dressup->school_outfit.sprite = (Sprite){
	    .pos = {32.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .hitbox_size = {40.0f, 131.0f},
	    .texture = &textures[DRESSUP_SCHOOL_OUTFIT],
	    .visible = game_state->score > 100 &&
		       game_state->lain_outfit != SCHOOL_OUTFIT,
	    .z_index = 1,
	};

	dressup->pajama_outfit.sprite = (Sprite){
	    .pos = {112.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .hitbox_size = {40.0f, 131.0f},
	    .texture = &textures[DRESSUP_PAJAMA_OUTFIT],
	    .visible = game_state->score > 2000 &&
		       game_state->lain_outfit != PAJAMA_OUTFIT,
	    .z_index = 1,
	};

	dressup->cyberia_outfit.sprite = (Sprite){
	    .pos = {336.0f, 64.0f},
	    .size = {160.0f, 262.0f},
	    .hitbox_size = {40.0f, 131.0f},
	    .texture = &textures[DRESSUP_CYBERIA_OUTFIT],
	    .visible = game_state->score > 200 &&
		       game_state->lain_outfit != CYBERIA_OUTFIT,
	    .z_index = 1,
	};

	dressup->ufo.sprite = (Sprite){
	    .pos = {216.0f, 8.0f},
	    .size = {48.0f, 40.0f},
	    .hitbox_size = {24.0f, 20.0f},
	    .texture = &textures[DRESSUP_UFO],
	    .visible = game_state->score > 1400 &&
		       game_state->lain_outfit != ALIEN_OUTFIT,
	    .z_index = 1,
	};

	dressup->navi.sprite = (Sprite){
	    .pos = {32.0f, 112.0f},
	    .size = {24.0f, 42.0f},
	    .hitbox_size = {12.0f, 42.0f},
	    .texture = &textures[DRESSUP_NAVI],
	    .visible = game_state->score > 900 &&
		       game_state->lain_tool_state != HOLDING_NAVI,
	    .z_index = 1,
	};

	dressup->screwdriver.sprite = (Sprite){
	    .pos = {32.0f, 192.0f},
	    .size = {24.0f, 42.0f},
	    .hitbox_size = {12.0f, 42.0f},
	    .texture = &textures[DRESSUP_SCREWDRIVER],
	    .visible = game_state->score > 10 &&
		       game_state->lain_tool_state != HOLDING_SCREWDRIVER,
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
	    .hitbox_size = {80.0f, 161.0f},
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

	Sprite *sprites[] = {
	    &dressup->school_outfit.sprite, &dressup->bear_outfit.sprite,
	    &dressup->pajama_outfit.sprite, &dressup->cyberia_outfit.sprite,
	    &dressup->ufo.sprite,	    &dressup->navi.sprite,
	    &dressup->screwdriver.sprite,   &dressup->background,
	    &dressup->lain.sprite};
	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	dressup->school_outfit.outfit = SCHOOL_OUTFIT;
	dressup->school_outfit.type = CLOTHING;

	dressup->pajama_outfit.outfit = PAJAMA_OUTFIT;
	dressup->pajama_outfit.type = CLOTHING;

	dressup->bear_outfit.outfit = BEAR_OUTFIT;
	dressup->bear_outfit.type = CLOTHING;

	dressup->cyberia_outfit.outfit = CYBERIA_OUTFIT;
	dressup->cyberia_outfit.type = CLOTHING;

	dressup->ufo.outfit = ALIEN_OUTFIT;
	dressup->ufo.type = CLOTHING;

	dressup->navi.type = NAVI;
	dressup->navi.outfit = -1;

	dressup->screwdriver.type = SCREWDRIVER;
	dressup->screwdriver.outfit = -1;

	SpriteBehavior sprite_behaviors[] = {
	    (SpriteBehavior){.sprite = &dressup->school_outfit.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->school_outfit},
	    (SpriteBehavior){.sprite = &dressup->pajama_outfit.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->pajama_outfit},
	    (SpriteBehavior){.sprite = &dressup->bear_outfit.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->bear_outfit},
	    (SpriteBehavior){.sprite = &dressup->cyberia_outfit.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->cyberia_outfit},
	    (SpriteBehavior){.sprite = &dressup->ufo.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->ufo},
	    (SpriteBehavior){.sprite = &dressup->navi.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->navi},
	    (SpriteBehavior){.sprite = &dressup->screwdriver.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->screwdriver},
	    (SpriteBehavior){.sprite = &dressup->lain.sprite,
			     .click_event = LAIN_SWAP_CLOTHING,
			     .object = &dressup->lain},
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

	DressUpLain *lain = &dressup->lain;
	switch (lain->move_state) {
	case STANDING: {
		Sprite *currently_grabbed = &dressup->currently_grabbed->sprite;
		if (currently_grabbed != NULL) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			// TODO original game puts it in different position
			// perhaps RE that
			currently_grabbed->pos =
			    (Vector2D){x - currently_grabbed->size.x / 2,
				       y - currently_grabbed->size.y / 2};
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

			if (game_state->lain_tool_state == HOLDING_NAVI) {
				dressup->navi.sprite.visible = true;
				dressup->navi.sprite.z_index = 3;
				dressup->navi.sprite.pos =
				    (Vector2D){320.0f, 249.0f};
			}

			if (game_state->lain_tool_state ==
			    HOLDING_SCREWDRIVER) {
				dressup->screwdriver.sprite.visible = true;
				dressup->screwdriver.sprite.z_index = 3;
				dressup->screwdriver.sprite.pos =
				    (Vector2D){248.0f, 232.0f};
			}

			depth_sort(dressup->scene.sprites,
				   cvector_size(dressup->scene.sprites));

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
	lain_set_outfit(resources->textures, game_state,
			game_state->lain_outfit, &dressup->lain);

	init_dressup_scene(resources, game_state, &dressup->scene, dressup);

	minigame->current = dressup;
	minigame->update = update_dressup;
	minigame->scene = &dressup->scene;
	minigame->type = DRESSUP;
	minigame->refresh_rate = 30.0;
	minigame->last_updated = game_state->time;
}

static _Bool can_wear(GameState *game_state, DressUpObject *item, Vector2D pos)
{

	switch (item->type) {
	case CLOTHING:
		return (256.0f <= pos.x && pos.x <= 336.0f) &&
		       (144.0f <= pos.y && pos.y <= 326.0f);
	case SCREWDRIVER:
		return (248.0f <= pos.x && pos.x <= 272.0f) &&
		       (232.0f <= pos.y && pos.y <= 274.0f) &&
		       game_state->lain_outfit == DEFAULT_OUTFIT;
	case NAVI:
		return (320.0f <= pos.x && pos.x <= 344.0f) &&
		       (249.0f <= pos.y && pos.y <= 291.0f) &&
		       game_state->lain_outfit == SCHOOL_OUTFIT;
	}
}

static void reset_tools(GameState *game_state, DressUp *dressup)
{
	if (game_state->lain_tool_state != NO_TOOLS) {
		sprite_set_to_origin_pos(&dressup->navi.sprite);
		sprite_set_to_origin_pos(&dressup->screwdriver.sprite);

		game_state->lain_tool_state = NO_TOOLS;
	}
}

static void wear_item(Texture *textures, GameState *game_state,
		      DressUp *dressup, DressUpObject *item)
{

	switch (item->type) {
	case CLOTHING:
		// reset current clothing
		if (game_state->lain_outfit != DEFAULT_OUTFIT) {
			DressUpObject *outfit_obj = get_object_for_outfit(
			    dressup, game_state->lain_outfit);

			sprite_set_to_origin_pos(&outfit_obj->sprite);
			outfit_obj->sprite.visible = true;
		}

		if (!(item->outfit == SCHOOL_OUTFIT &&
		      game_state->lain_tool_state == HOLDING_NAVI)) {

			reset_tools(game_state, dressup);
		}

		lain_set_outfit(textures, game_state,
				dressup->currently_grabbed->outfit,
				&dressup->lain);

		dressup->currently_grabbed->sprite.pos = (Vector2D){0.0f, 0.0f};
		dressup->currently_grabbed->sprite.visible = false;
		dressup->currently_grabbed = NULL;

		break;
	case SCREWDRIVER:
		dressup->currently_grabbed->sprite.pos =
		    (Vector2D){248.0f, 232.0f};
		game_state->lain_tool_state = HOLDING_SCREWDRIVER;
		dressup->currently_grabbed = NULL;

		break;
	case NAVI:
		dressup->currently_grabbed->sprite.pos =
		    (Vector2D){320.0f, 249.0f};
		game_state->lain_tool_state = HOLDING_NAVI;
		dressup->currently_grabbed = NULL;

		break;
	}
}

void handle_dressup_event(DressUpEvent event, void *object, Engine *engine)
{
	DressUp *dressup = (DressUp *)engine->minigame.current;
	DressUpLain *lain = &dressup->lain;
	GameState *game_state = &engine->game_state;
	Texture *textures = engine->resources.textures;

	if (lain->move_state != STANDING) {
		return;
	}

	switch (event) {
	case ITEM_GRAB: {
		dressup->currently_grabbed = object;
		dressup->currently_grabbed->sprite.visible = true;
		dressup->currently_grabbed->sprite.z_index = 5;

		depth_sort(dressup->scene.sprites,
			   cvector_size(dressup->scene.sprites));

		if (game_state->lain_tool_state == HOLDING_SCREWDRIVER &&
		    dressup->currently_grabbed->type == SCREWDRIVER) {
			reset_tools(game_state, dressup);
		}

		break;
	}
	case ITEM_RELEASE: {
		if (dressup->currently_grabbed == NULL) {
			break;
		}

		double x, y;
		glfwGetCursorPos(engine->minigame_window, &x, &y);
		Vector2D pos = (Vector2D){x, y};

		if (can_wear(game_state, dressup->currently_grabbed, pos)) {
			wear_item(textures, game_state, dressup,
				  dressup->currently_grabbed);
		} else {
			if (dressup->currently_grabbed->outfit ==
			    SCHOOL_OUTFIT) {
				reset_tools(game_state, dressup);
			}

			sprite_set_to_origin_pos(
			    &dressup->currently_grabbed->sprite);

			dressup->currently_grabbed->sprite.z_index = 1;
			dressup->currently_grabbed = NULL;
		}

		break;
	}
	case LAIN_SWAP_CLOTHING: {
		if (game_state->lain_outfit == DEFAULT_OUTFIT) {
			break;
		}

		DressUpObject *outfit_obj =

		    get_object_for_outfit(dressup, game_state->lain_outfit);

		handle_dressup_event(ITEM_GRAB, outfit_obj, engine);

		lain_set_outfit(textures, game_state, DEFAULT_OUTFIT, lain);

		break;
	}
	}

	Sprite *screwdriver_icon = &engine->menu.screwdriver_icon;
	if (game_state->lain_tool_state == HOLDING_SCREWDRIVER) {
		screwdriver_icon->texture = &textures[SCREWDRIVER_ICON_ACTIVE];
	} else {
		screwdriver_icon->texture =
		    &textures[SCREWDRIVER_ICON_INACTIVE];
	}
}
