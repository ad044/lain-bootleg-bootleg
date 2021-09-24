#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "shader.h"

int init_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       Texture **textures, unsigned int texture_count,
	       ShaderProgram shader)
{
	*scene = malloc(sizeof(Scene));
	if (*scene == NULL) {
		printf("Failed to allocate memory for scene.\n");
		return 0;
	}

	init_scene_buffers(&(*scene)->VAO, &(*scene)->VBO, &(*scene)->IBO,
			   sprites, sprite_count);

	(*scene)->sprites = malloc(sizeof(Sprite) * sprite_count);
	if ((*scene)->sprites == NULL) {
		printf("Failed to allocate memory for scene sprites.\n");
		return 0;
	}
	memcpy(sprites, (*scene)->sprites, sizeof(Sprite) * sprite_count);

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

	int sampler_count = 0;
	for (int i = 0; i < texture_count; i++) {
		(*scene)->textures[i] = *textures[i];
		(*scene)->samplers[i] = i;
		sampler_count++;
	}

	(*scene)->texture_count = texture_count;
	(*scene)->shader = shader;
	(*scene)->sprite_count = sprite_count;
	(*scene)->sampler_count = sampler_count;

	return 1;
}

void init_scene_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO, Sprite *sprites,
			unsigned int sprite_count)
{
	unsigned int vertex_buffer_size = sprite_count * 4 * 5;
	unsigned int index_count = sprite_count * 6;
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	GLfloat vertices[vertex_buffer_size];
	load_sprite_vertex_data(vertices, sprites, sprite_count);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	glGenBuffers(1, IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
	GLuint indices[index_count];
	generate_sprite_indices(indices, index_count);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);

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

void draw_scene(Scene *scene)
{
	// bind appropriate textures
	for (int i = 0; i < scene->texture_count; i++) {
		glBindTextureUnit(i, scene->textures[i].id);
	}

	// bind shader and set texture samplers
	glUseProgram(scene->shader);
	shader_program_set_samplers(scene->shader, "u_Textures",
				    scene->samplers, scene->sampler_count);

	// bind buffers
	glBindVertexArray(scene->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, scene->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->IBO);

	// update buffers
	/* GLint index_count = get_sprite_index_count(scene->sprite_count); */
	/* fill_buffer_data(scene->sprites, scene->sprite_count, index_count); */

	// draw
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}
