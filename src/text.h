#pragma once

#include <stdint.h>

#include "sprite.h"

#define MAX_FONT_COUNT 2
#define MAX_TEXT_LENGTH 20

enum { RED_FONT, WHITE_FONT };

typedef struct {
	float letter_spacing;
	unsigned int glyph_count;
	Vector2D glyph_texture_size;
	const uint16_t *glyph_order;
	Texture *texture;
} Font;

typedef struct {
	Vector2D pos;
	Vector2D origin_pos; // keeps track of initially passed position.
	_Bool visible;
	// text rendered with left-aligned set to true expands to the left as more
	// letters get added. i dont know a better name for it.
	_Bool left_aligned;
	Vector2D glyph_size;
	unsigned int texture_index;
	Font *font;
	char current_text[MAX_TEXT_LENGTH];
} Text;

void fonts_init(Font *fonts, Texture *textures);
void update_text(Text *text_obj, char *new_text);
Sprite get_glyph(Text *text_obj, char letter, unsigned int nth);
