#include <GL/glew.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#else
#define access _access
#endif

#include "util.h"
#include "sprite.h"
#include "stb_image.h"
#include "texture.h"

static uint64_t texture_hash(const void *item, uint64_t seed0, uint64_t seed1);
static int texture_compare(const void *a, const void *b, void *udata);

int init_texture(Texture *texture, char *image_path)
{
	stbi_set_flip_vertically_on_load(true);

	if (!(access(image_path, 0) == 0)) {
		printf("Can't find texture %s.\n", image_path);
		return 0;
	}

	int width, height, nr_channels;
	unsigned char *data =
	    stbi_load(image_path, &width, &height, &nr_channels, 0);
	if (!data) {
		printf("Failed to load texture.\n");
		return 0;
	}

	texture->size = (Vector2D){width, height};
	texture->nr_channels = nr_channels;

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x,
		     texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
	return 1;
}

int texture_cache_init(TextureCache **cache)
{
	*cache = hashmap_new(sizeof(Texture), 0, 0, 0, texture_hash,
			     texture_compare, NULL);
	if (*cache == NULL) {
		printf("Failed to allocate memory for texture cache.\n");
		return 0;
	}

	// preload textures into cache
	preload_textures(*cache);

	return 1;
}

static uint64_t texture_hash(const void *item, uint64_t seed0, uint64_t seed1)
{
	const Texture *texture = item;
	return hashmap_sip(texture->name, strlen(texture->name), seed0, seed1);
}

static int texture_compare(const void *a, const void *b, void *udata)
{
	const Texture *ua = a;
	const Texture *ub = b;
	return strcmp(ua->name, ub->name);
}

Texture *texture_cache_get(TextureCache *cache, char *texture_name)
{
	Texture *cached_texture =
	    hashmap_get(cache, &(Texture){.name = texture_name});
	if (cached_texture == NULL) {
		printf("Failed to load cached texture %s.\n", texture_name);
		exit(1);
	}
	return cached_texture;
}

Texture make_texture(char *image_path, char *name)
{
	Texture texture;
	if (!(init_texture(&texture, image_path))) {
		printf("Failed to initialize texture %s", image_path);
		exit(1);
	};

	texture.name = name;

	return texture;
}

void texture_cache_put(TextureCache *cache, Texture texture)
{
	hashmap_set(cache, &texture);
}

// perhaps this one can recursively search the texture directory
// and load the textures into cache with the keys being filename - extension
void preload_textures(TextureCache *cache)
{
	texture_cache_put(cache,
			  make_texture("assets/ui/main_ui.png", "main_ui"));

	texture_cache_put(cache,
			  make_texture("assets/ui/ui_lain.png", "ui_lain"));

	texture_cache_put(
	    cache, make_texture("assets/ui/ui_lain_bear.png", "ui_lain_bear"));

	texture_cache_put(
	    cache, make_texture("assets/ui/white_font.png", "white_font"));

	texture_cache_put(cache,
			  make_texture("assets/ui/main_ui_bar_active.png",
				       "main_ui_bar_active"));

	texture_cache_put(cache,
			  make_texture("assets/ui/main_ui_bar_inactive.png",
				       "main_ui_bar_inactive"));

	texture_cache_put(cache,
			  make_texture("assets/ui/dressup_button_active.png",
				       "dressup_button_active"));

	texture_cache_put(cache,
			  make_texture("assets/ui/dressup_button_inactive.png",
				       "dressup_button_inactive"));

	texture_cache_put(cache,
			  make_texture("assets/ui/bear_icon_inactive.png",
				       "bear_icon_inactive"));

	texture_cache_put(
	    cache, make_texture("assets/ui/screwdriver_icon_inactive.png",
				"screwdriver_icon_inactive"));

	texture_cache_put(cache,
			  make_texture("assets/ui/screwdriver_icon_active.png",
				       "screwdriver_icon_active"));

	texture_cache_put(cache,
			  make_texture("assets/ui/theater_button_active.png",
				       "theater_button_active"));
	texture_cache_put(cache,
			  make_texture("assets/ui/theater_button_inactive.png",
				       "theater_button_inactive"));

	texture_cache_put(cache, make_texture("assets/ui/theater_previews.png",
					      "theater_previews"));

	texture_cache_put(cache,
			  make_texture("assets/ui/paw_icon.png", "paw_icon"));

	texture_cache_put(cache, make_texture("assets/ui/score_preview.png",
					      "score_preview"));

	texture_cache_put(cache, make_texture("assets/ui/red_font.png",
					      "red_font"));

	texture_cache_put(cache, make_texture("assets/kumashoot/bush_overlay.png",
					      "bush_overlay"));

	texture_cache_put(cache, make_texture("assets/backgrounds/kumashoot.png",
					      "kumashoot_bg"));
}
