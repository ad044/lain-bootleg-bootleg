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

static void get_current_time(unsigned char *timestr);
static int init_menu_scene(Menu *menu, ResourceCache *resource_cache);
static void animate_menu(Menu *menu, GLFWwindow *window,
			 ResourceCache *resource_cache);
static void animate_menu_expand(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache);
static void animate_menu(Menu *menu, GLFWwindow *window,
			 ResourceCache *resource_cache);

static int init_menu_scene(Menu *menu, ResourceCache *resource_cache)
{
	// sprites
	SceneSprite sprites[] = {
	    (SceneSprite){.loc = &menu->sprites->lain,
			  .sprite = (Sprite){.pos = {-50.0f, 0.0f},
					     .size = {50.0f, 50.0f},
					     .texture_index = 0,
					     .texture_size = {1.0f, 1.0f},
					     .visible = true,
					     .z_index = 1}},
	    (SceneSprite){.loc = &menu->sprites->main_ui,
			  .sprite =
			      (Sprite){
				  .pos = {-20.0f, 0.0f},
				  .size = {150.0f, 150.0f},
				  .texture_index = 1,
				  .texture_size = {1.0f / 6.0f, 1.0f},
				  .current_frame = 0,
				  .max_frame = 5,
				  .visible = true,
				  .z_index = 0,
			      }},
	    (SceneSprite){.loc = &menu->sprites->main_ui_bar,
			  .sprite =
			      (Sprite){
				  .pos = {0.5f, -0.25f},
				  .size = {100.0f, 10.0f},
				  .texture_index = 2,
				  .texture_size = {1.0f, 1.0f},
				  .visible = true,
				  .z_index = 1,
			      }},
	    (SceneSprite){.loc = &menu->sprites->dressup_button,
			  .sprite =
			      (Sprite){
				  .pos = {0.5f, -0.25f},
				  .size = {50.0f, 50.0f},
				  .texture_index = 3,
				  .texture_size = {1.0f, 1.0f},
				  .visible = false,
				  .z_index = 1,
			      }},
	    (SceneSprite){.loc = &menu->sprites->bear_icon,
			  .sprite = (Sprite){
			      .pos = {0.5f, -0.25f},
			      .size = {25.0f, 25.0f},
			      .texture_index = 4,
			      .texture_size = {1.0f, 1.0f},
			      .visible = false,
			      .z_index = 1,
			  }}};
	unsigned int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	SceneText text_objects[] = {
	    (SceneText){.loc = &menu->text_objs->clock,
			.text_def = (TextDefinition){
			    .pos = {-0.1f, 0.1f},
			    .texture_index = 5,
			    .texture_glyph_count = 13.0f,
			}}};

	unsigned int text_obj_count =
	    sizeof(text_objects) / sizeof(text_objects[0]);

	// behavior definitions for sprites
	SpriteBehavior behaviors[] = {
	    (SpriteBehavior){.sprite = &menu->sprites->main_ui_bar,
			     .on_click = &toggle_menu_animating}

	};
	unsigned int behavior_count = sizeof(behaviors) / sizeof(behaviors[0]);

	// texture slots
	SceneTextureSlot *texture_slots[] = {
	    make_texture_slot(
		0, texture_cache_get(resource_cache->textures, "lain")),
	    make_texture_slot(
		1, texture_cache_get(resource_cache->textures, "main_ui")),
	    make_texture_slot(2, texture_cache_get(resource_cache->textures,
						   "main_ui_bar_inactive")),
	    make_texture_slot(3, texture_cache_get(resource_cache->textures,
						   "dressup_button_inactive")),
	    make_texture_slot(4, texture_cache_get(resource_cache->textures,
						   "bear_icon_inactive")),
	    make_texture_slot(
		5, texture_cache_get(resource_cache->textures, "white_font"))};

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

	(*menu)->sprites = malloc(sizeof(MenuSprites));
	if ((*menu)->sprites == NULL) {
		printf("Failed to allocate memory for menu sprites.\n");
		return 0;
	}

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

static void get_current_time(unsigned char *timestr)
{
	time_t t;
	struct tm *tmp;

	time(&t);

	tmp = localtime(&t);

	strftime((char *)timestr, sizeof(char) * 11, "%p%I:%M:%S", tmp);
}

void toggle_menu_animating(void *ctx, Sprite *clicked_sprite,
			   Vector2D click_pos)
{
	Engine *engine = (Engine *)ctx;

	engine->menu->animating = true;
}

static void animate_menu_expand(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache)
{
	MenuSprites *sprites = menu->sprites;
	Sprite *main_ui = sprites->main_ui;
	Sprite *main_ui_bar = sprites->main_ui_bar;
	Sprite *bear_icon = sprites->bear_icon;

	if (main_ui->current_frame < main_ui->max_frame) {
		main_ui->current_frame++;
		if (main_ui->current_frame == 1) {
			expand_main_window(window);
			sprite_show(bear_icon);
		}
	} else {
		// completed expanding
		menu->animating = false;
		menu->expanded = true;
		sprite_show(sprites->dressup_button);
		update_texture_slot(menu->scene, main_ui_bar,
				    texture_cache_get(resource_cache->textures,
						      "main_ui_bar_active"));
	}
}

static void animate_menu_shrink(Menu *menu, GLFWwindow *window,
				ResourceCache *resource_cache)
{
	MenuSprites *sprites = menu->sprites;
	Sprite *main_ui = sprites->main_ui;
	Sprite *main_ui_bar = sprites->main_ui_bar;
	Sprite *bear_icon = sprites->bear_icon;

	if (main_ui->current_frame > 0) {
		main_ui->current_frame--;
		if (main_ui->current_frame == 1) {
			shrink_main_window(window);
			sprite_hide(sprites->dressup_button);
			sprite_hide(bear_icon);
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

	update_scene(menu->scene);
}

void slice_str(const char *str, char *result, size_t start, size_t end)
{
	strncpy(result, str + start, end - start);
}

static void update_menu_icons(Menu *menu)
{
	char secs[3], mins[3], hrs[3];

	slice_str(menu->current_time, hrs, 2, 4);
	slice_str(menu->current_time, mins, 5, 7);
	slice_str(menu->current_time, secs, 8, 10);

	Sprite *bear_icon = menu->sprites->bear_icon;

	bear_icon->pos =
	    make_vec2d(sin(atof(secs) / 60) * 50, cos(atof(secs) / 60) * 50);
}

void update_menu(Menu *menu, GLFWwindow *window, ResourceCache *resource_cache)
{
	get_current_time(menu->current_time);

	set_text(menu->text_objs->clock, menu->current_time);

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
