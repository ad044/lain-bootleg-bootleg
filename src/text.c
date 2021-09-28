#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource_cache.h"
#include "scene.h"
#include "sprite.h"
#include "text.h"

#define WHITE_FONT_GLYPH_COUNT 13.0

const static uint16_t white_glyph_offsets[256] = {
    ['A'] = 0,	 ['P'] = 32,  ['0'] = 64,  ['1'] = 96,	['2'] = 128,
    ['3'] = 160, ['4'] = 192, ['5'] = 224, ['6'] = 256, ['7'] = 288,
    ['8'] = 320, ['9'] = 352, [':'] = 384};

static _Bool text_needs_update(Text *text_obj, unsigned char *text);
static void init_text_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO);

static void init_text_obj_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO)
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
}

int init_text_obj(Text **text_obj, Texture *texture,
		  ResourceCache *resource_cache)
{
	*text_obj = malloc(sizeof(Text));
	if (*text_obj == NULL) {
		printf("Failed to allocate memory for text renderer.\n");
		return 0;
	}

	(*text_obj)->current_text = malloc(sizeof(char) * 10);
	if ((*text_obj)->current_text == NULL) {
		printf("Failed to allocate memory for text inside the text "
		       "sprite.\n");
		return 0;
	}

	init_text_obj_buffers(&(*text_obj)->VAO, &(*text_obj)->VBO,
			      &(*text_obj)->IBO);

	(*text_obj)->shader = shader_cache_get(resource_cache->shaders, "text");
	(*text_obj)->texture = texture;

	return 1;
}

static _Bool text_needs_update(Text *text_obj, unsigned char *text)
{
	return (strcmp((char *)text_obj->current_text, (char *)text) != 0);
}

void update_text_vertices(Text *text_obj, unsigned char *text)
{
	Sprite sprites[6];

	unsigned int sprite_count = 0;
	unsigned char *p = text;

	float x_offset_carry = 0;

	for (int i = 0; *p != '\0'; p++, i++) {
		sprite_count++;
		sprites[i] = (Sprite){
		    // todo hardcoded values bad
		    .pos = {text_obj->pos.x + x_offset_carry + i * 0.15f,
			    text_obj->pos.y},
		    .size = text_obj->size,
		    // todo i have no idea why this works
		    .texture_size = WHITE_FONT_GLYPH_COUNT,
		    .texture_offset =
			white_glyph_offsets[*p] / text_obj->texture->width};

		if (*p == 'A' || *p == 'P') {
			p++;
			x_offset_carry += 0.2;
		}
	}

	text_obj->sprite_count = sprite_count;

	glBindVertexArray(text_obj->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, text_obj->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, text_obj->IBO);

	fill_sprite_buffer_data(sprites, text_obj->sprite_count,
				get_sprite_index_count(sprite_count));
}

void update_text(Text *text_obj, unsigned char *text)
{
	if (!text_needs_update(text_obj, text)) {
		update_text_vertices(text_obj, text);
	}
}

void draw_text(Text *text_obj)
{
	// bind shader and set texture
	glUseProgram(text_obj->shader);
	shader_program_set_texture(text_obj->shader, text_obj->texture->id);

	// bind vao
	glBindVertexArray(text_obj->VAO);

	// draw
	glDrawElements(GL_TRIANGLES,
		       get_sprite_index_count(text_obj->sprite_count),
		       GL_UNSIGNED_INT, 0);
}
