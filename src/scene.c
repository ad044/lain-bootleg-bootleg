#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"

SceneTextureSlot make_texture_slot(int slot_id, Texture *texture)
{
	return (SceneTextureSlot){slot_id, texture};
}

static void init_scene_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO);

static void init_scene_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO)
{
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	glGenBuffers(1, IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)0);
	// texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(2 * sizeof(GLfloat)));

	// texture id
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(4 * sizeof(GLfloat)));
}

int load_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       SceneTextureSlot *textures, unsigned int texture_count,
	       ResourceCache *resource_cache)
{
	*scene = malloc(sizeof(Scene));
	if (*scene == NULL) {
		printf("Failed to allocate memory for scene.\n");
		return 0;
	}

	// sort by z index before passing down the pipeline
	depth_sort(sprites, sprite_count);
	// initialize vao, vbo, ibo
	init_scene_buffers(&(*scene)->VAO, &(*scene)->VBO, &(*scene)->IBO);
	// fill buffers with sprite vertices
	update_sprite_vertices(sprites, sprite_count,
			       get_sprite_index_count(sprite_count));

	// copy sprites over to scene definition
	(*scene)->sprites = malloc(sizeof(Sprite) * sprite_count);
	if ((*scene)->sprites == NULL) {
		printf("Failed to allocate memory for scene sprites.\n");
		return 0;
	}
	memcpy((*scene)->sprites, sprites, sizeof(Sprite) * sprite_count);

	// texture stuff
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
		(*scene)->textures[i] = textures[i];
		(*scene)->samplers[i] = i;
	}

	(*scene)->texture_count = texture_count;
	(*scene)->sprite_count = sprite_count;
	(*scene)->shader = resource_cache->shaders[SCENE_SHADER];

	return 1;
}

void update_scene(Scene *scene)
{
	glBindVertexArray(scene->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, scene->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->IBO);

	update_sprite_vertices(scene->sprites, scene->sprite_count,
			       get_sprite_index_count(scene->sprite_count));
}

void draw_scene(Scene *scene)
{
	// bind appropriate textures
	for (int i = 0; i < scene->texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 + scene->textures[i].slot_id);
		glBindTexture(GL_TEXTURE_2D, scene->textures[i].texture->id);
	}

	// bind shader and set texture samplers
	glUseProgram(scene->shader);
	shader_program_set_texture_samplers(scene->shader, scene->samplers,
					    scene->texture_count);

	// bind vao
	glBindVertexArray(scene->VAO);

	// draw
	glDrawElements(GL_TRIANGLES,
		       get_sprite_index_count(scene->sprite_count),
		       GL_UNSIGNED_INT, 0);
}
