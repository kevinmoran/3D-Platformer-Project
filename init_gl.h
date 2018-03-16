#pragma once
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "gl_lite.h"
#include "GLFWData.h"
#include "Input.h"

void window_resize_callback(GLFWwindow* window, int width, int height);

bool init_gl(GLFWData* glfw_data, const char* title)
{
	/* start GL context and O/S window using the GLFW helper library */
	if(!glfwInit()) {
		fprintf(stderr, "Error: glfwInit failed\n");
		getchar();
		return false;
	}

	#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

	GLFWwindow** window = glfw_data->window->handle;

	*window = glfwCreateWindow(glfw_data->window->width, glfw_data->window->height, title, NULL, NULL);
	if(!(*window)) {
		fprintf(stderr, "Error: glfwCreateWindow failed\n");
		glfwTerminate();
		getchar();
		return false;
	}
	const char* glfw_version = glfwGetVersionString();
	printf("GLFW Version %s\n", glfw_version);
	glfwMakeContextCurrent(*window);

	glfwSetWindowUserPointer(*window, glfw_data);

	//Setup callbacks
	glfwSetKeyCallback(*window, key_callback);
	glfwSetWindowSizeCallback(*window, window_resize_callback);
	glfwSetMouseButtonCallback(*window, mouse_button_callback);
	glfwSetCursorPosCallback(*window, cursor_pos_callback);
	glfwSetScrollCallback(*window, scroll_callback);
	glfwSetCursorEnterCallback(*window, cursor_enter_callback);
	glfwSwapInterval(1);

	//Load OpenGL functions
	if(!gl_lite_init()){
		printf("Error in gl_lite_init\n");
		return false;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	printf("GLSL version supported: %s\n", glsl_version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return true;
}

void window_resize_callback(GLFWwindow* window, int width, int height){
	GLFWData* glfw_data = (GLFWData*)glfwGetWindowUserPointer(window);
	GLFWWindowData* win = glfw_data->window;

    win->width = width;
    win->height = height;
    win->aspect_ratio = (float)win->width/win->height;

    int fb_w, fb_h;
	glfwGetFramebufferSize(window, &fb_w, &fb_h);
	glViewport(0, 0, fb_w, fb_h);
}
