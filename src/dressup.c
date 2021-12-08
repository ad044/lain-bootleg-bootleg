#include "animations.h"
#include "cvector.h"
#include "kumashoot.h"
#include "minigame.h"
#include "resources.h"
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
	case OUTFIT_DEFAULT: {
		return NULL;
	}

	case OUTFIT_SWEATER: {
		return &dressup->sweater_outfit;
	}

	case OUTFIT_CYBERIA: {
		return &dressup->cyberia_outfit;
	}

	case OUTFIT_ALIEN: {
		return &dressup->ufo;
	}

	case OUTFIT_BEAR: {
		return &dressup->bear_outfit;
	}

	case OUTFIT_SCHOOL: {
		return &dressup->school_outfit;
	}
	}
}

void lain_set_outfit(Resources *resources, LainOutfit outfit, Lain *lain)
{
	lain->outfit = outfit;
	switch (outfit) {
	case OUTFIT_DEFAULT: {
		lain->standing_texture =
		    texture_get(resources, LAIN_DEFAULT_STANDING);
		lain->leave_animation = LAIN_DEFAULT_LEAVE_ANIMATION;
		lain->walk_animation = LAIN_DEFAULT_WALK_LEFT_ANIMATION;

		break;
	}

	case OUTFIT_SWEATER: {
		lain->standing_texture =
		    texture_get(resources, LAIN_SWEATER_STANDING);
		lain->leave_animation = LAIN_SWEATER_LEAVE_ANIMATION;
		lain->walk_animation = LAIN_SWEATER_WALK_LEFT_ANIMATION;

		break;
	}

	case OUTFIT_CYBERIA: {
		lain->standing_texture =
		    texture_get(resources, LAIN_CYBERIA_STANDING);
		lain->leave_animation = LAIN_CYBERIA_LEAVE_ANIMATION;
		lain->walk_animation = LAIN_CYBERIA_WALK_LEFT_ANIMATION;

		break;
	}

	case OUTFIT_ALIEN: {
		lain->standing_texture =
		    texture_get(resources, LAIN_ALIEN_STANDING);
		lain->leave_animation = LAIN_ALIEN_LEAVE_ANIMATION;
		lain->walk_animation = LAIN_ALIEN_WALK_LEFT_ANIMATION;

		break;
	}

	case OUTFIT_BEAR: {
		lain->standing_texture =
		    texture_get(resources, LAIN_BEAR_STANDING);
		lain->leave_animation = LAIN_BEAR_LEAVE_ANIMATION;
		lain->walk_animation = LAIN_BEAR_WALK_LEFT_ANIMATION;

		break;
	}

	case OUTFIT_SCHOOL: {
		lain->standing_texture =
		    texture_get(resources, LAIN_SCHOOL_STANDING);
		lain->leave_animation = LAIN_SCHOOL_LEAVE_ANIMATION;
		lain->walk_animation = LAIN_SCHOOL_WALK_LEFT_ANIMATION;

		break;
	}
	}
}

