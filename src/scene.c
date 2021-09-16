#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "timeutil.h"

Scene *get_menu(TextureCache *texture_cache)
{
	Sprite sprites[] = {
	    make_sprite(texture_cache, "main_ui_closed", "main_ui_closed",
			(vec2){1.0f, 0.0f}, (vec2){0.8f, 0.8f}),
	    make_sprite(texture_cache, "lain", "lain", (vec2){0.0f, 0.0f},
			(vec2){0.8f, 0.8f})};

	int sprite_count = sizeof(sprites) / sizeof(sprites[0]);

	Scene *scene = malloc(sizeof(Scene) + sizeof(Sprite) * sprite_count);
	if (scene == NULL) {
		printf("Failed to allocate memory for the scene.\n");
		exit(1);
	}

	scene->sprites = malloc(sizeof(Sprite) * sprite_count);
	if (scene->sprites == NULL) {
		printf("Failed to allocate memory for scene sprites.\n");
		exit(1);
	}

	// todo
	// memcpy for some reason didnt work perhaps i used it incorrectly, this
	// will do for now
	for (int i = 0; i < sprite_count; i++) {
		scene->sprites[i] = sprites[i];
	}
	scene->sprite_count = sprite_count;

	return scene;
}

void draw_scene(Scene *scene, ShaderCache *shaders, unsigned int VAO)
{
	for (int i = 0; i < scene->sprite_count; i++) {
		draw_sprite(&scene->sprites[i],
			    shader_cache_get(shaders, "sprite"), VAO);
	}
}
