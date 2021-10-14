#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quad.h"
#include "resource_cache.h"
#include "scene.h"
#include "sprite.h"
#include "text.h"

static const uint16_t white_glyphs_order[256] = {
    ['A'] = 0,	['P'] = 1,  ['0'] = 2, ['1'] = 3, ['2'] = 4,
    ['3'] = 5,	['4'] = 6,  ['5'] = 7, ['6'] = 8, ['7'] = 9,
    ['8'] = 10, ['9'] = 11, [':'] = 12};

#define MAX_TEXT_LEN 15

int init_text_obj(Text *text_obj, TextDefinition text_obj_def, Texture *texture)
{
	text_obj->glyph_size =
	    make_vec2d(texture->size.x / text_obj_def.texture_glyph_count,
		      texture->size.y / 1.0f);

	text_obj->glyph_texture_size =
	    make_vec2d(1.0f / text_obj_def.texture_glyph_count, 1.0f);

	text_obj->h_padding = text_obj->glyph_size.x / 3.0f;

	text_obj->current_text = malloc(sizeof(char) * MAX_TEXT_LEN);
	if (text_obj->current_text == NULL) {
		printf("Failed to allocate memory for text object string.\n");
		return 0;
	}

	return 1;
}

Sprite get_glyph(Text *text_obj, char letter, unsigned int nth)
{
	const Vector2D pos = make_vec2d(
	    text_obj->pos.x + nth * text_obj->h_padding, text_obj->pos.y);

	return (Sprite){.size = text_obj->glyph_size,
			.texture_size = text_obj->glyph_texture_size,
			.current_frame = white_glyphs_order[letter],
			.texture_index = text_obj->texture_index};
}

void set_text(Text *text_obj, char *text) { text_obj->current_text = text; }