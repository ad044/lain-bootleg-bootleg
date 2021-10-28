#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "input.h"
#include "kumashoot.h"
#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
#include "text.h"
#include "texture.h"
#include "util.h"
#include "window.h"

static void update_menu_time(Menu *menu);
static void get_menu_timestring(char *target, Menu *menu);
static int init_menu_scene(Menu *menu, GameState *game_state,
			   ResourceCache *resource_cache);
static void animate_menu_shrink(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache);
static void animate_menu_expand(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache);
static void animate_menu(Menu *menu, GLFWwindow *window,
			 ResourceCache *resource_cache);
static void main_ui_bar_click(void *ctx, Sprite *clicked_sprite,
			      Vector2D click_pos);
static void toggle_theater_preview(void *ctx, Sprite *clicked_sprite,
				   Vector2D click_pos);
static void toggle_score_preview(void *ctx, Sprite *clicked_sprite,
				 Vector2D click_pos);

static int init_menu_scene(Menu *menu, GameState *game_state,
			   ResourceCache *resource_cache)
{
	// sprites
	SceneSprite sprites[] = {
	    (SceneSprite){.loc = &menu->sprites->lain->sprite,
			  .sprite = (Sprite){.pos = {6.0f, 6.0f},
					     .size = {64.0f, 64.0f},
					     .texture = texture_cache_get(
						 resource_cache->textures,
						 "ui_lain_bear"),
					     .is_spritesheet = true,
					     .max_frame = 8,
					     .visible = true,
					     .z_index = 4}},
	    (SceneSprite){.loc = &menu->sprites->main_ui,
			  .sprite =
			      (Sprite){
				  .pos = {-34.0f, -70.0f},
				  .size = {200.0f, 200.0f},
				  .texture = texture_cache_get(
				      resource_cache->textures, "main_ui"),
				  .is_spritesheet = true,
				  .max_frame = 6,
				  .visible = true,
				  .z_index = 3,
			      }},
	    (SceneSprite){
		.loc = &menu->sprites->main_ui_bar,
		.sprite =
		    (Sprite){
			.pos = {102.0f, 38.0f},
			.size = {64.0f, 8.0f},
			.texture = texture_cache_get(resource_cache->textures,
						     "main_ui_bar_inactive"),
			.visible = true,
			.z_index = 4,
		    }},
	    (SceneSprite){
		.loc = &menu->sprites->dressup_button,
		.sprite =
		    (Sprite){
			.pos = {112.0f, 96.0f},
			.size = {72.0f, 72.0f},
			.texture = texture_cache_get(resource_cache->textures,
						     "dressup_button_inactive"),
			.visible = false,
			.z_index = 4,
		    }},
	    (SceneSprite){
		.loc = &menu->sprites->theater_preview,
		.sprite =
		    (Sprite){
			.pos = {104.0f, 80.0f},
			.size = {96.0f, 64.0f},
			.texture = texture_cache_get(resource_cache->textures,
						     "theater_previews"),
			.visible = false,
			.z_index = 7,
			.is_spritesheet = true,
			.max_frame = 6,
		    }},
	    (SceneSprite){
		.loc = &menu->sprites->theater_button,
		.sprite =
		    (Sprite){
			.pos = {0.0f, 128.0f},
			.size = {128.0f, 64.0f},
			.texture = texture_cache_get(resource_cache->textures,
						     "theater_button_inactive"),
			.visible = false,
			.z_index = 4,
		    }},
	    (SceneSprite){
		.loc = &menu->sprites->bear_icon,
		.sprite =
		    (Sprite){
			.pos = {56.0f, 56.0f},
			.size = {32.0f, 32.0f},
			.texture_index = 4,
			.texture = texture_cache_get(resource_cache->textures,
						     "bear_icon_inactive"),
			.visible = false,
			.z_index = 1,
		    }},
	    (SceneSprite){.loc = &menu->sprites->screwdriver_icon,
			  .sprite =
			      (Sprite){
				  .pos = {56.0f, 56.0f},
				  .size = {32.0f, 32.0f},
				  .texture = texture_cache_get(
				      resource_cache->textures,
				      "screwdriver_icon_inactive"),
				  .visible = false,
				  .z_index = 2,
			      }},
	    (SceneSprite){.loc = &menu->sprites->paw_icon,
			  .sprite =
			      (Sprite){
				  .pos = {56.0f, 56.0f},
				  .size = {32.0f, 32.0f},
				  .texture = texture_cache_get(
				      resource_cache->textures, "paw_icon"),
				  .visible = false,
				  .z_index = 1,
			      }},
	    (SceneSprite){.loc = &menu->sprites->score_preview,
			  .sprite = (Sprite){
			      .pos = {0.0f, 0.0f},
			      .size = {200.0f, 88.0f},
			      .texture_index = 10,
			      .texture = texture_cache_get(
				  resource_cache->textures, "score_preview"),
			      .visible = false,
			      .z_index = 7}}};
	unsigned int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	char timestring[11];
	get_menu_timestring(timestring, menu);

	char score[16];
	sprintf(score, "%d", game_state->score);

	SceneText text_objects[] = {
	    (SceneText){.loc = &menu->text_objs->clock,
			.text =
			    (Text){.pos = {70.0f, 22.0f},
				   .current_text = timestring,
				   .glyph_size = {32.0f, 16.0f},
				   .visible = true,
				   .font = resource_cache->fonts[WHITE_FONT]}},
	    (SceneText){.loc = &menu->text_objs->score,
			.text =
			    (Text){.pos = {178.0f, 16.0f},
				   .current_text = score,
				   .glyph_size = {10.0f, 16.0f},
				   .visible = false,
				   .left_aligned = true,
				   .font = resource_cache->fonts[RED_FONT]}}};
	unsigned int text_obj_count =
	    sizeof(text_objects) / sizeof(text_objects[0]);

	// behavior definitions for sprites
	SpriteBehavior behaviors[] = {
	    (SpriteBehavior){.sprite = &menu->sprites->main_ui_bar,
			     .on_click = &main_ui_bar_click},

	    (SpriteBehavior){.sprite = &menu->sprites->theater_button,
			     .on_click = &toggle_theater_preview},

	    (SpriteBehavior){.sprite = &menu->sprites->lain->sprite,
			     .on_click = &toggle_score_preview},

	    (SpriteBehavior){.sprite = &menu->sprites->bear_icon,
			     .on_click = &start_kumashoot}

	};
	unsigned int behavior_count = sizeof(behaviors) / sizeof(behaviors[0]);

	SceneDefinition menu_scene_def = {
	    .sprites = sprites,
	    .sprite_count = sprite_count,
	    .behaviors = behaviors,
	    .behavior_count = behavior_count,
	    .text_objects = text_objects,
	    .text_object_count = text_obj_count,
	};

	if (!(init_scene(menu->scene, &menu_scene_def, resource_cache))) {
		printf("Failed to initialize menu scene.\n");
		return 0;
	};

	return 1;
}

