#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"

void init_vertex_array(GLuint *VAO, GLfloat vertices[], GLuint vertices_size,
		       GLuint indices[], GLuint indices_size)
{
	GLuint VBO, IBO;

	glGenVertexArrays(1, VAO);

	glBindVertexArray(*VAO);

	init_vertex_buffer(&VBO, vertices, vertices_size);
	init_index_buffer(&IBO, indices, indices_size);

	bind_vertex_buffer(VBO);
	bind_index_buffer(IBO);

	// position
	set_vertex_attrib_pointer(0, 2, 5 * sizeof(GLfloat), (void *)0);
	// texture coords
	set_vertex_attrib_pointer(1, 2, 5 * sizeof(GLfloat),
				  (void *)(2 * sizeof(GLfloat)));
	// texture id
	set_vertex_attrib_pointer(2, 1, 5 * sizeof(GLfloat),
				  (void *)(4 * sizeof(GLfloat)));
}

void bind_vertex_array(GLuint VAO) { glBindVertexArray(VAO); }

void unbind_vertex_array(GLuint VAO) { glBindVertexArray(0); }