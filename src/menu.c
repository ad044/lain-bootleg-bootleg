#include <stdlib.h>

#include "hashmap.h"
#include "menu.h"
#include "sprite.h"
#include "timeutil.h"

int init_menu(ResourceCache *resource_cache, Menu **menu)
{
	// allocate mem for the menu struct
	*menu = malloc(sizeof(Menu));
	if (*menu == NULL) {
		printf("Failed to allocate memory for the scene.\n");
		return 0;
	}

	// allocate mem for menu sprites
	if (!(sprite_map_init(&(*menu)->sprite_map))) {
		printf("Failed to initialize menu sprites.\n");
		return 0;
	}

	Sprite *lain = make_sprite(resource_cache, "lain", "lain");
	sprite_set_size(lain, (vec2){1.0f, 1.0f});
	sprite_set_pos(lain, (vec2){0.0f, 0.0f});

	Sprite *main_ui_closed =
	    make_sprite(resource_cache, "main_ui_closed", "main_ui_closed");
	sprite_set_size(main_ui_closed, (vec2){1.0f, 1.0f});
	sprite_set_pos(main_ui_closed, (vec2){0.0f, 0.0f});

	Sprite *sprites[] = {
	    lain,
	    main_ui_closed,
	};

	int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	// populate sprite map
	for (int i = 0; i < sprite_count; i++) {
		sprite_map_put((*menu)->sprite_map, sprites[i]);
	}

	return 1;
}

void update_menu(Menu *menu) { get_current_time(menu->current_time); }

void draw_menu(Menu *menu, GLuint *VAO)
{
	draw_spritemap(menu->sprite_map, VAO);
}