static void init_dressup_sprites(Resources *resources, GameState *game_state,
				 DressUp *dressup)
{
	make_sprite(&dressup->bear_outfit.sprite,
		    (Sprite){
			.pos = {440.0f, 64.0f},
			.hitbox_size = {40.0f, 131.0f},
			.texture = texture_get(resources, BEAR_OUTFIT),
			.visible = game_state->score > 500 &&
				   game_state->lain.outfit != OUTFIT_BEAR,
			.z_index = 1,
		    });

	make_sprite(&dressup->school_outfit.sprite,
		    (Sprite){
			.pos = {32.0f, 64.0f},
			.hitbox_size = {40.0f, 131.0f},
			.texture = texture_get(resources, SCHOOL_OUTFIT),
			.visible = game_state->score > 100 &&
				   game_state->lain.outfit != OUTFIT_SCHOOL,
			.z_index = 1,
		    });

	make_sprite(&dressup->sweater_outfit.sprite,
		    (Sprite){
			.pos = {112.0f, 64.0f},
			.hitbox_size = {40.0f, 131.0f},
			.texture = texture_get(resources, SWEATER_OUTFIT),
			.visible = game_state->score > 2000 &&
				   game_state->lain.outfit != OUTFIT_SWEATER,
			.z_index = 1,
		    });

	make_sprite(&dressup->cyberia_outfit.sprite,
		    (Sprite){
			.pos = {336.0f, 64.0f},
			.hitbox_size = {40.0f, 131.0f},
			.texture = texture_get(resources, CYBERIA_OUTFIT),
			.visible = game_state->score > 200 &&
				   game_state->lain.outfit != OUTFIT_CYBERIA,
			.z_index = 1,
		    });

	make_sprite(&dressup->ufo.sprite,
		    (Sprite){
			.pos = {216.0f, 8.0f},
			.hitbox_size = {24.0f, 20.0f},
			.texture = texture_get(resources, DRESSUP_UFO),
			.visible = game_state->score > 1400 &&
				   game_state->lain.outfit != OUTFIT_ALIEN,
			.z_index = 1,
		    });

	make_sprite(&dressup->navi.sprite,
		    (Sprite){
			.pos = {32.0f, 112.0f},
			.hitbox_size = {12.0f, 42.0f},
			.texture = texture_get(resources, DRESSUP_NAVI),
			.visible = game_state->score > 900 &&
				   game_state->lain.tool_state != HOLDING_NAVI,
			.z_index = 1,
		    });

	make_sprite(
	    &dressup->screwdriver.sprite,
	    (Sprite){
		.pos = {32.0f, 192.0f},
		.hitbox_size = {12.0f, 42.0f},
		.texture = texture_get(resources, DRESSUP_SCREWDRIVER),
		.visible = game_state->score > 10 &&
			   game_state->lain.tool_state != HOLDING_SCREWDRIVER,
		.z_index = 1,
	    });

	make_sprite(&dressup->background,
		    (Sprite){
			.pos = {0.0f, 0.0f},
			.texture = texture_get(resources, LAIN_ROOM),
			.visible = true,
			.z_index = 0,
		    });

	make_sprite(&dressup->lain.sprite, (Sprite){
					       .hitbox_size = {80.0f, 161.0f},
					       .visible = true,
					       .z_index = 2,
					   });
	sprite_set_animation(resources, game_state->time, &dressup->lain.sprite,
			     game_state->lain.walk_animation);
}

