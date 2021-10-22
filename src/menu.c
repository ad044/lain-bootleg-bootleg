#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "input.h"
#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
#include "text.h"
#include "texture.h"
#include "window.h"

static void update_menu_time(Menu *menu);
static void get_menu_timestring(char *target, Menu *menu);
static int init_menu_scene(Menu *menu, ResourceCache *resource_cache);
static void animate_menu_expand(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache);
static void animate_menu(Menu *menu, GLFWwindow *window,
			 ResourceCache *resource_cache);
static void main_ui_bar_click(void *ctx, Sprite *clicked_sprite,
			      Vector2D click_pos);
static void toggle_theater_preview(void *ctx, Sprite *clicked_sprite,
				   Vector2D click_pos);

static int init_menu_scene(Menu *menu, ResourceCache *resource_cache)
{
	// sprites
	SceneSprite sprites[] = {
	    (SceneSprite){.loc = &menu->sprites->lain->sprite,
			  .sprite = (Sprite){.pos = {6.0f, 6.0f},
					     .size = {64.0f, 64.0f},
					     .texture_index = 0,
					     .is_spritesheet = true,
					     .max_frame = 8,
					     .visible = true,
					     .z_index = 4}},
	    (SceneSprite){.loc = &menu->sprites->main_ui,
			  .sprite =
			      (Sprite){
				  .pos = {-34.0f, -70.0f},
				  .size = {200.0f, 200.0f},
				  .texture_index = 1,
				  .is_spritesheet = true,
				  .max_frame = 6,
				  .visible = true,
				  .z_index = 3,
			      }},
	    (SceneSprite){.loc = &menu->sprites->main_ui_bar,
			  .sprite =
			      (Sprite){
				  .pos = {102.0f, 38.0f},
				  .size = {64.0f, 8.0f},
				  .texture_index = 2,
				  .visible = true,
				  .z_index = 4,
			      }},
	    (SceneSprite){.loc = &menu->sprites->dressup_button,
			  .sprite =
			      (Sprite){
				  .pos = {112.0f, 96.0f},
				  .size = {72.0f, 72.0f},
				  .texture_index = 3,
				  .visible = false,
				  .z_index = 4,
			      }},
	    (SceneSprite){.loc = &menu->sprites->theater_preview,
			  .sprite =
			      (Sprite){
				  .pos = {104.0f, 80.0f},
				  .size = {96.0f, 64.0f},
				  .texture_index = 9,
				  .visible = false,
				  .z_index = 7,
				  .is_spritesheet = true,
				  .max_frame = 6,
			      }},
	    (SceneSprite){.loc = &menu->sprites->theater_button,
			  .sprite =
			      (Sprite){
				  .pos = {0.0f, 128.0f},
				  .size = {128.0f, 64.0f},
				  .texture_index = 8,
				  .visible = false,
				  .z_index = 4,
			      }},
	    (SceneSprite){.loc = &menu->sprites->bear_icon,
			  .sprite =
			      (Sprite){
				  .pos = {56.0f, 56.0f},
				  .size = {32.0f, 32.0f},
				  .texture_index = 4,
				  .visible = false,
				  .z_index = 1,
			      }},
	    (SceneSprite){.loc = &menu->sprites->screwdriver_icon,
			  .sprite =
			      (Sprite){
				  .pos = {56.0f, 56.0f},
				  .size = {32.0f, 32.0f},
				  .texture_index = 6,
				  .visible = false,
				  .z_index = 2,
			      }},
	    (SceneSprite){.loc = &menu->sprites->paw_icon,
			  .sprite = (Sprite){
			      .pos = {56.0f, 56.0f},
			      .size = {32.0f, 32.0f},
			      .texture_index = 7,
			      .visible = false,
			      .z_index = 1,
			  }}};
	unsigned int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	char timestring[11];
	get_menu_timestring(timestring, menu);
	SceneText text_objects[] = {(SceneText){
	    .loc = &menu->text_objs->clock,
	    .text_def = (TextDefinition){.pos = {70.0f, 22.0f},
					 .texture_index = 5,
					 .texture_glyph_count = 13.0f,
					 .initial_text = timestring}}};

	unsigned int text_obj_count =
	    sizeof(text_objects) / sizeof(text_objects[0]);

	// behavior definitions for sprites
	SpriteBehavior behaviors[] = {
	    (SpriteBehavior){.sprite = &menu->sprites->main_ui_bar,
			     .on_click = &main_ui_bar_click},
	    (SpriteBehavior){.sprite = &menu->sprites->theater_button,
			     .on_click = &toggle_theater_preview}

	};
	unsigned int behavior_count = sizeof(behaviors) / sizeof(behaviors[0]);

	// texture slots
	SceneTextureSlot *texture_slots[] = {
	    make_texture_slot(
		0, texture_cache_get(resource_cache->textures, "ui_lain_bear")),
	    make_texture_slot(
		1, texture_cache_get(resource_cache->textures, "main_ui")),
	    make_texture_slot(2, texture_cache_get(resource_cache->textures,
						   "main_ui_bar_inactive")),
	    make_texture_slot(3, texture_cache_get(resource_cache->textures,
						   "dressup_button_inactive")),
	    make_texture_slot(4, texture_cache_get(resource_cache->textures,
						   "bear_icon_inactive")),
	    make_texture_slot(
		5, texture_cache_get(resource_cache->textures, "white_font")),
	    make_texture_slot(6,
			      texture_cache_get(resource_cache->textures,
						"screwdriver_icon_inactive")),
	    make_texture_slot(
		7, texture_cache_get(resource_cache->textures, "paw_icon")),
	    make_texture_slot(8, texture_cache_get(resource_cache->textures,
						   "theater_button_inactive")),
	    make_texture_slot(9, texture_cache_get(resource_cache->textures,
						   "theater_previews")),
	};

	unsigned int texture_slot_count =
	    sizeof(texture_slots) / sizeof(texture_slots[0]);

	SceneDefinition menu_scene_def = {
	    .sprites = sprites,
	    .sprite_count = sprite_count,
	    .behaviors = behaviors,
	    .behavior_count = behavior_count,
	    .texture_slots = texture_slots,
	    .texture_slot_count = texture_slot_count,
	    .text_objects = text_objects,
	    .text_obj_count = text_obj_count,
	};

	if (!(init_scene(menu->scene, &menu_scene_def, resource_cache))) {
		printf("Failed to initialize menu scene.\n");
		return 0;
	};

	return 1;
}

