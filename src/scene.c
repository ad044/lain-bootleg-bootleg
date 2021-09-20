#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "index_buffer.h"
#include "scene.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

int init_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       TextureSlot *texture_slots, unsigned int texture_slot_count,
	       ShaderProgram shader)
{
	*scene = malloc(sizeof(Scene));
	if (*scene == NULL) {
		printf("Failed to allocate memory for scene.\n");
		return 0;
	}

	// texture stuff
	if (!(texture_slot_map_init(&(*scene)->texture_slot_map))) {
		printf("Failed to initialize texture slot map for menu.\n");
		return 0;
	}

	for (int i = 0; i < texture_slot_count; i++) {
		texture_slot_map_put((*scene)->texture_slot_map,
				     texture_slots[i]);
	};

	GLint samplers[texture_slot_count];
	for (int i = 0; i < texture_slot_count; i++) {
		samplers[i] = i;
	};

	shader_bind(shader);
	shader_program_set_samplers(shader, "u_Textures", samplers,
				    sizeof(samplers) / sizeof(GLint));

	// vao stuff
	unsigned int index_count = sprite_count * 6;
	unsigned int vertex_buffer_size = sprite_count * 4 * 5;

	GLfloat vertices[vertex_buffer_size];
	put_sprites_into_buffer(vertices, sprites, sprite_count);

	GLuint indices[index_count];
	generate_quad_indices(indices, index_count);

	GLuint VAO;
	init_vertex_array(&VAO, vertices, sizeof(vertices), indices,
			  sizeof(indices));

	(*scene)->VAO = VAO;
	(*scene)->index_count = index_count;
	(*scene)->current_shader = shader;

	return 1;
}

void draw_scene(Scene *scene)
{
	shader_bind(scene->current_shader);
	texture_slot_map_bind(scene->texture_slot_map);
	bind_vertex_array(scene->VAO);

	glDrawElements(GL_TRIANGLES, scene->index_count, GL_UNSIGNED_INT, 0);
}