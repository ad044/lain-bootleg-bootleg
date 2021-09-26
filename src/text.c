#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "scene.h"
#include "text.h"

#define WHITE_FONT_GLYPH_COUNT 13.0

static _Bool text_needs_update(Text *text_obj, unsigned char *text);
static void update_text(Text *text_obj, unsigned char *text);
static void text_to_render_data(RenderData *render_data, Text *sprite);

const static uint16_t white_glyph_offsets[256] = {
    ['A'] = 0,	 ['P'] = 32,  ['0'] = 64,  ['1'] = 96,	['2'] = 128,
    ['3'] = 160, ['4'] = 192, ['5'] = 224, ['6'] = 256, ['7'] = 288,
    ['8'] = 320, ['9'] = 352, [':'] = 384};

int init_text_obj(Text **text_obj, ShaderProgram shader, Texture *texture)
{

	*text_obj = malloc(sizeof(Text));
	if (*text_obj == NULL) {
		printf("Failed to allocate memory for text renderer.\n");
		return 0;
	}

	(*text_obj)->current_text = malloc(sizeof(char) * 10);
	if ((*text_obj)->current_text == NULL) {
		printf("Failed to allocate memory for text in sprite.\n");
		return 0;
	}

	init_sprite_buffers(&(*text_obj)->VAO, &(*text_obj)->VBO,
			    &(*text_obj)->IBO);

	(*text_obj)->shader = shader;
	(*text_obj)->texture = texture;

	return 1;
}

static _Bool text_needs_update(Text *text_obj, unsigned char *text)
{
	return (strcmp((char *)text_obj->current_text, (char *)text) != 0);
}

static void update_text(Text *text_obj, unsigned char *text)
{
	Sprite sprites[6];

	unsigned char sprite_count;
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

	fill_buffer_data(sprites, sprite_count,
			 get_sprite_index_count(sprite_count));
}

void update_text_obj(Text *text_obj, unsigned char *text)
{
	if (text_needs_update(text_obj, text)) {
		update_text(text_obj, text);
	}
}

static void text_to_render_data(RenderData *render_data, Text *text_obj)
{
	render_data->VAO = text_obj->VAO;
	render_data->texture_count = 1;
	render_data->textures = text_obj->texture;
	render_data->shader = text_obj->shader;
	render_data->index_count =
	    get_sprite_index_count(text_obj->sprite_count);
	memcpy(render_data->samplers, &(GLint){0}, sizeof(GLint));
}

void draw_text(Text *text_obj)
{
	RenderData render_data;

	text_to_render_data(&render_data, text_obj);

	render(render_data);
}
