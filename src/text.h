#pragma once

#include "texture.h"
#include "sprite.h"
#include "texture.h"

#define FONT_COUNT 2

enum { RED_FONT, WHITE_FONT };

typedef struct {
	float letter_spacing;
	unsigned int glyph_count;
	Vector2D glyph_texture_size;
	uint16_t *glyph_order;
	Texture *texture;
} Font;

typedef struct {
	Vector2D pos;
	Vector2D glyph_size;
	unsigned int texture_index;
	Font *font;
	char *current_text;
} Text;

void fonts_init(Font **fonts, TextureCache *textures);
Sprite get_glyph(Text *text_obj, char letter, unsigned int nth);
