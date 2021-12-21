#pragma once

#include <GL/glew.h>
#include <mpv/client.h>
#include <mpv/render.h>
#include <mpv/render_gl.h>

#include "shader.h"

typedef struct {
	mpv_handle *mpv_handle;
	mpv_render_context *mpv_render_ctx;
	GLuint texture_buffer;
	GLuint FBO;
	GLuint VBO;
	GLuint VAO;
} Movie;

int movie_init(Movie *movie);
void movie_render(ShaderProgram shader, Movie *movie);
