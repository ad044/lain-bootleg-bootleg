#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resources.h"
#include "scene.h"
#include "sprite.h"
#include "text.h"
#include "vector2d.h"

static const uint16_t white_glyphs_order[256] = {
    ['A'] = 0,	['P'] = 1,  ['0'] = 2, ['1'] = 3, ['2'] = 4,
    ['3'] = 5,	['4'] = 6,  ['5'] = 7, ['6'] = 8, ['7'] = 9,
    ['8'] = 10, ['9'] = 11, [':'] = 12};

static const uint16_t red_glyphs_order[256] = {
    ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4, ['5'] = 5,
    ['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9, ['-'] = 10};

void fonts_init(Resources *resources)
{
	resources->fonts[FONT_RED] =
	    (Font){.letter_spacing = 10.0f,
		   .glyph_texture_size = {1.0f / 11.0f, 1.0f},
		   .glyph_order = red_glyphs_order,
		   .texture = texture_get(resources, RED_FONT_SPRITESHEET)};

	resources->fonts[FONT_WHITE] =
	    (Font){.letter_spacing = 11.4f,
		   .glyph_texture_size = {1.0f / 13.0f, 1.0f},
		   .glyph_order = white_glyphs_order,
		   .texture = texture_get(resources, WHITE_FONT_SPRITESHEET)};
}

void update_text(Text *text_obj, char *new_text)
{
	int text_len = strlen(new_text);
	if (text_obj->left_aligned) {
		float x_pad = ((text_len - 1) * text_obj->glyph_size.x);

		text_obj->pos.x = text_obj->origin_pos.x - x_pad;
	}

	memset(text_obj->current_text, 0, 20);
	memcpy(text_obj->current_text, new_text, sizeof(char) * text_len);
}

GLfloat *get_glyph_vertices(GLfloat *buffer, Text *text_obj, char letter,
			    uint8_t nth)
{
	GLfloat pos_x = text_obj->pos.x + nth * text_obj->font->letter_spacing;
	GLfloat pos_y = text_obj->pos.y;

	GLfloat offset = text_obj->font->glyph_order[(int)letter];

	Vector2D glyph_size = text_obj->glyph_size;
	Vector2D glyph_texture_size = text_obj->font->glyph_texture_size;

	GLfloat vertices[] = {
	    // top right
	    pos_x + glyph_size.x,
	    pos_y,
	    offset * glyph_texture_size.x + glyph_texture_size.x,
	    glyph_texture_size.y,
	    text_obj->texture_index,

	    // bottom right
	    pos_x + glyph_size.x,
	    pos_y + glyph_size.y,
	    offset * glyph_texture_size.x + glyph_texture_size.x,
	    0.0f,
	    text_obj->texture_index,

	    // bottom left
	    pos_x,
	    pos_y + glyph_size.y,
	    offset * glyph_texture_size.x,
	    0.0f,
	    text_obj->texture_index,

	    // top left
	    pos_x,
	    pos_y,
	    offset * glyph_texture_size.x,
	    glyph_texture_size.y,
	    text_obj->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}