static void init_dressup_scene(Resources *resources, GameState *game_state,
			       Scene *scene, DressUp *dressup)
{
	init_dressup_sprites(resources, game_state, dressup);

	Sprite *sprites[] = {&dressup->school_outfit.sprite,
			     &dressup->bear_outfit.sprite,
			     &dressup->sweater_outfit.sprite,
			     &dressup->cyberia_outfit.sprite,
			     &dressup->ufo.sprite,
			     &dressup->navi.sprite,
			     &dressup->screwdriver.sprite,
			     &dressup->background,
			     &dressup->lain.sprite};
	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	dressup->school_outfit.outfit = OUTFIT_SCHOOL;
	dressup->school_outfit.type = CLOTHING;

	dressup->sweater_outfit.outfit = OUTFIT_SWEATER;
	dressup->sweater_outfit.type = CLOTHING;

	dressup->bear_outfit.outfit = OUTFIT_BEAR;
	dressup->bear_outfit.type = CLOTHING;

	dressup->cyberia_outfit.outfit = OUTFIT_CYBERIA;
	dressup->cyberia_outfit.type = CLOTHING;

	dressup->ufo.outfit = OUTFIT_ALIEN;
	dressup->ufo.type = CLOTHING;

	dressup->navi.type = NAVI;
	dressup->navi.outfit = -1;

	dressup->screwdriver.type = SCREWDRIVER;
	dressup->screwdriver.outfit = -1;

	SpriteBehavior sprite_behaviors[] = {
	    (SpriteBehavior){.sprite = &dressup->school_outfit.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->school_outfit},
	    (SpriteBehavior){.sprite = &dressup->sweater_outfit.sprite,
			     .click_event = ITEM_GRAB,
			     .object = &dressup->sweater_outfit},
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

void update_dressup(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, Minigame *minigame)
{
	DressUp *dressup = &minigame->current.dressup;
	DressUpLain *lain = &dressup->lain;

	if (glfwWindowShouldClose(window) && lain->move_state != LEAVING) {
		enqueue_sound(&game_state->queued_sounds, SND_116);

		lain->move_state = LEAVING;

		sprite_set_animation(resources, game_state->time, &lain->sprite,
				     game_state->lain.leave_animation);

		depth_sort(dressup->scene.sprites,
			   cvector_size(dressup->scene.sprites));

		if (game_state->lain.tool_state == HOLDING_SCREWDRIVER) {
			dressup->screwdriver.sprite.visible = false;
		}

		if (game_state->lain.tool_state == HOLDING_NAVI) {
			dressup->navi.sprite.visible = false;
		}
	}

	switch (lain->move_state) {
	case STANDING: {
		DressUpObject *currently_grabbed = dressup->currently_grabbed;
		if (currently_grabbed != NULL) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			currently_grabbed->sprite.pos = (Vector2D){
			    x - currently_grabbed->sprite.texture->size.x / 2,
			    y - currently_grabbed->sprite.texture->size.y / 2};
		};

		break;
	}
	case ENTERING: {
		if (sprite_animation_is_last_frame(&lain->sprite)) {
			lain->move_state = STANDING;

			if (game_state->lain.tool_state == HOLDING_NAVI) {
				dressup->navi.sprite.visible = true;
				dressup->navi.sprite.z_index = 3;
				dressup->navi.sprite.pos =
				    (Vector2D){320.0f, 249.0f};
			}

			if (game_state->lain.tool_state ==
			    HOLDING_SCREWDRIVER) {
				dressup->screwdriver.sprite.visible = true;
				dressup->screwdriver.sprite.z_index = 3;
				dressup->screwdriver.sprite.pos =
				    (Vector2D){248.0f, 232.0f};
			}

			depth_sort(dressup->scene.sprites,
				   cvector_size(dressup->scene.sprites));
		} else {
			sprite_try_next_frame(game_state->time, &lain->sprite);
		}

		break;
	}
	case LEAVING: {
		if (sprite_animation_is_last_frame(&lain->sprite)) {
			destroy_minigame(resources->textures, menu, minigame,
				      window);
			return;
		} else {
			sprite_try_next_frame(game_state->time, &lain->sprite);
		}

		break;
	}
	}

	update_scene(&dressup->scene);
}
int start_dressup(Menu *menu, Resources *resources, GameState *game_state,
		  Minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window)
{
	if (!(make_window(minigame_window, MINIGAME_WIDTH, MINIGAME_HEIGHT,
			  "lain dress up", main_window, true))) {
		printf("Failed to create dressup window.\n");
		return 0;
	}

	DressUp *dressup = &minigame->current.dressup;

	dressup->lain.move_state = ENTERING;
	dressup->currently_grabbed = NULL;

	lain_set_outfit(resources, game_state->lain.outfit, &game_state->lain);

	init_dressup_scene(resources, game_state, &dressup->scene, dressup);

	minigame->type = DRESSUP;
	minigame->last_updated = game_state->time;

	menu->dressup_button.texture =
	    texture_get(resources, DRESSUP_BUTTON_ACTIVE);

	enqueue_sound(&game_state->queued_sounds, SND_115);

	return 1;
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
		       game_state->lain.outfit == OUTFIT_DEFAULT;
	case NAVI:
		return (320.0f <= pos.x && pos.x <= 344.0f) &&
		       (249.0f <= pos.y && pos.y <= 291.0f) &&
		       game_state->lain.outfit == OUTFIT_SCHOOL;
	}
}

static void reset_tools(GameState *game_state, DressUp *dressup)
{
	if (game_state->lain.tool_state != NO_TOOLS) {
		sprite_set_to_origin_pos(&dressup->navi.sprite);
		sprite_set_to_origin_pos(&dressup->screwdriver.sprite);

		game_state->lain.tool_state = NO_TOOLS;
	}
}

