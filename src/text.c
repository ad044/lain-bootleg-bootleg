#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void fonts_init(Font *fonts, Texture *textures)
{
	Font red_font = (Font){.letter_spacing = 10.0f,
			       .glyph_count = 11,
			       .glyph_texture_size = {1.0f / 11.0f, 1.0f},
			       .glyph_order = red_glyphs_order,
			       .texture = &textures[RED_FONT_TEXTURE]};

	Font white_font = (Font){.letter_spacing = 11.4f,
				 .glyph_count = 13,
				 .glyph_texture_size = {1.0f / 13.0f, 1.0f},
				 .glyph_order = white_glyphs_order,
				 .texture = &textures[WHITE_FONT_TEXTURE]};

	fonts[RED_FONT] = red_font;
	fonts[WHITE_FONT] = white_font;
}

void update_text(Text *text_obj, char *new_text)
{
	if (strcmp(text_obj->current_text, new_text) == 0) {
		return;
	}

	int text_len = strlen(new_text);
	if (text_obj->left_aligned) {
		float x_pad = ((text_len - 1) * text_obj->glyph_size.x);

		text_obj->pos.x = text_obj->origin_pos.x - x_pad;
	}

	memcpy(text_obj->current_text, new_text, sizeof(char) * text_len);
}

Sprite get_glyph(Text *text_obj, char letter, unsigned int nth)
{
	Sprite glyph = (Sprite){
	    .size = text_obj->glyph_size,
	    .pos = {text_obj->pos.x + nth * text_obj->font->letter_spacing,
		    text_obj->pos.y},
	    .texture_size = text_obj->font->glyph_texture_size,
	    .frame_index = text_obj->font->glyph_order[(int)letter],
	    .texture_index = text_obj->texture_index};

	return glyph;
}
