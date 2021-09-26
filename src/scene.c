#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "scene.h"
#include "shader.h"

static void scene_to_render_data(RenderData *render_data, Scene *scene);

int init_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       Texture **textures, unsigned int texture_count,
	       ShaderProgram shader)
{
	*scene = malloc(sizeof(Scene));
	if (*scene == NULL) {
		printf("Failed to allocate memory for scene.\n");
		return 0;
	}

	// sort by z index before passing down the pipeline
	depth_sort(sprites, sprite_count);

	init_sprite_buffers(&(*scene)->VAO, &(*scene)->VBO, &(*scene)->IBO);
	// fill buffers with sprites initially
	fill_buffer_data(sprites, sprite_count,
			 get_sprite_index_count(sprite_count));

	(*scene)->sprites = malloc(sizeof(Sprite) * sprite_count);
	if ((*scene)->sprites == NULL) {
		printf("Failed to allocate memory for scene sprites.\n");
		return 0;
	}
	memcpy((*scene)->sprites, sprites, sizeof(Sprite) * sprite_count);

	(*scene)->textures = malloc(sizeof(Texture) * texture_count);
	if ((*scene)->textures == NULL) {
		printf("Failed to allocate memory for scene textures.\n");
		return 0;
	}

	(*scene)->samplers = malloc(sizeof(GLint) * texture_count);
	if ((*scene)->samplers == NULL) {
		printf("Failed to allocate memory for samplers.\n");
		return 0;
	}

	for (int i = 0; i < texture_count; i++) {
		(*scene)->textures[i] = *textures[i];
		(*scene)->samplers[i] = i;
	}

	(*scene)->texture_count = texture_count;
	(*scene)->sprite_count = sprite_count;
	(*scene)->shader = shader;

	return 1;
}

static void scene_to_render_data(RenderData *render_data, Scene *scene)
{
	render_data->VAO = scene->VAO;
	render_data->texture_count = scene->texture_count;
	render_data->textures = scene->textures;
	render_data->shader = scene->shader;
	render_data->index_count = get_sprite_index_count(scene->sprite_count);
	render_data->samplers = scene->samplers;
}

void draw_scene(Scene *scene)
{
	RenderData render_data;

	scene_to_render_data(&render_data, scene);

	render(render_data);
}