int init_menu(ResourceCache *resource_cache, GameState *game_state, Menu **menu)
{
	// allocate mem for the menu struct
	*menu = malloc(sizeof(Menu));
	if (menu == NULL) {
		printf("Failed to allocate memory for the menu.\n");
		return 0;
	}

	update_menu_time((*menu));

	(*menu)->sprites = malloc(sizeof(MenuSprites));
	if ((*menu)->sprites == NULL) {
		printf("Failed to allocate memory for menu sprites.\n");
		return 0;
	}

	(*menu)->sprites->lain = malloc(sizeof(MenuLain));
	if ((*menu)->sprites->lain == NULL) {
		printf("Failed to allocate memory for menu Lain.\n");
		return 0;
	}

	(*menu)->sprites->lain->blink_state = NOT_BLINKING;

	(*menu)->text_objs = malloc(sizeof(MenuTextObjects));
	if ((*menu)->text_objs == NULL) {
		printf("Failed to allocate memory for menu text objects.\n");
		return 0;
	}

	(*menu)->scene = malloc(sizeof(Scene));
	if ((*menu)->scene == NULL) {
		printf("Failed to allocate memory for menu scene.\n");
		return 0;
	}

	// load scene
	if (!(init_menu_scene(*menu, game_state, resource_cache))) {
		printf("Failed to initialize menu scene.\n");
		return 0;
	}

	(*menu)->expanded = false;
	(*menu)->animating = false;

	return 1;
}

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

