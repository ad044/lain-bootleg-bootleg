#pragma once
#include <stdint.h>

#include "util.h"

#define MAX_TEXTURE_COUNT 64

enum {
	BEAR_ICON_ACTIVE,
	BEAR_ICON_INACTIVE,
	UI_LAIN_BEAR,
	MAIN_UI,
	MAIN_UI_BAR_ACTIVE,
	MAIN_UI_BAR_INACTIVE,
	DRESSUP_BUTTON_INACTIVE,
	THEATER_PREVIEWS,
	THEATER_BUTTON_ACTIVE,
	THEATER_BUTTON_INACTIVE,
	SCREWDRIVER_ICON_INACTIVE,
	PAW_ICON,
	SCORE_PREVIEW,
	RED_FONT_TEXTURE,
	WHITE_FONT_TEXTURE,
	KUMA_SHOOT_BG,
	KUMA_SHOOT_BUSH_OVERLAY,
};

typedef struct {
	GLuint id;
	int nr_channels;
	Vector2D size;
	char *name;
} Texture;

Texture make_texture(char *image_path, char *name);
void textures_init(Texture *textures);
