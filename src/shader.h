#pragma once
#include <GL/glew.h>
#include <cglm/types.h>
#include <stdbool.h>
#include <stdint.h>

enum { QUAD_SHADER };

typedef GLuint ShaderProgram;

void shader_program_set_texture_samplers(ShaderProgram program,
					 const GLint *samplers,
					 const GLint sampler_count);
void shader_program_set_texture(ShaderProgram program, const GLint texture);
void shader_program_set_mat4(ShaderProgram program, const GLchar *name,
			     mat4 mat);
int shader_cache_init(ShaderProgram **shaders);
