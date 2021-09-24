#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "text_renderer.h"

#define WHITE_FONT_GLYPH_COUNT 13.0

const static uint16_t white_glyph_offsets[256] = {
    ['A'] = 0,	 ['P'] = 32,  ['0'] = 64,  ['1'] = 96,	['2'] = 128,
    ['3'] = 160, ['4'] = 192, ['5'] = 224, ['6'] = 256, ['7'] = 288,
    ['8'] = 320, ['9'] = 352, [':'] = 384};

int init_text_sprite(TextSprite **text_sprite, ShaderProgram shader,
		     Texture *texture)
{

	*text_sprite = malloc(sizeof(TextSprite));
	if (*text_sprite == NULL) {
		printf("Failed to allocate memory for text renderer.\n");
		return 0;
	}
	GLuint VBO, IBO;

	glGenVertexArrays(1, &(*text_sprite)->VAO);

	glBindVertexArray((*text_sprite)->VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


/* 	set_sprite_vertex_attrib_pointers(); */

	(*text_sprite)->shader = shader;
	(*text_sprite)->texture = texture;

	glBindVertexArray(0);

	return 1;
}

void draw_text(TextSprite *text_sprite, char *time)
{
	Sprite clock_quads[6];

	char *p = time;
	unsigned int sprite_count = 0;
	float x_offset_carry = 0;
	for (int i = 0; *p != '\0'; p++, i++) {
		sprite_count++;
		clock_quads[i] =
		    (Sprite){.pos = {x_offset_carry + i * 0.15f, 0.0f},
			     .size = {0.5f, 0.5f},
			     .texture_size = WHITE_FONT_GLYPH_COUNT,
			     .texture_offset = white_glyph_offsets[*p] /
					       text_sprite->texture->width};

		if (*p == 'A' || *p == 'P') {
			p++;
			x_offset_carry += 0.2;
		}
	}

	glBindVertexArray(text_sprite->VAO);

	unsigned int index_count = sprite_count * 6;
	unsigned int vertex_buffer_size = sprite_count * 4 * 5;

	/* GLfloat vertices[vertex_buffer_size]; */
	/* put_sprites_into_buffer(vertices, clock_quads, 6); */
	/* glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, */
	/* 	     GL_DYNAMIC_DRAW); */

	/* GLuint indices[index_count]; */
	/* generate_quad_indices(indices, index_count); */
	/* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, */
	/* 	     GL_DYNAMIC_DRAW); */

	/* glBindTextureUnit(0, text_sprite->texture->id); */

	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}
