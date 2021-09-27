#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "menu.h"
#include "resource_cache.h"
#include "shader.h"
#include "text.h"
#include "texture.h"
#include "window.h"

static void get_current_time(unsigned char *timestr);
static int init_menu_scene(Scene **scene, ResourceCache *resource_cache);
static int init_clock(Text **clock, ResourceCache *resource_cache);

static int init_menu_scene(Scene **scene, ResourceCache *resource_cache)
{
	// textures (will be bound in slots in order)
	Texture *textures[] = {
	    texture_cache_get(resource_cache->textures, "lain"),
	    texture_cache_get(resource_cache->textures, "main_ui_closed"),
	    texture_cache_get(resource_cache->textures, "main_ui_bar")};
	unsigned int texture_count = sizeof(textures) / sizeof(textures[0]);

	// sprite definitions
	Sprite sprites[] = {// lain
			    (Sprite){.pos = {-0.5f, -0.0f},
				     .size = {0.6f, 1.2f},
				     .texture_index = 0,
				     .z_index = 1},
			    // main ui
			    (Sprite){.pos = {-0.3f, -0.0f},
				     .size = {2.0f, 2.8f},
				     .texture_index = 1,
				     .z_index = 0},
			    // main ui bar
			    (Sprite){.pos = {0.5f, -0.3f},
				     .size = {1.6f, 0.2f},
				     .texture_index = 2,
				     .z_index = 2}};
	unsigned int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	ShaderProgram shader =
	    shader_cache_get(resource_cache->shaders, "sprite");

	if (!(init_scene(scene, sprites, sprite_count, textures, texture_count,
			 shader))) {
		printf("Failed to initialize menu scene.\n");
		return 0;
	};

	return 1;
}

static int init_clock(Text **clock, ResourceCache *resource_cache)
{
	Texture *clock_texture =
	    texture_cache_get(resource_cache->textures, "white_font");

	ShaderProgram clock_shader =
	    shader_cache_get(resource_cache->shaders, "sprite");

	if (!(init_text_obj(clock, clock_shader, clock_texture))) {
		printf("Failed to initialize clock.\n");
		return 0;
	}

	(*clock)->pos = (Vector2D){-0.1f, 0.1f};
	(*clock)->size = (Vector2D){0.4f, 0.4f};

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

	// allocate mem for time string
	(*menu)->current_time = malloc(sizeof(char) * 8);
	if ((*menu)->current_time == NULL) {
		printf(
		    "Failed to allocate memory for the menu clock string.\n");
		return 0;
	}

	/// scene definition
	if (!(init_menu_scene(&(*menu)->scene, resource_cache))) {
		return 0;
	}

	// clock
	if (!(init_clock(&(*menu)->clock, resource_cache))) {
		return 0;
	}

	return 1;
}

static void get_current_time(unsigned char *timestr)
{
	time_t t;
	struct tm *tmp;

	time(&t);

	tmp = localtime(&t);

	strftime(timestr, sizeof(char) * 8, "%p%I:%M", tmp);
}

void update_menu(Menu *menu)
{
	get_current_time(menu->current_time);
	update_text_obj(menu->clock, menu->current_time);
}

void draw_menu(ResourceCache *resource_cache, Menu *menu)
{
	draw_scene(menu->scene);
	draw_text(menu->clock);
}
