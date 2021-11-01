#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "quad.h"
#include "resources.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"

#define MAX_SCENE_QUADS 30
#define MAX_SCENE_TEXTURES 20

#define MAX_SCENE_VBO_SIZE MAX_SCENE_QUADS *QUAD_VBO_SIZE
#define MAX_SCENE_IBO_SIZE MAX_SCENE_QUADS *QUAD_INDEX_COUNT

static SceneTextureSlot register_texture_slot(Scene *scene, Texture *texture)
{
	SceneTextureSlot texture_slot;

	texture_slot.texture_index = cvector_size(scene->texture_slots);
	texture_slot.texture = texture;

	cvector_push_back(scene->texture_slots, texture_slot);

	return texture_slot;
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

void init_scene(Scene *scene, Sprite **sprites, uint8_t sprite_count,
		SpriteBehavior *sprite_behaviors, uint8_t sprite_behavior_count,
		Text **text_objs, uint8_t text_obj_count, ShaderProgram shader)
{
	// initialize vao, vbo, ibo
	init_scene_buffers(scene);

	scene->texture_slots = NULL;

	scene->sprites = NULL;
	for (int i = 0; i < sprite_count; i++) {
		Sprite *sprite = sprites[i];

		SceneTextureSlot slot =
		    register_texture_slot(scene, sprite->texture);

		sprite->texture_index = slot.texture_index;
		sprite->origin_pos = sprite->pos;

		// if spritesheet, set size for single sprite (of what will be
		// displayed), and initialize current frame to 0
		if (sprite->is_spritesheet) {
			float size_x = 1.0f / ((float)sprite->max_frame + 1.0f);
			sprite->texture_size = (Vector2D){size_x, 1.0f};
			sprite->current_frame = 0;
		} else {
			sprite->texture_size = (Vector2D){1.0f, 1.0f};
		}

		cvector_push_back(scene->sprites, sprite);
	}

	depth_sort(scene->sprites, sprite_count);

	scene->sprite_behaviors = NULL;
	for (int i = 0; i < sprite_behavior_count; i++) {
		cvector_push_back(scene->sprite_behaviors, sprite_behaviors[i]);
	}

	scene->text_objects = NULL;
	for (int i = 0; i < text_obj_count; i++) {
		Text *text = text_objs[i];

		SceneTextureSlot slot =
		    register_texture_slot(scene, text->font->texture);

		text->texture_index = slot.texture_index;
		text->origin_pos = text->pos;

		cvector_push_back(scene->text_objects, text);
	}

	scene->shader = shader;

	update_scene(scene);
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
				scene->texture_slots[i].texture_index);
		glBindTexture(GL_TEXTURE_2D,
			      scene->texture_slots[i].texture->id);
	}

	// bind shader and set texture samplers
	glUseProgram(scene->shader);
	GLint samplers[MAX_SCENE_TEXTURES];
	for (int i = 0; i < texture_count; i++) {
		samplers[i] = scene->texture_slots[i].texture_index;
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

void update_texture_slot(Scene *scene, Sprite *sprite, Texture *texture)
{
	scene->texture_slots[sprite->texture_index].texture = texture;
}