static void wear_item(Resources *resources, GameState *game_state,
		      DressUp *dressup, DressUpObject *item)
{

	switch (item->type) {
	case CLOTHING:
		if (game_state->lain.outfit != OUTFIT_DEFAULT) {
			// reset current clothing
			DressUpObject *outfit_obj = get_object_for_outfit(
			    dressup, game_state->lain.outfit);

			sprite_set_to_origin_pos(&outfit_obj->sprite);
			outfit_obj->sprite.visible = true;
			outfit_obj->sprite.z_index = 1;
		}

		if (!(item->outfit == OUTFIT_SCHOOL &&
		      game_state->lain.tool_state == HOLDING_NAVI)) {

			reset_tools(game_state, dressup);
		}

		lain_set_outfit(resources, dressup->currently_grabbed->outfit,
				&game_state->lain);

		dressup->lain.sprite.texture =
		    game_state->lain.standing_texture;

		dressup->currently_grabbed->sprite.pos = (Vector2D){0.0f, 0.0f};
		dressup->currently_grabbed->sprite.visible = false;

		break;
	case SCREWDRIVER:
		dressup->currently_grabbed->sprite.pos =
		    (Vector2D){248.0f, 232.0f};
		game_state->lain.tool_state = HOLDING_SCREWDRIVER;
		dressup->currently_grabbed->sprite.z_index = 3;

		break;
	case NAVI:
		dressup->currently_grabbed->sprite.pos =
		    (Vector2D){320.0f, 249.0f};
		game_state->lain.tool_state = HOLDING_NAVI;
		dressup->currently_grabbed->sprite.z_index = 3;

		break;
	}
}

void handle_dressup_event(DressUpEvent event, void *object, Engine *engine)
{
	DressUp *dressup = &engine->minigame.current.dressup;
	DressUpLain *lain = &dressup->lain;
	GameState *game_state = &engine->game_state;
	Resources *resources = &engine->resources;

	if (lain->move_state != STANDING) {
		return;
	}

	switch (event) {
	case ITEM_GRAB: {
		dressup->currently_grabbed = object;
		dressup->currently_grabbed->sprite.visible = true;
		dressup->currently_grabbed->sprite.z_index = 4;

		depth_sort(dressup->scene.sprites,
			   cvector_size(dressup->scene.sprites));

		if (game_state->lain.tool_state == HOLDING_SCREWDRIVER &&
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
			enqueue_sound(&game_state->queued_sounds, SND_117);

			wear_item(resources, game_state, dressup,
				  dressup->currently_grabbed);
		} else {
			if (dressup->currently_grabbed->outfit ==
			    OUTFIT_SCHOOL) {
				reset_tools(game_state, dressup);
			}

			sprite_set_to_origin_pos(
			    &dressup->currently_grabbed->sprite);

			dressup->currently_grabbed->sprite.z_index = 1;
		}

		dressup->currently_grabbed = NULL;

		depth_sort(dressup->scene.sprites,
			   cvector_size(dressup->scene.sprites));
		break;
	}
	case LAIN_SWAP_CLOTHING: {
		if (game_state->lain.outfit == OUTFIT_DEFAULT) {
			break;
		}

		DressUpObject *outfit_obj =
		    get_object_for_outfit(dressup, game_state->lain.outfit);

		handle_dressup_event(ITEM_GRAB, outfit_obj, engine);

		lain_set_outfit(resources, OUTFIT_DEFAULT, &game_state->lain);
		dressup->lain.sprite.texture =
		    game_state->lain.standing_texture;

		break;
	}
	}

	Sprite *screwdriver_icon = &engine->menu.screwdriver_icon;
	if (game_state->lain.tool_state == HOLDING_SCREWDRIVER) {
		screwdriver_icon->texture =
		    texture_get(resources, SCREWDRIVER_ICON_ACTIVE);
	} else {
		screwdriver_icon->texture =
		    texture_get(resources, SCREWDRIVER_ICON);
	}
}
