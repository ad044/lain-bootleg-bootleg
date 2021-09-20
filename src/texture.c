#include <GL/glew.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "stb_image.h"
#include "texture.h"

static int texture_slot_compare(const void *a, const void *b, void *udata);
static uint64_t texture_slot_hash(const void *item, uint64_t seed0,
				  uint64_t seed1);

static uint64_t texture_hash(const void *item, uint64_t seed0, uint64_t seed1);
static int texture_compare(const void *a, const void *b, void *udata);
static bool texture_slot_bind(const void *item, void *udata);

int init_texture(Texture *texture, char *image_path)
{
	stbi_set_flip_vertically_on_load(true);

	if (!(access(image_path, F_OK) == 0)) {
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

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE, data);
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

GLuint texture_cache_get(TextureCache *cache, char *texture_name)
{
	Texture *cached_texture =
	    hashmap_get(cache, &(Texture){.name = texture_name});
	if (cached_texture == NULL) {
		printf("Failed to load cached texture %s.\n", texture_name);
		return 0;
	}
	return cached_texture->id;
}

Texture *make_texture(char *image_path, char *name)
{
	Texture *texture = malloc(sizeof(Texture));
	if (!(init_texture(texture, image_path))) {
		printf("Failed to initialize texture %s", image_path);
		exit(1);
	};

	texture->name = name;

	return texture;
}

void texture_cache_put(TextureCache *cache, Texture *texture)
{
	hashmap_set(cache, texture);
}

static int texture_slot_compare(const void *a, const void *b, void *udata)
{
	const TextureSlot *ua = a;
	const TextureSlot *ub = b;
	return strcmp(ua->name, ub->name);
}

static uint64_t texture_slot_hash(const void *item, uint64_t seed0,
				  uint64_t seed1)
{
	const TextureSlot *texture_slot = item;
	return hashmap_sip(texture_slot->name, strlen(texture_slot->name),
			   seed0, seed1);
}

void texture_slot_map_put(TextureSlotMap *map, TextureSlot slot)
{
	hashmap_set(map, &slot);
}

int texture_slot_map_init(TextureSlotMap **map)
{
	*map = hashmap_new(sizeof(TextureSlot), 0, 0, 0, texture_slot_hash,
			   texture_slot_compare, NULL);
	if (*map == NULL) {
		printf("Failed to allocate memory for texture slot map.\n");
		return 0;
	}

	return 1;
}

static bool texture_slot_bind(const void *item, void *udata)
{
	const TextureSlot *texture_slot = item;
	glBindTextureUnit(texture_slot->texture_index,
			  texture_slot->texture_id);
	return true;
}

void texture_slot_map_bind(TextureSlotMap *map)
{
	hashmap_scan(map, texture_slot_bind, NULL);
}

TextureSlot texture_slot_map_get(TextureSlotMap *map, char *texture_slot_name)
{
	TextureSlot *slot =
	    hashmap_get(map, &(Texture){.name = texture_slot_name});
	if (slot == NULL) {
		printf("Failed to load cached texture %s.\n",
		       texture_slot_name);
		exit(1);
	}
	return *slot;
}

// perhaps this one can recursively search the texture directory
// and load the textures into cache with the keys being filename - extension
void preload_textures(TextureCache *cache)
{
	texture_cache_put(cache, make_texture("assets/ui/main_ui_closed.png",
					      "main_ui_closed"));

	texture_cache_put(cache, make_texture("assets/579", "lain"));

	texture_cache_put(
	    cache, make_texture("assets/ui/white_font.png", "white_font"));
}
