#pragma once

#include <GL/glew.h>

typedef struct {
	GLfloat x;
	GLfloat y;
} Vector2D;

float dist_between(Vector2D v1, Vector2D v2);