int init_menu(ResourceCache *resource_cache, Menu **menu)
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
	if (!(init_menu_scene(*menu, resource_cache))) {
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

			main_ui->pos = make_vec2d(0.0f, 0.0f);
			ui_lain->pos = make_vec2d(40.0f, 40.0f);
			main_ui_bar->pos = make_vec2d(136.0f, 72.0f);

			clock->pos = make_vec2d(104.0f, 56.0f);

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

	MenuSprites *sprites = menu->sprites;

	Sprite *main_ui = sprites->main_ui;
	Sprite *main_ui_bar = sprites->main_ui_bar;
	Sprite *ui_lain = sprites->lain->sprite;
	Sprite *bear_icon = sprites->bear_icon;
	Sprite *screwdriver_icon = sprites->screwdriver_icon;
	Sprite *paw_icon = sprites->paw_icon;
	Sprite *dressup_button = sprites->dressup_button;
	Sprite *theater_button = sprites->theater_button;

	if (main_ui->current_frame > 0) {
		main_ui->current_frame--;
		if (main_ui->current_frame == main_ui->max_frame - 1) {
			dressup_button->visible = false;
			theater_button->visible = false;
		}
		if (main_ui->current_frame == 1) {
			shrink_main_window(window);

			main_ui->pos = main_ui->origin_pos;
			ui_lain->pos = ui_lain->origin_pos;
			main_ui_bar->pos = main_ui_bar->origin_pos;

			clock->pos = make_vec2d(70.0f, 22.0f);

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

	int secs = curr_time->tm_sec;
	int mins = curr_time->tm_min;
	int hrs = curr_time->tm_hour;

	float v1 = 3.14 / 30.0f;
	float v2 = 3.14 / 6.0f;

	// todo the -15 and -3 are hacky
	screwdriver_icon->pos = make_vec2d(
	    screwdriver_icon->origin_pos.x + cos((secs - 15) * v1) * 44.0f,
	    screwdriver_icon->origin_pos.y + sin((secs - 15) * v1) * 44.0f);

	bear_icon->pos =
	    make_vec2d(bear_icon->origin_pos.x + cos((mins - 15) * v1) * 44.0f,
		       bear_icon->origin_pos.y + sin((mins - 15) * v1) * 44.0f);

	paw_icon->pos =
	    make_vec2d(paw_icon->origin_pos.x + cos((hrs - 3) * v2) * 44.0f,
		       paw_icon->origin_pos.y + sin((hrs - 3) * v2) * 44.0f);
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

void update_menu(Menu *menu, GLFWwindow *window, ResourceCache *resource_cache)
{
	update_menu_time(menu);

	char timestring[11];
	get_menu_timestring(timestring, menu);

	menu->text_objs->clock->current_text = timestring;

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

void draw_menu(Menu *menu, GLFWwindow *window)
{
	draw_scene(menu->scene, window);
}
