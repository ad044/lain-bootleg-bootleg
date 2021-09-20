#include <stdio.h>
#include <stdlib.h>

#include "index_buffer.h"
#include "menu.h"
#include "shader.h"
#include "texture.h"
#include "timeutil.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

int init_menu(ResourceCache *resource_cache, Menu **menu)
{
	// allocate mem for the menu struct
	*menu = malloc(sizeof(Menu));
	if (*menu == NULL) {
		printf("Failed to allocate memory for the menu.\n");
		return 0;
	}

	// texture slot definitions
	TextureSlot texture_slots[] = {
	    (TextureSlot){.texture_id = texture_cache_get(
			      resource_cache->textures, "lain"),
			  .texture_index = 0,
			  .name = "lain_ui"},
	    (TextureSlot){.texture_id = texture_cache_get(
			      resource_cache->textures, "main_ui_closed"),
			  .texture_index = 1,
			  .name = "main_ui"}};

	unsigned int texture_slot_count =
	    sizeof(texture_slots) / sizeof(TextureSlot);

	// sprite definitions
	Sprite sprites[] = {
	    // lain
	    (Sprite){.pos = {-1.0f, -1.0f},
		     .size = {1.0f, 2.0f},
		     .texture_index = 0},
	    // main ui
	    (Sprite){.pos = {-0.0f, -1.0f},
		     .size = {1.0f, 2.0f},
		     .texture_index = 1},
	};

	unsigned int sprite_count = sizeof(sprites) / sizeof(Sprite);

	// which shader to use
	ShaderProgram shader =
	    shader_cache_get(resource_cache->shaders, "sprite");

	if (!(init_scene(&(*menu)->scene, sprites, sprite_count, texture_slots,
			 texture_slot_count, shader))) {
		printf("Failed to initialize menu.\n");
		return 0;
	};

	return 1;
}

void update_menu(Menu *menu) { get_current_time(menu->current_time); }
