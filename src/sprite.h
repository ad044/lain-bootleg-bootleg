#include <GL/glew.h>
#include <cglm/types.h>

typedef struct {
	vec2 pos;
	vec2 size;
	unsigned int texture_index;
	// optional parameters for when offsets/tiling are needed
	vec2 texture_size;
	vec2 texture_offset;
} Sprite;

void generate_sprite_indices(GLuint *buffer, unsigned int index_count);
void load_sprite_vertex_data(GLfloat *buffer, Sprite *sprites,
			     unsigned int sprite_count);
void fill_buffer_data(Sprite *sprites, unsigned int sprite_count,
		      unsigned int index_count);
unsigned int get_sprite_index_count(unsigned int sprite_count);
