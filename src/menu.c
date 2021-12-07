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
#include "resources.h"
#include "scene.h"
#include "shader.h"
#include "sound.h"
#include "sprite.h"
#include "state.h"
#include "text.h"
#include "texture.h"
#include "theater.h"
#include "vector2d.h"
#include "window.h"

#define PI 3.14159265359

static inline _Bool is_bear(struct tm *current_time)
{
	return (17 < current_time->tm_hour) || (current_time->tm_hour < 6);
}

static void update_menu_time(Menu *menu)
{
	time_t t;

	time(&t);

	menu->current_time = localtime(&t);
}

static void get_menu_timestring(char *target, Menu *menu)
{
	strftime(target, sizeof(char) * 16, "%p%I:%M:%S", menu->current_time);
}

static void init_menu_sprites(struct tm *current_time, Resources *resources,
			      Menu *menu)
{
	// texture for this gets set later on
	make_sprite(
	    &menu->ui_lain.sprite,
	    (Sprite){.pos = {6.0f, 6.0f},
		     .hitbox_size = {32.0f, 32.0f},
		     .texture = is_bear(current_time)
				    ? texture_get(resources, UI_BEAR_LAIN_1)
				    : texture_get(resources, UI_DEFAULT_LAIN_1),
		     .z_index = 4,
		     .visible = true});

	make_sprite(&menu->main_ui,
		    (Sprite){.pos = {-34.0f, -34.0f},
			     .texture = texture_get(resources, MAIN_UI_1),
			     .z_index = 3,
			     .visible = true});

	make_sprite(&menu->main_ui_bar,
		    (Sprite){
			.pos = {102.0f, 38.0f},
			.hitbox_size = {64.0f, 8.0f},
			.texture = texture_get(resources, MAIN_UI_BAR),
			.z_index = 4,
			.visible = true,
		    });

	make_sprite(&menu->dressup_button,
		    (Sprite){
			.pos = {112.0f, 96.0f},
			.hitbox_size = {36.0f, 36.0f},
			.texture = texture_get(resources, DRESSUP_BUTTON),
			.z_index = 4,
			.visible = false,
		    });

	make_sprite(&menu->theater_preview,
		    (Sprite){
			.pos = {104.0f, 80.0f},
			.hitbox_size = {96.0f, 64.0f},
			.texture = texture_get(resources, CLASSROOM_PREVIEW),
			.z_index = 7,
			.visible = false,
		    });

	make_sprite(&menu->theater_button,
		    (Sprite){
			.pos = {0.0f, 128.0f},
			.hitbox_size = {64.0f, 32.0f},
			.texture = texture_get(resources, THEATER_BUTTON),
			.z_index = 4,
			.visible = false,
		    });

	make_sprite(&menu->bear_icon,
		    (Sprite){
			.pos = {56.0f, 56.0f},
			.hitbox_size = {32.0f, 32.0f},
			.texture = texture_get(resources, BEAR_ICON),
			.z_index = 1,
			.visible = false,
		    });

	make_sprite(&menu->screwdriver_icon,
		    (Sprite){
			.pos = {56.0f, 56.0f},
			.texture = texture_get(resources, SCREWDRIVER_ICON),
			.z_index = 2,
			.visible = false,
		    });

	make_sprite(&menu->paw_icon,
		    (Sprite){
			.pos = {56.0f, 56.0f},
			.texture = texture_get(resources, PAW_ICON),
			.z_index = 1,
			.visible = false,
		    });

	make_sprite(&menu->score_preview,
		    (Sprite){
			.pos = {0.0f, 0.0f},
			.texture = texture_get(resources, SCORE_DISPLAY),
			.z_index = 7,
			.visible = false,
		    });

	make_sprite(&menu->background,
		    (Sprite){
			.pos = {0.0f, 0.0f},
			.texture = texture_get(resources, MENU_BG),
			.z_index = 0,
			.visible = true,
		    });
}

