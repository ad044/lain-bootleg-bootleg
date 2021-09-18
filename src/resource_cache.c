#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "resource_cache.h"

// i am not yet sure if &(*ptr) is too much voodoo, so i will stick with it
int init_resource_cache(ResourceCache **resource_cache)
{
	*resource_cache = malloc(sizeof(ResourceCache));
	if (resource_cache == NULL) {
		printf("Failed to allocate memory for resource cache.\n");
		return 0;
	}

	// initialize shader cache
	if (!(shader_cache_init(&(*resource_cache)->shaders))) {
		printf("Failed to initialize shader cache.\n");
		return 0;
	}

	// initialize texture cache and fill it
	// (for now i will try to preload every texture, which is currently)
	// around 9 mb. if this proves to be problematic we can change the
	// approach.
	if (!(texture_cache_init(&(*resource_cache)->textures))) {
		printf("Failed to initialize texture cache.\n");
		return 0;
	}

	// initialize quad vao (for sprites)
	init_quad_vao(&(*resource_cache)->quad_VAO);

	// preload textures into cache
	preload_textures((*resource_cache)->textures);

	return 1;
}

void init_quad_vao(unsigned int *quad_VAO)
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
	unsigned int VBO, EBO;
	glGenVertexArrays(1, quad_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(*quad_VAO);

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
}
