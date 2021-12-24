#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

static GLuint compile_shader(GLenum shader_type, const GLchar *shader_source);
static GLint check_shader_compile_errors(GLuint shader);
static GLint check_shader_program_link_errors(GLuint program);

static char barrier_quad_fragment[] = "#version 330\n"
				      "out vec4 FragColor;"

				      "void main()"
				      "{"
				      "FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
				      "}";

// manually written every index case because having the index as a non-constant
// expression was causing issues on very old drivers
static char quad_fragment[] =
    "#version 330\n"
    "out vec4 FragColor;"

    "in vec2 v_TexCoord;"
    "in float v_TexIndex;"

    "uniform sampler2D u_Textures[20];"

    "void main()"
    "{"
    "int index = int(v_TexIndex);"
    "if (index == 0) FragColor = texture(u_Textures[0], v_TexCoord);"
    "if (index == 1) FragColor = texture(u_Textures[1], v_TexCoord);"
    "if (index == 2) FragColor = texture(u_Textures[2], v_TexCoord);"
    "if (index == 3) FragColor = texture(u_Textures[3], v_TexCoord);"
    "if (index == 4) FragColor = texture(u_Textures[4], v_TexCoord);"
    "if (index == 5) FragColor = texture(u_Textures[5], v_TexCoord);"
    "if (index == 6) FragColor = texture(u_Textures[6], v_TexCoord);"
    "if (index == 7) FragColor = texture(u_Textures[7], v_TexCoord);"
    "if (index == 8) FragColor = texture(u_Textures[8], v_TexCoord);"
    "if (index == 9) FragColor = texture(u_Textures[9], v_TexCoord);"
    "if (index == 10) FragColor = texture(u_Textures[10], v_TexCoord);"
    "if (index == 11) FragColor = texture(u_Textures[11], v_TexCoord);"
    "if (index == 12) FragColor = texture(u_Textures[12], v_TexCoord);"
    "if (index == 13) FragColor = texture(u_Textures[13], v_TexCoord);"
    "if (index == 14) FragColor = texture(u_Textures[14], v_TexCoord);"
    "if (index == 15) FragColor = texture(u_Textures[15], v_TexCoord);"
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

static char movie_fragment[] =
    "#version 330 core\n"
    "out vec4 FragColor;"

    "in vec2 v_TexCoords;"

    "uniform sampler2D u_screenTexture;"

    "void main()"
    "{ "
    "FragColor = texture(u_screenTexture, v_TexCoords);"
    "}";

static char movie_vertex[] = "#version 330\n"
			     "layout (location = 0) in vec2 a_Pos;"
			     "layout (location = 1) in vec2 a_TexCoords;"

			     "out vec2 v_TexCoords;"

			     "void main()"
			     "{"
			     "gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0);"
			     "v_TexCoords = a_TexCoords;"
			     "}";

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

	ShaderProgram movie_shader =
	    create_shader(movie_vertex, movie_fragment);
	if (!movie_shader) {
		printf("Failed to create movie shader.\n");
		return 0;
	}

	ShaderProgram barrier_shader =
	    create_shader(quad_vertex, barrier_quad_fragment);

	shaders[SPRITE_SHADER] = quad_shader;
	shaders[MOVIE_SHADER] = movie_shader;
	shaders[BARRIER_SHADER] = barrier_shader;

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