static void toggle_score_preview(void *ctx, Sprite *clicked_sprite,
				 Vector2D click_pos)
{
	Engine *engine = (Engine *)ctx;

	Sprite *score_preview = engine->menu->sprites->score_preview;
	Text *score = engine->menu->text_objs->score;

	if (engine->menu->expanded) {
		score_preview->visible = !score_preview->visible;
		score->visible = !score->visible;
	}
}

static void toggle_theater_preview(void *ctx, Sprite *clicked_sprite,
				   Vector2D click_pos)
{
	Engine *engine = (Engine *)ctx;

	Sprite *theater_preview = engine->menu->sprites->theater_preview;
	Sprite *theater_button = engine->menu->sprites->theater_button;

	theater_preview->visible = !theater_preview->visible;
	char *new_texture_name = theater_preview->visible
				     ? "theater_button_active"
				     : "theater_button_inactive";

	Texture *new_theater_button_texture = texture_cache_get(
	    engine->resource_cache->textures, new_texture_name);

	update_texture_slot(engine->menu->scene, theater_button,
			    new_theater_button_texture);
}

static void main_ui_bar_click(void *ctx, Sprite *clicked_sprite,
			      Vector2D click_pos)
{
	Engine *engine = (Engine *)ctx;

	Sprite *theater_preview = engine->menu->sprites->theater_preview;

	if (theater_preview->visible) {
		if (theater_preview->current_frame <
		    theater_preview->max_frame) {
			theater_preview->current_frame++;
		} else {
			theater_preview->current_frame = 0;
		}
	} else {
		engine->menu->animating = true;
	}
}

static void animate_menu_expand(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache)
{
	Text *clock = menu->text_objs->clock;

	MenuSprites *sprites = menu->sprites;

	Sprite *main_ui = sprites->main_ui;
	Sprite *main_ui_bar = sprites->main_ui_bar;
	Sprite *bear_icon = sprites->bear_icon;
	Sprite *screwdriver_icon = sprites->screwdriver_icon;
	Sprite *ui_lain = sprites->lain->sprite;
	Sprite *paw_icon = sprites->paw_icon;
	Sprite *dressup_button = sprites->dressup_button;
	Sprite *theater_button = sprites->theater_button;

	if (main_ui->current_frame < main_ui->max_frame) {
		main_ui->current_frame++;
		if (main_ui->current_frame == 1) {
			expand_main_window(window);

			main_ui->pos = (Vector2D){0.0f, 0.0f};
			ui_lain->pos = (Vector2D){40.0f, 40.0f};
			main_ui_bar->pos = (Vector2D){136.0f, 72.0f};

			clock->pos = (Vector2D){104.0f, 56.0f};

			bear_icon->visible = true;
			screwdriver_icon->visible = true;
			paw_icon->visible = true;
		}
	} else {
		// completed expanding
		menu->animating = false;
		menu->expanded = true;
		dressup_button->visible = true;
		theater_button->visible = true;
		update_texture_slot(menu->scene, main_ui_bar,
				    texture_cache_get(resource_cache->textures,
						      "main_ui_bar_active"));
	}
}

