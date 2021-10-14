#pragma once

#include <GL/glew.h>

typedef struct {
	GLfloat x;
	GLfloat y;
} Vector2D;

Vector2D make_vec2d(GLfloat x, GLfloat y);