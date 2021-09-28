#pragma once
#include <GL/glew.h>
#include <cglm/types.h>
#include <stdbool.h>
#include <stdint.h>

typedef GLuint ShaderProgram;
typedef struct hashmap ShaderCache;

typedef struct {
	ShaderProgram id;
	char *name;
} Shader;
int shader_cache_init(ShaderCache **cache);
ShaderProgram shader_cache_get(ShaderCache *cache, char *shader_name);
void shader_cache_put(ShaderCache *cache, char *shader_name,
		      ShaderProgram shader_id);
ShaderProgram create_shader(const char *vertex_shader_path,
			    const char *fragment_shader_path);
void shader_program_set_texture_samplers(ShaderProgram program,
					 const GLint *samplers,
					 const GLint sampler_count);
void shader_program_set_texture(ShaderProgram program, const GLint texture);
void preload_shaders();
// todo
void shader_cache_free(ShaderCache *cache);
