#pragma once

#include <GL/glew.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SHADER_COUNT 8

enum { SPRITE_SHADER, MOVIE_SHADER };

typedef GLuint ShaderProgram;

int shaders_init(ShaderProgram *shaders);
void shader_program_set_texture_samplers(ShaderProgram program,
					 const GLint *samplers,
					 const GLint sampler_count);
void shader_program_set_mat4(ShaderProgram program, const GLchar *name,
			     const GLfloat mat4[4][4]);
