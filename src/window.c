#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "scene.h"
#include "window.h"

static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type,
						 GLuint id, GLenum severity,
						 GLsizei length,
						 const GLchar *message,
						 const void *user_param)
{
	fprintf(stderr, "\ntype = 0x%x, severity = 0x%x, message = %s\n", type,
		severity, message);
}

void shrink_main_window(GLFWwindow *window)
{
	glfwSetWindowSize(window, SHRINKED_MENU_WIDTH, SHRINKED_MENU_HEIGHT);
}

void expand_main_window(GLFWwindow *window)
{
	glfwSetWindowSize(window, EXPANDED_MENU_WIDTH, EXPANDED_MENU_HEIGHT);
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

		overlap = max(0, min(wx + ww, mx + mw) - max(wx, mx)) *
			  max(0, min(wy + wh, my + mh) - max(wy, my));

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

	return 1;
}
