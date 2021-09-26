#include "render.h"

void render(RenderData render_data)
{
	// bind appropriate textures
	for (int i = 0; i < render_data.texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, render_data.textures[i].id);
	}

	// bind shader and set texture samplers
	glUseProgram(render_data.shader);
	shader_program_set_samplers(render_data.shader, "u_Textures",
				    render_data.samplers,
				    render_data.texture_count);

	// bind vao
	glBindVertexArray(render_data.VAO);

	// draw
	glDrawElements(GL_TRIANGLES, render_data.index_count, GL_UNSIGNED_INT,
		       0);
}
