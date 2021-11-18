#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "animations.h"
#include "dressup.h"
#include "kumashoot.h"
#include "menu.h"
#include "minigame.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
#include "state.h"
#include "text.h"
#include "texture.h"
#include "vector2d.h"
#include "window.h"

#define PI 3.14159265359

static void update_menu_time(Menu *menu)
{
	time_t t;

	time(&t);

	menu->current_time = localtime(&t);
}

static void get_menu_timestring(char *target, Menu *menu)
{
	strftime(target, sizeof(char) * 11, "%p%I:%M:%S", menu->current_time);
}

static void init_menu_sprites(Menu *menu, Texture *textures)
{
	menu->ui_lain.sprite = (Sprite){.pos = {6.0f, 6.0f},
					.size = {64.0f, 64.0f},
					.hitbox_size = {32.0f, 32.0f},
					.is_spritesheet = true,
					.max_frame = 8,
					.visible = true,
					.z_index = 4};

	menu->main_ui = (Sprite){
	    .pos = {-34.0f, -70.0f},
	    .size = {200.0f, 200.0f},
	    .texture = &textures[MAIN_UI],
	    .is_spritesheet = true,
	    .max_frame = 6,
	    .visible = true,
	    .z_index = 3,
	};

	menu->main_ui_bar = (Sprite){
	    .pos = {102.0f, 38.0f},
	    .size = {64.0f, 8.0f},
	    .hitbox_size = {64.0f, 8.0f},
	    .texture = &textures[MAIN_UI_BAR_INACTIVE],
	    .visible = true,
	    .z_index = 4,
	};

	menu->dressup_button = (Sprite){
	    .pos = {112.0f, 96.0f},
	    .size = {72.0f, 72.0f},
	    .hitbox_size = {36.0f, 36.0f},
	    .texture = &textures[DRESSUP_BUTTON_INACTIVE],
	    .visible = false,
	    .z_index = 4,
	};

	menu->theater_preview = (Sprite){
	    .pos = {104.0f, 80.0f},
	    .size = {96.0f, 64.0f},
	    .texture = &textures[THEATER_PREVIEWS],
	    .visible = false,
	    .z_index = 7,
	    .is_spritesheet = true,
	    .max_frame = 6,
	};

	menu->theater_button = (Sprite){
	    .pos = {0.0f, 128.0f},
	    .size = {128.0f, 64.0f},
	    .hitbox_size = {64.0f, 32.0f},
	    .texture = &textures[THEATER_BUTTON_INACTIVE],
	    .visible = false,
	    .z_index = 4,
	};

	menu->bear_icon = (Sprite){
	    .pos = {56.0f, 56.0f},
	    .size = {32.0f, 32.0f},
	    .hitbox_size = {32.0f, 32.0f},
	    .texture_index = 4,
	    .texture = &textures[BEAR_ICON_INACTIVE],
	    .visible = false,
	    .z_index = 1,
	};

	menu->screwdriver_icon = (Sprite){
	    .pos = {56.0f, 56.0f},
	    .size = {32.0f, 32.0f},
	    .texture = &textures[SCREWDRIVER_ICON_INACTIVE],
	    .visible = false,
	    .z_index = 2,
	};

	menu->paw_icon = (Sprite){
	    .pos = {56.0f, 56.0f},
	    .size = {32.0f, 32.0f},
	    .texture = &textures[PAW_ICON],
	    .visible = false,
	    .z_index = 1,
	};

	menu->score_preview = (Sprite){.pos = {0.0f, 0.0f},
				       .size = {200.0f, 88.0f},
				       .texture_index = 10,
				       .texture = &textures[SCORE_PREVIEW],
				       .visible = false,
				       .z_index = 7};
}

