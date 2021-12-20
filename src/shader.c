#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

static char quad_fragment[] =
    "#version 330\n"
    "out vec4 FragColor;"

    "in vec2 v_TexCoord;"
    "in float v_TexIndex;"

    "uniform sampler2D u_Textures[20];"

    "void main()"
    "{"
    "int index = int(v_TexIndex);"
    "if (index == -1) {"
    "FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
    "} else {"
    "FragColor = texture(u_Textures[index], v_TexCoord);"
    "}"
    "}";

static char quad_vertex[] =
    "#version 330\n"
    "layout (location = 0) in vec2 a_Pos;"
    "layout (location = 1) in vec2 a_TexCoord;"
    "layout (location = 2) in float a_TexIndex;"

    "out vec2 v_TexCoord;"
    "out float v_TexIndex;"

    "uniform mat4 u_Model;"
    "uniform mat4 u_Projection;"
    "uniform mat4 u_View;"

    "void main()"
    "{"
    "v_TexCoord = a_TexCoord;"
    "v_TexIndex = a_TexIndex;"
    "gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 0.0, 1.0);"
    "}";

static GLuint compile_shader(GLenum shader_type, const GLchar *shader_source);
static GLint check_shader_compile_errors(GLuint shader);
static GLint check_shader_program_link_errors(GLuint program);

static ShaderProgram create_shader(const char *vertex, const char *fragment)
{
	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex);
	if (!vertex_shader) {
		return 0;
	}

	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment);
	if (!fragment_shader) {
		return 0;
	}

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	if (!check_shader_program_link_errors(shader_program)) {
		return 0;
	}

	return shader_program;
}

static GLuint compile_shader(GLenum shader_type, const GLchar *shader_source)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);
	if (!check_shader_compile_errors(shader)) {
		return 0;
	}

	return shader;
}

static GLint check_shader_compile_errors(GLuint shader)
{
	GLint success;
	GLchar log[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", log);
	}

	return success;
}

static GLint check_shader_program_link_errors(GLuint program)
{
	GLint success;
	GLchar log[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, log);
		printf("ERROR::SHADER::LINKING_FAILED\n%s\n", log);
	}

	return success;
}

int shaders_init(ShaderProgram *shaders)
{
	ShaderProgram quad_shader = create_shader(quad_vertex, quad_fragment);
	if (!quad_shader) {
		printf("Failed to create quad shader.\n");
		return 0;
	}
	shaders[SPRITE_SHADER] = quad_shader;

	return 1;
}

void shader_program_set_texture_samplers(ShaderProgram program,
					 const GLint *samplers,
					 const GLint sampler_count)
{
	glUniform1iv(glGetUniformLocation(program, "u_Textures"), sampler_count,
		     samplers);
}

void shader_program_set_mat4(ShaderProgram program, const GLchar *name,
			     const GLfloat mat4[4][4])
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
			   (const GLfloat *)mat4);
}
