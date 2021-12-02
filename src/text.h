#pragma once

#include <stdint.h>

#include "sprite.h"

#define MAX_FONT_COUNT 2
#define MAX_TEXT_LENGTH 20

enum { FONT_RED, FONT_WHITE };

typedef struct {
	Texture *texture;
	float letter_spacing;
	Vector2D glyph_texture_size;
	const uint16_t *glyph_order;
} Font;

typedef struct {
	Vector2D pos;
	Vector2D origin_pos; // keeps track of initially passed position.
	_Bool visible;
	_Bool left_aligned; // text rendered with left-aligned set to true
			    // expands to the left as  more letters get added.
			    // i dont know a better name for it.
	Vector2D glyph_size;
	unsigned int texture_index;
	Font *font;
	char current_text[MAX_TEXT_LENGTH];
} Text;

struct resources;

void fonts_init(struct resources *resources);
void update_text(Text *text_obj, char *new_text);
GLfloat *get_glyph_vertices(GLfloat *buffer, Text *text_obj, char letter,
			    uint8_t nth);
