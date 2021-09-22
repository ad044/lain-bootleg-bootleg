#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "index_buffer.h"
#include "scene.h"
#include "text_renderer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

#define WHITE_FONT_GLYPH_COUNT 13.0

const static uint16_t white_glyph_offsets[256] = {
    ['A'] = 0,	 ['P'] = 32,  ['0'] = 64,  ['1'] = 96,	['2'] = 128,
    ['3'] = 160, ['4'] = 192, ['5'] = 224, ['6'] = 256, ['7'] = 288,
    ['8'] = 320, ['9'] = 352, [':'] = 384};

void draw_clock(Texture *texture, char *time)
{
	glBindTexture(GL_TEXTURE_2D, texture->id);

	Sprite clock_quads[6];

	// period (AM/PM) separately since its assoced with a single char
	// whereas its 2 chars.
	clock_quads[0] = (Sprite){
	    .pos = {-0.25f, 0.0f},
	    .size = {0.5f, 0.5f},
	    .texture_size = WHITE_FONT_GLYPH_COUNT,
	    .texture_offset = white_glyph_offsets[time[0]] / texture->width};

	char *p = time;
	p += 2;
	for (int i = 1; *p != '\0'; p++, i++) {
		clock_quads[i] = (Sprite){
		    .pos = {i * 0.15f, 0.0f},
		    .size = {0.5f, 0.5f},
		    .texture_size = WHITE_FONT_GLYPH_COUNT,
		    .texture_offset = white_glyph_offsets[*p] / texture->width};
	}

	// sprites are always going to be 6 in this case
	// each quad having 6 indices.
	unsigned int index_count = 6 * 6;
	unsigned int vertex_buffer_size = 6 * 4 * 5;

	GLfloat vertices[vertex_buffer_size];
	put_sprites_into_buffer(vertices, clock_quads, 6);

	GLuint indices[index_count];
	generate_quad_indices(indices, index_count);

	GLuint VAO;
	init_vertex_array(&VAO, vertices, sizeof(vertices), indices,
			  sizeof(indices));

	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}
