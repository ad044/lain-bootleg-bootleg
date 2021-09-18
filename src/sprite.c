#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"
#include "sprite.h"
#include "texture.h"

static uint64_t sprite_hash(const void *item, uint64_t seed0, uint64_t seed1);
static int sprite_compare(const void *a, const void *b, void *udata);
static bool spritemap_draw_iter(const void *item, void *udata);

static uint64_t sprite_hash(const void *item, uint64_t seed0, uint64_t seed1)
{
	const Sprite *sprite = item;
	return hashmap_sip(sprite->name, strlen(sprite->name), seed0, seed1);
}

static int sprite_compare(const void *a, const void *b, void *udata)
{
	const Sprite *ua = a;
	const Sprite *ub = b;
	return strcmp(ua->name, ub->name);
}

Sprite *make_sprite(ResourceCache *resource_cache, char *texture_name,
		    char *sprite_name)
{
	Sprite *sprite = malloc(sizeof(Sprite));

	GLuint texture_id =
	    texture_cache_get(resource_cache->textures, texture_name);

	if (!texture_id) {
		printf("Missing texture %s.\n", texture_name);
		exit(1);
	}

	ShaderProgram shader_program =
	    shader_cache_get(resource_cache->shaders, "sprite");
	if (!shader_program) {
		printf("Missing shader for sprite.\n");
		exit(1);
	}

	sprite->name = sprite_name;
	sprite->texture_id = texture_id;
	sprite->shader_program = shader_program;

	return sprite;
}

int sprite_map_init(SpriteMap **cache)
{
	*cache = hashmap_new(sizeof(Sprite), 0, 0, 0, sprite_hash,
			     sprite_compare, NULL);
	if (*cache == NULL) {
		printf("Failed to allocate memory for sprite map.\n");
		return 0;
	}

	return 1;
}

Sprite sprite_map_get(SpriteMap *cache, char *sprite_name)
{
	Sprite *sprite = hashmap_get(cache, &(Sprite){.name = sprite_name});
	if (sprite == NULL) {
		printf("Failed to load sprite %s.\n", sprite_name);
		exit(1);
	}
	return *sprite;
}

void sprite_set_size(Sprite *sprite, vec2 size)
{
	memcpy(sprite->size, size, sizeof(vec2));
}

void sprite_set_pos(Sprite *sprite, vec2 pos)
{
	memcpy(sprite->pos, pos, sizeof(vec2));
}

void sprite_map_put(SpriteMap *cache, Sprite *sprite)
{
	hashmap_set(cache, sprite);
}

static bool spritemap_draw_iter(const void *item, void *udata)
{
	const Sprite *sprite = item;
	const GLuint *VAO = udata;

	draw_sprite(sprite, *VAO);
	return true;
}

void draw_spritemap(SpriteMap *sprite_map, unsigned int *VAO)
{
	hashmap_scan(sprite_map, spritemap_draw_iter, VAO);
}

void draw_sprite(const Sprite *sprite, const GLuint VAO)
{
	glUseProgram(sprite->shader_program);
	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, (vec3){sprite->pos[0], sprite->pos[1], 0.0f});
	glm_scale(model, (vec3){sprite->size[0], sprite->size[1], 0.0f});

	shaderProgramSetMat4(sprite->shader_program, "model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->texture_id);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
