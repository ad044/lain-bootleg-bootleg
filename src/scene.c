#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "quad.h"
#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"

#define MAX_SCENE_QUADS 30
#define MAX_SCENE_TEXTURES 20

#define MAX_SCENE_VBO_SIZE MAX_SCENE_QUADS *QUAD_VBO_SIZE
#define MAX_SCENE_IBO_SIZE MAX_SCENE_QUADS *QUAD_INDEX_COUNT

static SceneTextureSlot *register_texture_slot(Scene *scene, Texture *texture)
{
	SceneTextureSlot *slot =
	    make_texture_slot(cvector_size(scene->texture_slots), texture);

	cvector_push_back(scene->texture_slots, slot);

	return slot;
}

static int register_sprite(Scene *scene, SceneSprite scene_sprite)
{
	*scene_sprite.loc = malloc(sizeof(Sprite));
	if (scene_sprite.loc == NULL) {
		printf("Failed to allocate memory for sprite\n");
		return 0;
	}

	Sprite sprite_obj = scene_sprite.sprite;

	SceneTextureSlot *slot =
	    register_texture_slot(scene, sprite_obj.texture);

	sprite_obj.texture_index = slot->texture_index;

	// initialize origin position
	sprite_obj.origin_pos = sprite_obj.pos;

	// if spritesheet, set size for single sprite (of what will be
	// displayed), and initialize current frame to 0
	if (sprite_obj.is_spritesheet) {
		float size_x = 1.0f / ((float)sprite_obj.max_frame + 1.0f);
		sprite_obj.texture_size = (Vector2D){size_x, 1.0f};
		sprite_obj.current_frame = 0;
	} else {
		sprite_obj.texture_size = (Vector2D){1.0f, 1.0f};
	}

	memcpy(*scene_sprite.loc, &sprite_obj, sizeof(Sprite));

	cvector_push_back(scene->sprites, *scene_sprite.loc);

	return 1;
}

static int register_text_object(Scene *scene, SceneText scene_text)
{
	*scene_text.loc = malloc(sizeof(Text));
	if (scene_text.loc == NULL) {
		printf("Failed to allocate memory for sprite\n");
		return 0;
	}

	SceneTextureSlot *slot =
	    register_texture_slot(scene, scene_text.text.font->texture);

	scene_text.text.texture_index = slot->texture_index;

	scene_text.text.origin_pos = scene_text.text.pos;

	memcpy(*scene_text.loc, &scene_text.text, sizeof(Text));

	cvector_push_back(scene->text_objects, *scene_text.loc);

	return 1;
}

static void init_scene_buffers(Scene *scene)
{
	glGenVertexArrays(1, &scene->VAO);
	glBindVertexArray(scene->VAO);

	glGenBuffers(1, &scene->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, scene->VBO);

	glGenBuffers(1, &scene->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->IBO);

	GLuint indices[MAX_SCENE_IBO_SIZE];

	generate_quad_indices(indices, MAX_SCENE_IBO_SIZE);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)0);
	// texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(2 * sizeof(GLfloat)));

	// texture id
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(4 * sizeof(GLfloat)));
}

int init_scene(Scene *scene, SceneDefinition *scene_definition,
	       ResourceCache *resource_cache)
{
	SceneSprite *sprites = scene_definition->sprites;
	unsigned int sprite_count = scene_definition->sprite_count;

	SpriteBehavior *behaviors = scene_definition->behaviors;
	unsigned int behavior_count = scene_definition->behavior_count;

	SceneText *text_objects = scene_definition->text_objects;
	unsigned int text_obj_count = scene_definition->text_object_count;

	// initialize vao, vbo, ibo
	init_scene_buffers(scene);

	// initialize texture slots
	scene->texture_slots = NULL;

	// set up sprites
	scene->sprites = NULL;
	for (int i = 0; i < sprite_count; i++) {
		if (!register_sprite(scene, sprites[i])) {
			return 0;
		}
	}

	// sort sprites by z index
	depth_sort(scene->sprites, sprite_count);

	// register behaviors
	scene->sprite_behaviors = NULL;
	for (int i = 0; i < behavior_count; i++) {
		cvector_push_back(scene->sprite_behaviors, behaviors[i]);
	}

	// register text objects
	scene->text_objects = NULL;
	for (int i = 0; i < text_obj_count; i++) {
		if (!register_text_object(scene, text_objects[i])) {
			return 0;
		};
	}

	scene->shader = resource_cache->shaders[QUAD_SHADER];

	update_scene(scene);

	return 1;
}

