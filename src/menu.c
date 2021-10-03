#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "menu.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
#include "text.h"
#include "texture.h"
#include "window.h"

static void get_current_time(unsigned char *timestr);
static int init_clock(Text **clock, ResourceCache *resource_cache);
static int init_menu_scene(Menu *menu, ResourceCache *resource_cache);

static int init_clock(Text **clock, ResourceCache *resource_cache)
{
	Texture *clock_texture =
	    texture_cache_get(resource_cache->textures, "white_font");

	if (!(init_text_obj(clock, clock_texture, resource_cache))) {
		printf("Failed to initialize clock.\n");
		return 0;
	}

	(*clock)->pos = (Vector2D){-0.1f, 0.1f};
	(*clock)->glyph_size = (Vector2D){clock_texture->size.x / 13.0f,
					  clock_texture->size.y / 1.0f};
	(*clock)->glyph_texture_size = (Vector2D){1.0f / 13.0f, 1.0f};

	// allocate mem for time string
	(*clock)->current_text = malloc(sizeof(char) * 8);
	if ((*clock)->current_text == NULL) {
		printf(
		    "Failed to allocate memory for the menu clock string.\n");
		return 0;
	}

	// the clock always consists of 6 sprites
	(*clock)->sprite_count = 6;

	// initialize current time and set vertices
	get_current_time((*clock)->current_text);
	update_text((*clock), (*clock)->current_text, 6);

	return 1;
}

static int init_menu_scene(Menu *menu, ResourceCache *resource_cache)
{
	menu->sprites = malloc(sizeof(MenuSprites));
	if (menu->sprites == NULL) {
		printf("Failed to allocate memory for menu sprites.\n");
		return 0;
	}

	SceneSprite sprites[] = {
	    (SceneSprite){.sprite_loc = &menu->sprites->lain,
			  .sprite = (Sprite){.pos = {-50.0f, 0.0f},
					     .size = {50.0f, 50.0f},
					     .texture_index = 0,
					     .texture_size = {1.0f, 1.0f},
					     .texture_offset = {0.0f, 0.0f},
					     .z_index = 1}},
	    (SceneSprite){.sprite_loc = &menu->sprites->main_ui,
			  .sprite =
			      (Sprite){
				  .pos = {-20.0f, 0.0f},
				  .size = {150.0f, 100.0f},
				  .texture_index = 1,
				  .texture_size = {1.0f, 1.0f},
				  .texture_offset = {0.0f, 0.0f},
				  .z_index = 0,
			      }},
	    (SceneSprite){.sprite_loc = &menu->sprites->main_ui_bar,
			  .sprite = (Sprite){
			      .pos = {0.5f, -0.25f},
			      .size = {100.0f, 10.0f},
			      .texture_index = 2,
			      .texture_size = {1.0f, 1.0f},
			      .texture_offset = {0.0f, 0.0f},
			      .on_click = toggle_main_window_expanded,
			      .z_index = 1,
			  }}};

	unsigned int sprite_count = sizeof(sprites) / sizeof(sprites[0]);
	for (int i = 0; i < sprite_count; i++) {
		if (!make_sprite(sprites[i].sprite_loc, sprites[i].sprite)) {
			return 0;
		}
	}

	SceneTextureSlot textures[] = {
	    make_texture_slot(
		0, texture_cache_get(resource_cache->textures, "lain")),

	    make_texture_slot(1, texture_cache_get(resource_cache->textures,
						   "main_ui_closed")),

	    make_texture_slot(
		2, texture_cache_get(resource_cache->textures, "main_ui_bar"))};

	unsigned int texture_count = sizeof(textures) / sizeof(textures[0]);

	if (!(init_scene(&menu->scene, sprites, sprite_count, textures,
			 texture_count, resource_cache))) {
		printf("Failed to initialize menu scene.\n");
		return 0;
	};

	return 1;
}

int init_menu(ResourceCache *resource_cache, Menu **menu)
{
	// allocate mem for the menu struct
	*menu = malloc(sizeof(Menu));

	if (*menu == NULL) {
		printf("Failed to allocate memory for the menu.\n");
		return 0;
	}

	// load scene
	if (!(init_menu_scene(*menu, resource_cache))) {
		printf("Failed to initialize menu scene.\n");
		return 0;
	}

	// clock
	if (!(init_clock(&(*menu)->clock, resource_cache))) {
		return 0;
	}

	(*menu)->expanded = false;

	return 1;
}

static void get_current_time(unsigned char *timestr)
{
	time_t t;
	struct tm *tmp;

	time(&t);

	tmp = localtime(&t);

	strftime((char *)timestr, sizeof(char) * 8, "%p%I:%M", tmp);
}

void update_menu(Menu *menu)
{
	unsigned char current_time[8];
	get_current_time(current_time);

	if (text_obj_needs_update(menu->clock, current_time)) {
		update_text(menu->clock, current_time, 6);
	}
}

void draw_menu(Menu *menu, GLFWwindow *window)
{
	draw_scene(menu->scene, window);
	draw_text(menu->clock, window);
}
