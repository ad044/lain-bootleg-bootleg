#pragma once
#include <cglm/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef GLuint ShaderProgram;
typedef struct hashmap ShaderCache;

typedef struct {
  ShaderProgram id;
  char *name;
} Shader;
int shader_cache_init(ShaderCache **cache);
ShaderProgram shader_cache_get(ShaderCache *cache, char *shader_name);
void shader_cache_put(ShaderCache *cache, char *shader_name, ShaderProgram shader_id);
ShaderProgram create_shader(const char *vertex_shader_path,
			    const char *fragment_shader_path);
void shaderProgramSetVec3(ShaderProgram program, const GLchar *name, vec3 vec);
void shaderProgramSetMat4(ShaderProgram program, const GLchar *name, const mat4 mat);
