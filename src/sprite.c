#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types.h>
#include <stdio.h>

#include "shader.h"
#include "sprite.h"
#include "texture.h"

Sprite make_sprite(TextureCache *cache, char *texture_name, char *name,
		   vec2 initial_pos, vec2 size)
{
	return (Sprite){.texture = texture_cache_get(cache, texture_name),
			.name = name,
			.pos = {initial_pos[0], initial_pos[1]},
			.size = {size[0], size[1]}};
}

void draw_sprite(Sprite *sprite, ShaderProgram shader, GLuint VAO)
{
	glUseProgram(shader);
	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, (vec3){sprite->pos[0], sprite->pos[1], 0.0f});
	glm_scale(model, (vec3){sprite->size[0], sprite->size[1], 0.0f});

	shaderProgramSetMat4(shader, "model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->texture.id);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int init_sprite_vao()
{
	float vertices[] = {
	    // positions (x,y)  and texture coords (u,v)
	    0.0f,  1.0f,  1.0f, 1.0f, // top right
	    0.0f,  -1.0f, 1.0f, 0.0f, // bottom right
	    -1.0f, -1.0f, 0.0f, 0.0f, // bottom left
	    -1.0f, 1.0f,  0.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
	    0, 1, 3, // first triangle
	    1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
			      (void *)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
			      (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}