static void animate_menu_shrink(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache)
{
	Text *clock = menu->text_objs->clock;
	Text *score = menu->text_objs->score;

	MenuSprites *sprites = menu->sprites;

	Sprite *main_ui = sprites->main_ui;
	Sprite *main_ui_bar = sprites->main_ui_bar;
	Sprite *ui_lain = sprites->lain->sprite;
	Sprite *bear_icon = sprites->bear_icon;
	Sprite *screwdriver_icon = sprites->screwdriver_icon;
	Sprite *paw_icon = sprites->paw_icon;
	Sprite *dressup_button = sprites->dressup_button;
	Sprite *theater_button = sprites->theater_button;
	Sprite *score_preview = sprites->score_preview;

	if (main_ui->current_frame > 0) {
		main_ui->current_frame--;
		if (main_ui->current_frame == main_ui->max_frame - 1) {
			dressup_button->visible = false;
			theater_button->visible = false;
			score_preview->visible = false;
			score->visible = false;
		}
		if (main_ui->current_frame == 1) {
			shrink_main_window(window);

			main_ui->pos = main_ui->origin_pos;
			ui_lain->pos = ui_lain->origin_pos;
			main_ui_bar->pos = main_ui_bar->origin_pos;

			clock->pos = (Vector2D){70.0f, 22.0f};

			bear_icon->visible = false;
			screwdriver_icon->visible = false;
			paw_icon->visible = false;

			update_texture_slot(
			    menu->scene, main_ui_bar,
			    texture_cache_get(resource_cache->textures,
					      "main_ui_bar_inactive"));
		}
	} else {
		// completed shrinking
		menu->animating = false;
		menu->expanded = false;
	}
}

static void animate_menu(Menu *menu, GLFWwindow *window,
			 ResourceCache *resource_cache)
{
	if (menu->expanded) {
		animate_menu_shrink(menu, window, resource_cache);
	} else {
		animate_menu_expand(menu, window, resource_cache);
	}
}

static void update_menu_icons(Menu *menu)
{
	Sprite *bear_icon = menu->sprites->bear_icon;
	Sprite *screwdriver_icon = menu->sprites->screwdriver_icon;
	Sprite *paw_icon = menu->sprites->paw_icon;

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

static void animate_lain_blink(MenuLain *lain)
{
	Sprite *sprite = lain->sprite;

	if (lain->blink_state == BLINK_CLOSING) {
		switch (sprite->current_frame) {
		case 0:
		case 1:
		case 4:
		case 5:
			sprite->current_frame++;
			break;
		case 2:
		case 6:
			lain->blink_state = BLINK_OPENING;
			break;
		}
	} else if (lain->blink_state == BLINK_OPENING) {
		switch (sprite->current_frame) {
		case 0:
		case 4:
			lain->blink_state = HAS_BLINKED;
			break;
		case 1:
		case 5:
		case 2:
		case 6:
			sprite->current_frame--;
			break;
		}
	}
}

void update_menu(Menu *menu, GameState *game_state, GLFWwindow *window,
		 ResourceCache *resource_cache)
{
	update_menu_time(menu);

	char timestring[11];
	get_menu_timestring(timestring, menu);

	char score[16];
	sprintf(score, "%d", game_state->score);

	update_text(menu->text_objs->clock, timestring);
	update_text(menu->text_objs->score, score);

	if (menu->current_time->tm_sec % 15 == 0) {
		MenuLain *lain = menu->sprites->lain;
		if (lain->blink_state != HAS_BLINKED) {
			if (lain->blink_state == NOT_BLINKING) {
				lain->blink_state = BLINK_CLOSING;
			}
			animate_lain_blink(lain);
		}
	} else {
		menu->sprites->lain->blink_state = NOT_BLINKING;
	}

	if (menu->animating) {
		animate_menu(menu, window, resource_cache);
	}

	update_menu_icons(menu);

	update_scene(menu->scene);
}
