#pragma once

#include <GL/glew.h>
#include <cglm/types.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SHADER_COUNT 10

enum { SPRITE_SHADER };

typedef GLuint ShaderProgram;

int shaders_init(ShaderProgram *shaders);
void shader_program_set_texture_samplers(ShaderProgram program,
					 const GLint *samplers,
					 const GLint sampler_count);
void shader_program_set_mat4(ShaderProgram program, const GLchar *name,
			     mat4 mat);
