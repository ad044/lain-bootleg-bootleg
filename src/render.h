#include "shader.h"
#include "scene.h"
#include "texture.h"

typedef struct {
	GLuint VAO;
	ShaderProgram shader;
	Texture *textures;
	unsigned int texture_count;
	GLint *samplers;
	GLuint index_count;
} RenderData;
void render(RenderData render_data);
