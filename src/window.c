#include <stdio.h>
#include <stdlib.h>

#include "stb_image.h"

#include "menu.h"
#include "scene.h"
#include "window.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type,
						 GLuint id, GLenum severity,
						 GLsizei length,
						 const GLchar *message,
						 const void *user_param)
{
	fprintf(stderr, "\ntype = 0x%x, severity = 0x%x, message = %s\n", type,
		severity, message);
}

static void set_window_icon(GLFWwindow *window)
{
	stbi_set_flip_vertically_on_load(false);

	GLFWimage images[1];
	images[0].pixels = stbi_load("./res/window_icon.png", &images[0].width,
				     &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
}

static GLFWmonitor *get_current_monitor(GLFWwindow *window)
{
	int nmonitors, i;
	int wx, wy, ww, wh;
	int mx, my, mw, mh;
	int overlap, bestoverlap;
	GLFWmonitor *bestmonitor;
	GLFWmonitor **monitors;
	const GLFWvidmode *mode;

	bestoverlap = 0;
	bestmonitor = NULL;

	glfwGetWindowPos(window, &wx, &wy);
	glfwGetWindowSize(window, &ww, &wh);
	monitors = glfwGetMonitors(&nmonitors);

	for (i = 0; i < nmonitors; i++) {
		mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &mx, &my);
		mw = mode->width;
		mh = mode->height;

		overlap = MAX(0, MIN(wx + ww, mx + mw) - MAX(wx, mx)) *
			  MAX(0, MIN(wy + wh, my + mh) - MAX(wy, my));

		if (bestoverlap < overlap) {
			bestoverlap = overlap;
			bestmonitor = monitors[i];
		}
	}

	return bestmonitor;
}

static void get_centered_window_coords(GLFWwindow *window, int *x, int *y)
{
	GLFWmonitor *monitor = get_current_monitor(window);
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);

	int win_width, win_height;
	glfwGetWindowSize(window, &win_width, &win_height);

	*x = (mode->width - win_width) / 2;
	*y = (mode->height - win_height) / 2;
}

int make_window(GLFWwindow **window, int width, int height, char *name,
		GLFWwindow *shared_ctx, _Bool centered)
{
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	*window = glfwCreateWindow(width, height, name, NULL, shared_ctx);
	if (window == NULL) {
		printf("Failed to initialize window.\n");
		glfwTerminate();
		return 0;
	}

	int posx, posy;

	if (centered) {
		get_centered_window_coords(*window, &posx, &posy);
	} else {
		int left, top, right, bottom;
		glfwGetWindowFrameSize(*window, &left, &top, &right, &bottom);
		posx = left;
		posy = top;
	}

	glfwSetWindowPos(*window, posx, posy);
	set_window_icon(*window);

	glfwShowWindow(*window);

	glfwMakeContextCurrent(*window);

	glfwSwapInterval(1);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize glew.\n");
		glfwTerminate();
		return 0;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_debug_message_callback, 0);

	if (shared_ctx != NULL) {
		Engine *engine = (Engine *)glfwGetWindowUserPointer(shared_ctx);
		glfwSetWindowUserPointer(*window, engine);
		glfwSetMouseButtonCallback(*window, handle_minigame_event);
	}
	return 1;
}
