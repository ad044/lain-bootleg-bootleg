#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "sprite.h"
#include "texture.h"

// could be abstracted into a general loader which takes in a scene name
// fetches resources, and then does the rest of the computations (which are generic)
Scene *get_initial_scene()
{
	Sprite sprites[] = {

	    make_sprite(make_texture("assets/ui/main_ui_closed.png"),
			"main_ui_closed", (vec2){1.0f, 0.0f},
			(vec2){0.8f, 0.8f}),

	    make_sprite(make_texture("assets/579"), "lain", (vec2){0.0f, 0.0f},
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

void draw_scene(Scene *scene, shader_cache *shaders, unsigned int VAO)
{
	for (int i = 0; i < scene->sprite_count; i++) {
		draw_sprite(&scene->sprites[i],
			    shader_cache_get(shaders, "sprite"), VAO);
	}
}