static void init_menu_text_objects(Menu *menu, Font *fonts,
				   GameState *game_state)
{
	menu->clock = (Text){.pos = {70.0f, 22.0f},
			     .glyph_size = {32.0f, 16.0f},
			     .visible = true,
			     .font = &fonts[WHITE_FONT]};
	get_menu_timestring(menu->clock.current_text, menu);

	menu->score_text = (Text){.pos = {178.0f, 16.0f},
				  .glyph_size = {10.0f, 16.0f},
				  .visible = false,
				  .left_aligned = true,
				  .font = &fonts[RED_FONT]};
	sprintf(menu->score_text.current_text, "%d", game_state->score);
}

static void animate_menu_expand(GameState *game_state, Texture *textures,
				Menu *menu, GLFWwindow *window)
{
	Sprite *main_ui = &menu->main_ui;

	if (!sprite_is_max_frame(main_ui)) {
		if (main_ui->frame_index == 1) {
			expand_main_window(window);

			main_ui->pos = (Vector2D){0.0f, 0.0f};
			menu->ui_lain.sprite.pos = (Vector2D){40.0f, 40.0f};
			menu->main_ui_bar.pos = (Vector2D){136.0f, 72.0f};

			menu->clock.pos = (Vector2D){104.0f, 56.0f};

			menu->bear_icon.visible = true;
			menu->screwdriver_icon.visible = true;
			menu->paw_icon.visible = true;
		}
		sprite_try_next_frame(game_state->time, main_ui);
	} else {
		// completed expanding
		menu->state = EXPANDED;
		menu->dressup_button.visible = true;
		menu->theater_button.visible = true;

		menu->main_ui_bar.texture = &textures[MAIN_UI_BAR_ACTIVE];
	}
}

static void animate_menu_shrink(GameState *game_state, Texture *textures,
				Menu *menu, GLFWwindow *window)
{
	Sprite *main_ui = &menu->main_ui;

	if (main_ui->frame_index > 0) {
		sprite_try_next_frame(game_state->time, main_ui);
		if (main_ui->frame_index == main_ui->max_frame - 1) {
			menu->dressup_button.visible = false;
			menu->theater_button.visible = false;
			menu->score_preview.visible = false;
			menu->score_text.visible = false;
		}
		if (main_ui->frame_index == 1) {
			shrink_main_window(window);
		}
	} else {
		menu->state = SHRINKED;
		sprite_set_to_origin_pos(main_ui);
		sprite_set_to_origin_pos(&menu->ui_lain.sprite);
		sprite_set_to_origin_pos(&menu->main_ui_bar);

		menu->clock.pos = (Vector2D){70.0f, 22.0f};

		menu->bear_icon.visible = false;
		menu->screwdriver_icon.visible = false;
		menu->paw_icon.visible = false;

		menu->main_ui_bar.texture = &textures[MAIN_UI_BAR_INACTIVE];
	}
}

static void update_menu_icons(Menu *menu)
{
	Sprite *bear_icon = &menu->bear_icon;
	Sprite *screwdriver_icon = &menu->screwdriver_icon;
	Sprite *paw_icon = &menu->paw_icon;

	struct tm *curr_time = menu->current_time;

	int secs = curr_time->tm_sec - 15;
	int mins = curr_time->tm_min - 15;
	int hrs = curr_time->tm_hour - 3;

	float deg_60 = PI / 3.0f;
	float deg_30 = PI / 6.0f;

	float secs_angle = secs * (deg_60 / 10);
	float mins_angle = mins * (deg_60 / 10);
	float hrs_angle = hrs * deg_30;

	float radius = 44.0f;

	screwdriver_icon->pos = (Vector2D){
	    screwdriver_icon->origin_pos.x + cos(secs_angle) * radius,
	    screwdriver_icon->origin_pos.y + sin(secs_angle) * radius};

	bear_icon->pos =
	    (Vector2D){bear_icon->origin_pos.x + cos(mins_angle) * radius,
		       bear_icon->origin_pos.y + sin(mins_angle) * radius};

	paw_icon->pos =
	    (Vector2D){paw_icon->origin_pos.x + cos(hrs_angle) * radius,
		       paw_icon->origin_pos.y + sin(hrs_angle) * radius};
}