void update_scene(Scene *scene)
{
	glBindVertexArray(scene->VAO);

	GLfloat vertices[MAX_SCENE_VBO_SIZE];
	GLfloat *buffer_ptr = vertices;

	unsigned int quad_count = 0;
	for (int i = 0; i < cvector_size(scene->sprites); i++) {
		if (!scene->sprites[i]->visible) {
			continue;
		}
		buffer_ptr = get_quad_vertices(buffer_ptr, scene->sprites[i]);
		quad_count++;
	};

	for (int i = 0; i < cvector_size(scene->text_objects); i++) {
		Text *text_obj = scene->text_objects[i];
		if (!text_obj->visible) {
			continue;
		}

		char *text = text_obj->current_text;
		int text_len = strlen(text);
		// on each letter pad the entire text back by one letter size

		for (int j = 0; j < text_len; j++) {
			Sprite glyph = get_glyph(text_obj, text[j], j);
			buffer_ptr = get_quad_vertices(buffer_ptr, &glyph);
			quad_count++;

			if (text[j] == 'A' || text[j] == 'P') {
				j++;
			}
		}
	}

	scene->quad_count = quad_count;

	glBindBuffer(GL_ARRAY_BUFFER, scene->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STREAM_DRAW);
}

void draw_scene(Scene *scene, GLFWwindow *window)
{
	// window data
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);

	unsigned int texture_count = cvector_size(scene->texture_slots);

	// bind appropriate textures
	for (int i = 0; i < texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 +
				scene->texture_slots[i]->texture_index);
		glBindTexture(GL_TEXTURE_2D,
			      scene->texture_slots[i]->texture->id);
	}

	// bind shader and set texture samplers
	glUseProgram(scene->shader);
	GLint samplers[MAX_SCENE_TEXTURES];
	for (int i = 0; i < texture_count; i++) {
		samplers[i] = scene->texture_slots[i]->texture_index;
	}
	shader_program_set_texture_samplers(scene->shader, samplers,
					    texture_count);

	// set up matrices
	mat4 proj, model, view;

	glm_ortho(0, w, h, 0, -1.0f, 1.0f, proj);

	glm_mat4_identity(model);
	glm_mat4_identity(view);

	shader_program_set_mat4(scene->shader, "u_Projection", proj);
	shader_program_set_mat4(scene->shader, "u_Model", model);
	shader_program_set_mat4(scene->shader, "u_View", view);

	// bind vao
	glBindVertexArray(scene->VAO);

	// draw
	glDrawElements(GL_TRIANGLES, get_quad_index_count(scene->quad_count),
		       GL_UNSIGNED_INT, 0);
}

SceneTextureSlot *make_texture_slot(unsigned int index, Texture *texture)
{

	SceneTextureSlot *texture_slot;
	texture_slot = (SceneTextureSlot *)malloc(sizeof(SceneTextureSlot));

	if (texture_slot == NULL) {
		printf("Failed to allocate memory for texture slot.\n");
		exit(1);
	}

	texture_slot->texture_index = index;
	texture_slot->texture = texture;

	return texture_slot;
}

void update_texture_slot(Scene *scene, Sprite *sprite, Texture *texture)
{
	SceneTextureSlot *slot = scene->texture_slots[sprite->texture_index];

	slot->texture = texture;
}
