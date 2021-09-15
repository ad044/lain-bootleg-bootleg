#pragma once
#include <cglm/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef GLuint ShaderProgram;
typedef struct hashmap shader_cache;

typedef struct {
  ShaderProgram id;
  char *name;
} Shader;
int shader_cache_init(shader_cache **cache);
ShaderProgram shader_cache_get(shader_cache *cache, char *shader_name);
void shader_cache_put(shader_cache *cache, char *name, ShaderProgram shader_id);
ShaderProgram create_shader(const char *vertex_shader_path,
			    const char *fragment_shader_path);
void shaderProgramSetVec3(ShaderProgram program, const GLchar *name, vec3 vec);
void shaderProgramSetMat4(ShaderProgram program, const GLchar *name, const mat4 mat);