void update_menu_lain(Resources *resources, GameState *game_state,
		      struct tm *current_time, MenuLain *lain)
{

	if ((17 < current_time->tm_hour) || (current_time->tm_hour < 6)) {
		lain->sprite.texture = &resources->textures[UI_LAIN_BEAR];
	} else {
		lain->sprite.texture = &resources->textures[UI_LAIN];
	}

	if (current_time->tm_sec % 15 == 0) {
		if (!lain->recently_changed_laugh) {
			unsigned int quarter = current_time->tm_sec / 15;
			if (quarter == lain->laugh_quarter) {
				if (!lain->laughing) {
					lain->laughing = true;
					sprite_set_animation(
					    &lain->sprite,
					    &resources->animations
						 [UI_LAIN_LAUGH_ANIMATION]);
				} else {
					lain->laughing = false;
					sprite_set_animation(
					    &lain->sprite,
					    &resources->animations
						 [UI_LAIN_STOP_LAUGH_ANIMATION]);
				}
				lain->recently_changed_laugh = true;
			} else if (!lain->blinking) {
				lain->blinking = true;
				if (!lain->laughing) {
					sprite_set_animation(
					    &lain->sprite,
					    &resources->animations
						 [UI_LAIN_BLINK_ANIMATION]);
				} else {
					sprite_set_animation(
					    &lain->sprite,
					    &resources->animations
						 [UI_LAIN_LAUGH_BLINK_ANIMATION]);
				}
			}
		}
	} else {
		lain->recently_changed_laugh = false;
		lain->blinking = false;
	}

	if (lain->sprite.animation != NULL) {
		sprite_try_next_frame(game_state->time, &lain->sprite);
	}
}

void update_menu(Menu *menu, GameState *game_state, GLFWwindow *window,
		 Resources *resources)
{
	update_menu_time(menu);

	char timestring[11];
	get_menu_timestring(timestring, menu);
	update_text(&menu->clock, timestring);

	char score[16];
	sprintf(score, "%d", game_state->score);
	update_text(&menu->score_text, score);

	update_menu_lain(resources, game_state, menu->current_time,
			 &menu->ui_lain);

	update_menu_icons(menu);

	update_scene(&menu->scene);

	switch (menu->state) {
	case EXPANDING:
		animate_menu_expand(game_state, resources->textures, menu,
				    window);
		break;
	case SHRINKING:
		animate_menu_shrink(game_state, resources->textures, menu,
				    window);
		break;
	default:
		break;
	}
}

void init_menu(Menu *menu, GameState *game_state, Resources *resources)
{
	update_menu_time(menu);

	menu->state = SHRINKED;

	menu->ui_lain.recently_changed_laugh = false;
	menu->ui_lain.laughing = false;
	menu->ui_lain.blinking = false;
	menu->ui_lain.laugh_quarter = menu->current_time->tm_sec / 15;

	init_menu_sprites(menu, resources->textures);

	init_menu_text_objects(menu, resources->fonts, game_state);

	Sprite *sprites[] = {
	    &menu->ui_lain.sprite,   &menu->main_ui,
	    &menu->main_ui_bar,	     &menu->dressup_button,
	    &menu->theater_button,   &menu->bear_icon,
	    &menu->screwdriver_icon, &menu->paw_icon,
	    &menu->theater_preview,  &menu->score_preview,
	};
	uint8_t sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	Text *text_objs[] = {&menu->clock, &menu->score_text};
	uint8_t text_obj_count = sizeof(text_objs) / sizeof(text_objs[0]);

	// behavior definitions for sprites
	SpriteBehavior sprite_behaviors[] = {
	    (SpriteBehavior){.sprite = &menu->main_ui_bar,
			     .click_event = MAIN_UI_BAR_CLICK},

	    (SpriteBehavior){.sprite = &menu->theater_button,
			     .click_event = TOGGLE_THEATER_PREVIEW},

	    (SpriteBehavior){.sprite = &menu->ui_lain.sprite,
			     .click_event = TOGGLE_SCORE_PREVIEW},

	    (SpriteBehavior){.sprite = &menu->bear_icon,
			     .click_event = BEAR_ICON_CLICK},

	    (SpriteBehavior){.sprite = &menu->dressup_button,
			     .click_event = DRESSUP_TOGGLE}

	};
	uint8_t sprite_behavior_count =
	    sizeof(sprite_behaviors) / sizeof(sprite_behaviors[0]);

	// TODO delete this
	// only here to check actual clickboxes of sprites, temporary.
	/* Sprite click_barriers[] = { */
	/*     make_click_barrier(136, 73, 192, 79), */
	/*     make_click_barrier(125, 111, 168, 145), */
	/*     make_click_barrier(36, 145, 101, 177), */
	/* }; */

	/* uint8_t click_barrier_count = */
	/*     sizeof(click_barriers) / sizeof(click_barriers[0]); */

	init_scene(&menu->scene, sprites, sprite_count, sprite_behaviors,
		   sprite_behavior_count, text_objs, text_obj_count, NULL, 0);

	/* menu->scene.draw_barriers = true; */
}