static void init_menu_text_objects(Font *fonts, GameState *game_state,
				   Menu *menu)
{
	menu->clock = (Text){.pos = {70.0f, 22.0f},
			     .glyph_size = {32.0f, 16.0f},
			     .visible = true,
			     .font = &fonts[FONT_WHITE]};
	get_menu_timestring(menu->clock.current_text, menu);

	menu->score_text = (Text){.pos = {178.0f, 16.0f},
				  .glyph_size = {10.0f, 16.0f},
				  .visible = false,
				  .left_aligned = true,
				  .font = &fonts[FONT_RED]};
	sprintf(menu->score_text.current_text, "%d", game_state->score);
}

static void animate_menu(GameState *game_state, Resources *resources,
			 Menu *menu, GLFWwindow *window)
{
	Sprite *main_ui = &menu->main_ui;

	if (menu->collapsed) {
		if (main_ui->animation_frame->index == 0) {
			glfwSetWindowSize(window, EXPANDED_MENU_WIDTH,
					  EXPANDED_MENU_HEIGHT);

			main_ui->pos = (Vector2D){0.0f, 0.0f};
			menu->ui_lain.sprite.pos = (Vector2D){40.0f, 40.0f};
			menu->main_ui_bar.pos = (Vector2D){136.0f, 72.0f};

			menu->clock.pos = (Vector2D){104.0f, 56.0f};

			menu->bear_icon.visible = true;
			menu->screwdriver_icon.visible = true;
			menu->paw_icon.visible = true;
		}
	} else {
		if (main_ui->animation_frame->index == 1) {
			menu->dressup_button.visible = false;
			menu->theater_button.visible = false;
			menu->score_preview.visible = false;
			menu->score_text.visible = false;
		}
		if (main_ui->animation_frame->index == 5) {
			glfwSetWindowSize(window, COLLAPSED_MENU_WIDTH,
					  COLLAPSED_MENU_HEIGHT);

			sprite_set_to_origin_pos(main_ui);
			sprite_set_to_origin_pos(&menu->ui_lain.sprite);
			sprite_set_to_origin_pos(&menu->main_ui_bar);

			menu->clock.pos = (Vector2D){70.0f, 22.0f};

			menu->bear_icon.visible = false;
			menu->screwdriver_icon.visible = false;
			menu->paw_icon.visible = false;
		}
	}

	if (sprite_animation_is_last_frame(&menu->main_ui)) {
		menu->collapsed = !menu->collapsed;
		if (menu->collapsed) {
			menu->main_ui_bar.texture =
			    texture_get(resources, MAIN_UI_BAR);
		} else {
			menu->main_ui_bar.texture =
			    texture_get(resources, MAIN_UI_BAR_ACTIVE);
			menu->dressup_button.visible = true;
			menu->theater_button.visible = true;
		}
	}

	sprite_try_next_frame(game_state->time, &menu->main_ui);
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

static int get_menu_lain_animation(struct tm *current_time,
				   MenuLainAnimation animation_type)
{
	if (is_bear(current_time)) {
		switch (animation_type) {
		case LAUGH: {
			return UI_BEAR_LAIN_LAUGH_ANIMATION;
		}
		case BLINK: {
			return UI_BEAR_LAIN_BLINK_ANIMATION;
		}
		case LAUGH_BLINK: {
			return UI_BEAR_LAIN_LAUGH_BLINK_ANIMATION;
		}
		}
	} else {
		switch (animation_type) {
		case LAUGH: {
			return UI_DEFAULT_LAIN_LAUGH_ANIMATION;
		}
		case BLINK: {
			return UI_DEFAULT_LAIN_BLINK_ANIMATION;
		}
		case LAUGH_BLINK: {
			return UI_DEFAULT_LAIN_LAUGH_BLINK_ANIMATION;
		}
		}
	}
}

void update_menu_lain(Resources *resources, GameState *game_state,
		      struct tm *current_time, MenuLain *lain)
{
	if (current_time->tm_sec % 15 == 0) {
		if (!lain->recently_changed_laugh) {
			unsigned int quarter = current_time->tm_sec / 15;
			if (quarter == lain->laugh_quarter) {
				if (!lain->laughing) {
					sprite_set_animation(
					    resources, game_state->time,
					    &lain->sprite,
					    get_menu_lain_animation(
						current_time, LAUGH));

					lain->laughing = !lain->laughing;
					lain->recently_changed_laugh = true;
				} else {
					lain->laughing = !lain->laughing;
					lain->recently_changed_laugh = true;

					sprite_set_animation(
					    resources, game_state->time,
					    &lain->sprite,
					    get_menu_lain_animation(
						current_time, BLINK));
				}
			} else if (!lain->blinking) {
				if (!lain->laughing) {
					sprite_set_animation(
					    resources, game_state->time,
					    &lain->sprite,
					    get_menu_lain_animation(
						current_time, BLINK));
				} else {
					sprite_set_animation(
					    resources, game_state->time,
					    &lain->sprite,
					    get_menu_lain_animation(
						current_time, LAUGH_BLINK));
				}
				lain->blinking = true;
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

	char timestring[16];
	get_menu_timestring(timestring, menu);
	update_text(&menu->clock, timestring);

	char score[16];
	sprintf(score, "%d", game_state->score);
	update_text(&menu->score_text, score);

	update_menu_lain(resources, game_state, menu->current_time,
			 &menu->ui_lain);

	update_menu_icons(menu);

	update_scene(&menu->scene);

	if (menu->main_ui.animation != NULL) {
		animate_menu(game_state, resources, menu, window);
	}
}

void init_menu(Menu *menu, GameState *game_state, Resources *resources)
{
	update_menu_time(menu);

	menu->collapsed = true;

	menu->ui_lain.recently_changed_laugh = false;
	menu->ui_lain.laughing = false;
	menu->ui_lain.blinking = false;
	menu->ui_lain.laugh_quarter = menu->current_time->tm_sec / 15;

	init_menu_sprites(menu->current_time, resources, menu);

	init_menu_text_objects(resources->fonts, game_state, menu);

	Sprite *sprites[] = {
	    &menu->ui_lain.sprite,   &menu->main_ui,
	    &menu->main_ui_bar,	     &menu->dressup_button,
	    &menu->theater_button,   &menu->bear_icon,
	    &menu->screwdriver_icon, &menu->paw_icon,
	    &menu->theater_preview,  &menu->score_preview,
	    &menu->background,
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
			     .click_event = DRESSUP_TOGGLE},

	    (SpriteBehavior){.sprite = &menu->theater_preview,
			     .click_event = THEATER_TOGGLE}

	};
	uint8_t sprite_behavior_count =
	    sizeof(sprite_behaviors) / sizeof(sprite_behaviors[0]);

	init_scene(&menu->scene, sprites, sprite_count, sprite_behaviors,
		   sprite_behavior_count, text_objs, text_obj_count, NULL, 0);
}

static TextureID get_next_theater_preview(GameState *game_state,
					  TextureID current_texture_id)
{
	switch (current_texture_id) {
	case CLASSROOM_PREVIEW:
		return SCHOOL_PREVIEW;
	case SCHOOL_PREVIEW:
		return LAIN_ROOM_NIGHT_PREVIEW;
	case LAIN_ROOM_NIGHT_PREVIEW:
		return ARISU_ROOM_PREVIEW;
	case ARISU_ROOM_PREVIEW:
		return CYBERIA_PREVIEW;
	case CYBERIA_PREVIEW:
		return STREET_PREVIEW;
	case STREET_PREVIEW:
		return game_state->score >= 2000 ? BRIDGE_PREVIEW
						 : CLASSROOM_PREVIEW;
	case BRIDGE_PREVIEW:
		return CLASSROOM_PREVIEW;
	default:
		return 0;
	}
}

void handle_menu_event(MenuEvent event, Engine *engine)
{
	Menu *menu = &engine->menu;
	Resources *resources = &engine->resources;

	switch (event) {
	case MAIN_UI_BAR_CLICK: {
		Sprite *theater_preview = &menu->theater_preview;
		Sprite *main_ui = &menu->main_ui;

		if (theater_preview->visible) {
			if (engine->minigame.type != THEATER) {
				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_114);
				menu->theater_preview.texture = texture_get(
				    resources,
				    get_next_theater_preview(
					&engine->game_state,
					menu->theater_preview.texture->id));
			}
		} else {
			if (engine->menu.collapsed) {
				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_112);

				sprite_set_animation(
				    resources, engine->game_state.time, main_ui,
				    MAIN_UI_EXPAND_ANIMATION);
			} else if (engine->minigame.type != DRESSUP) {
				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_113);

				sprite_set_animation(
				    resources, engine->game_state.time, main_ui,
				    MAIN_UI_COLLAPSE_ANIMATION);
			}
		}
		break;
	}
	case TOGGLE_THEATER_PREVIEW: {
		if (engine->minigame.type != THEATER &&
		    engine->minigame.type != DRESSUP) {
			Sprite *theater_preview = &menu->theater_preview;

			if (theater_preview->visible) {
				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_111);
			} else {

				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_110);
			}

			theater_preview->visible = !theater_preview->visible;

			menu->theater_button.texture =
			    texture_get(resources, theater_preview->visible
						       ? THEATER_BUTTON_ACTIVE
						       : THEATER_BUTTON);
		}
		break;
	}
	case TOGGLE_SCORE_PREVIEW: {
		Sprite *score_preview = &menu->score_preview;
		Text *score = &menu->score_text;

		if (!menu->collapsed) {
			if (score_preview->visible) {
				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_111);
			} else {
				enqueue_sound(&engine->game_state.queued_sounds,
					      SND_110);
			}
			score_preview->visible = !score_preview->visible;
			score->visible = !score->visible;
		}
		break;
	}
	case BEAR_ICON_CLICK: {
		MinigameType minigame_type = engine->minigame.type;
		switch (minigame_type) {
		case THEATER:
		case DRESSUP: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_110);

			glfwSetWindowShouldClose(engine->minigame_window, 1);
			engine->minigame.queued_minigame = KUMASHOOT;
			break;
		}
		case KUMASHOOT: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_111);

			glfwSetWindowShouldClose(engine->minigame_window, 1);
			break;
		}
		case NO_MINIGAME: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_110);

			engine->minigame.queued_minigame = KUMASHOOT;
			break;
		}
		}

		break;
	}
	case DRESSUP_TOGGLE: {
		MinigameType minigame_type = engine->minigame.type;

		if (menu->theater_preview.visible) {
			return;
		}

		switch (minigame_type) {
		case DRESSUP: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_111);

			glfwSetWindowShouldClose(engine->minigame_window, 1);
			break;
		}
		case KUMASHOOT: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_110);

			glfwSetWindowShouldClose(engine->minigame_window, 1);
			engine->minigame.queued_minigame = DRESSUP;
			break;
		}
		case NO_MINIGAME: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_110);

			engine->minigame.queued_minigame = DRESSUP;
			break;
		}
		case THEATER:
			break;
		}

		break;
	}
	case THEATER_TOGGLE: {
		MinigameType minigame_type = engine->minigame.type;

		switch (minigame_type) {
		case DRESSUP: {
			break;
		}
		case KUMASHOOT: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_111);

			glfwSetWindowShouldClose(engine->minigame_window, 1);
			engine->minigame.queued_minigame = THEATER;
			break;
		}
		case THEATER: {
			glfwSetWindowShouldClose(engine->minigame_window, 1);
			break;
		}
		case NO_MINIGAME: {
			enqueue_sound(&engine->game_state.queued_sounds,
				      SND_110);

			engine->minigame.queued_minigame = THEATER;
			break;
		}
		}
		break;
	}
	}
}
