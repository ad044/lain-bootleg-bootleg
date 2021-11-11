#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "kumashoot.h"
#include "menu.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
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
	menu->ui_lain = (Sprite){.pos = {6.0f, 6.0f},
				 .size = {64.0f, 64.0f},
				 .texture = &textures[UI_LAIN_BEAR],
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
	    .texture = &textures[MAIN_UI_BAR_INACTIVE],
	    .visible = true,
	    .z_index = 4,
	};

	menu->dressup_button = (Sprite){
	    .pos = {112.0f, 96.0f},
	    .size = {72.0f, 72.0f},
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
	    .texture = &textures[THEATER_BUTTON_INACTIVE],
	    .visible = false,
	    .z_index = 4,
	};

	menu->bear_icon = (Sprite){
	    .pos = {56.0f, 56.0f},
	    .size = {32.0f, 32.0f},
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

static void animate_menu_expand(Menu *menu, GLFWwindow *window,
				Texture *textures)
{
	Sprite *main_ui = &menu->main_ui;

	if (main_ui->current_frame < main_ui->max_frame) {
		main_ui->current_frame++;
		if (main_ui->current_frame == 1) {
			expand_main_window(window);

			main_ui->pos = (Vector2D){0.0f, 0.0f};
			menu->ui_lain.pos = (Vector2D){40.0f, 40.0f};
			menu->main_ui_bar.pos = (Vector2D){136.0f, 72.0f};

			menu->clock.pos = (Vector2D){104.0f, 56.0f};

			menu->bear_icon.visible = true;
			menu->screwdriver_icon.visible = true;
			menu->paw_icon.visible = true;
		}
	} else {
		// completed expanding
		menu->animating = false;
		menu->expanded = true;
		menu->dressup_button.visible = true;
		menu->theater_button.visible = true;

		menu->main_ui_bar.texture = &textures[MAIN_UI_BAR_ACTIVE];
	}
}

static void animate_menu_shrink(Menu *menu, GLFWwindow *window,
				Texture *textures)
{
	Sprite *main_ui = &menu->main_ui;

	if (main_ui->current_frame > 0) {
		main_ui->current_frame--;
		if (main_ui->current_frame == main_ui->max_frame - 1) {
			menu->dressup_button.visible = false;
			menu->theater_button.visible = false;
			menu->score_preview.visible = false;
			menu->score_text.visible = false;
		}
		if (main_ui->current_frame == 1) {
			shrink_main_window(window);

			main_ui->pos = main_ui->origin_pos;
			menu->ui_lain.pos = menu->ui_lain.origin_pos;
			menu->main_ui_bar.pos = menu->main_ui_bar.origin_pos;

			menu->clock.pos = (Vector2D){70.0f, 22.0f};

			menu->bear_icon.visible = false;
			menu->screwdriver_icon.visible = false;
			menu->paw_icon.visible = false;

			menu->main_ui_bar.texture =
			    &textures[MAIN_UI_BAR_INACTIVE];
		}
	} else {
		// completed shrinking
		menu->animating = false;
		menu->expanded = false;
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

static void animate_lain_blink(Sprite *ui_lain, BlinkState *blink_state)
{
	if (*blink_state == BLINK_CLOSING) {
		switch (ui_lain->current_frame) {
		case 0:
		case 1:
		case 4:
		case 5:
			ui_lain->current_frame++;
			break;
		case 2:
		case 6:
			*blink_state = BLINK_OPENING;
			break;
		}
	} else if (*blink_state == BLINK_OPENING) {
		switch (ui_lain->current_frame) {
		case 0:
		case 4:
			*blink_state = HAS_BLINKED;
			break;
		case 1:
		case 5:
		case 2:
		case 6:
			ui_lain->current_frame--;
			break;
		}
	}
}

void update_menu(Menu *menu, const GameState *game_state, GLFWwindow *window,
		 Texture *textures)
{
	update_menu_time(menu);

	char timestring[11];
	get_menu_timestring(timestring, menu);

	char score[16];
	sprintf(score, "%d", game_state->score);

	update_text(&menu->clock, timestring);
	update_text(&menu->score_text, score);

	Sprite *ui_lain = &menu->ui_lain;
	BlinkState *lain_blink_state = &menu->lain_blink_state;

	if (menu->current_time->tm_sec % 15 == 0) {
		if (*lain_blink_state != HAS_BLINKED) {
			if (*lain_blink_state == NOT_BLINKING) {
				*lain_blink_state = BLINK_CLOSING;
			}
			animate_lain_blink(ui_lain, lain_blink_state);
		}
	} else {
		*lain_blink_state = NOT_BLINKING;
	}

	if (menu->animating) {
		if (menu->expanded) {
			animate_menu_shrink(menu, window, textures);
		} else {
			animate_menu_expand(menu, window, textures);
		}
	}

	update_menu_icons(menu);

	update_scene(&menu->scene);
}

void init_menu(Menu *menu, GameState *game_state, Resources *resources)
{
	update_menu_time(menu);

	menu->lain_blink_state = NOT_BLINKING;
	menu->expanded = false;
	menu->animating = false;

	init_menu_sprites(menu, resources->textures);

	init_menu_text_objects(menu, resources->fonts, game_state);

	Sprite *sprites[] = {
	    &menu->ui_lain,	     &menu->main_ui,
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

	    (SpriteBehavior){.sprite = &menu->ui_lain,
			     .click_event = TOGGLE_SCORE_PREVIEW},

	    (SpriteBehavior){.sprite = &menu->bear_icon,
			     .click_event = BEAR_ICON_CLICK}

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
		   sprite_behavior_count, text_objs, text_obj_count,
		   NULL, 0);

	/* menu->scene.draw_barriers = true; */
}

void handle_menu_event(MenuEvent event, void *game)
{
	Engine *engine = (Engine *)game;

	switch (event) {
	case MAIN_UI_BAR_CLICK: {
		Sprite *theater_preview = &engine->menu.theater_preview;

		if (theater_preview->visible) {
			if (theater_preview->current_frame <
			    theater_preview->max_frame) {
				theater_preview->current_frame++;
			} else {
				theater_preview->current_frame = 0;
			}
		} else {
			engine->menu.animating = true;
		}
		break;
	}
	case TOGGLE_THEATER_PREVIEW: {
		Sprite *theater_preview = &engine->menu.theater_preview;
		theater_preview->visible = !theater_preview->visible;

		engine->menu.theater_button.texture =
		    &engine->resources.textures[theater_preview->visible
						    ? THEATER_BUTTON_ACTIVE
						    : THEATER_BUTTON_INACTIVE];
		break;
	}
	case TOGGLE_SCORE_PREVIEW: {
		Sprite *score_preview = &engine->menu.score_preview;
		Text *score = &engine->menu.score_text;

		if (engine->menu.expanded) {
			score_preview->visible = !score_preview->visible;
			score->visible = !score->visible;
		}
		break;
	}
	case BEAR_ICON_CLICK: {
		MinigameType minigame_type = engine->minigame.type;

		if (minigame_type != NONE) {
			kill_minigame(&engine->menu, &engine->minigame,
				      &engine->minigame_window,
				      engine->resources.textures);
		}

		if (minigame_type != KUMASHOOT) {
			start_kumashoot(&engine->resources, &engine->game_state,
					&engine->minigame,
					&engine->minigame_window,
					engine->main_window);

			engine->menu.bear_icon.texture =
			    &engine->resources.textures[BEAR_ICON_ACTIVE];
		}
		break;
	}
	}
}