void handle_menu_event(MenuEvent event, void *game)
{
	Engine *engine = (Engine *)game;
	Menu *menu = &engine->menu;
	Resources *resources = &engine->resources;

	switch (event) {
	case MAIN_UI_BAR_CLICK: {
		Sprite *theater_preview = &menu->theater_preview;
		Sprite *main_ui = &menu->main_ui;
		Animation *animations = resources->animations;

		if (theater_preview->visible) {
			if (!sprite_is_max_frame(theater_preview)) {
				theater_preview->frame_index++;
			} else {
				theater_preview->frame_index = 0;
			}
		} else {
			if (engine->menu.state == SHRINKED) {
				menu->state = EXPANDING;
				sprite_set_animation(
				    main_ui,
				    &animations[MAIN_UI_EXPAND_ANIMATION]);
			} else {
				menu->state = SHRINKING;
				sprite_set_animation(
				    main_ui,
				    &animations[MAIN_UI_SHRINK_ANIMATION]);
			}
		}
		break;
	}
	case TOGGLE_THEATER_PREVIEW: {
		Sprite *theater_preview = &menu->theater_preview;
		theater_preview->visible = !theater_preview->visible;

		menu->theater_button.texture =
		    &resources->textures[theater_preview->visible
					     ? THEATER_BUTTON_ACTIVE
					     : THEATER_BUTTON_INACTIVE];
		break;
	}
	case TOGGLE_SCORE_PREVIEW: {
		Sprite *score_preview = &menu->score_preview;
		Text *score = &menu->score_text;

		if (menu->state == EXPANDED) {
			score_preview->visible = !score_preview->visible;
			score->visible = !score->visible;
		}
		break;
	}
	case BEAR_ICON_CLICK: {
		MinigameType minigame_type = engine->minigame.type;

		switch (minigame_type) {
		case DRESSUP: {
			glfwSetWindowShouldClose(engine->minigame_window, 1);
			engine->minigame.queued_start = start_kumashoot;
			break;
		}
		case KUMASHOOT: {
			glfwSetWindowShouldClose(engine->minigame_window, 1);
			break;
		}
		case NO_MINIGAME: {
			engine->minigame.queued_start = start_kumashoot;
			break;
		}
		}

		break;
	}
	case DRESSUP_TOGGLE: {
		MinigameType minigame_type = engine->minigame.type;

		switch (minigame_type) {
		case DRESSUP: {
			glfwSetWindowShouldClose(engine->minigame_window, 1);
			break;
		}
		case KUMASHOOT: {
			glfwSetWindowShouldClose(engine->minigame_window, 1);
			engine->minigame.queued_start = start_dressup;
			break;
		}
		case NO_MINIGAME: {
			engine->minigame.queued_start = start_dressup;
			break;
		}
		}

		break;
	}
	}
}
