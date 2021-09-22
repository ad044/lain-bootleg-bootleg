#pragma once

#include <GL/glew.h>
#include "texture.h"

void draw_text(GLuint texture, char *text);
void draw_clock(Texture *texture, char *time);