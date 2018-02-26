#pragma once
#include "gl_lite.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Input.h"

bool init_gl(GLFWwindow** window, const char* title, int window_width, int window_height);
void window_resize_callback(GLFWwindow* window, int width, int height);

bool init_gl(GLFWwindow** window, const char* title, int window_width, int window_height) {

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

	*window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
	if(!(*window)) {
		fprintf(stderr, "Error: glfwCreateWindow failed\n");
		glfwTerminate();
		getchar();
		return false;
	}
	const char* glfw_version = glfwGetVersionString();
	printf("GLFW Version %s\n", glfw_version);
	glfwMakeContextCurrent(*window);

	//Setup callbacks
	glfwSetKeyCallback(*window, key_callback);
	glfwSetWindowSizeCallback(*window, window_resize_callback);
	glfwSetMouseButtonCallback(*window, mouse_button_callback);
	glfwSetCursorPosCallback(*window, cursor_pos_callback);
	glfwSetScrollCallback(*window, scroll_callback);
	glfwSetCursorEnterCallback(*window, cursor_enter_callback);
	glfwSetJoystickCallback(joystick_callback);
	glfwSwapInterval(1);

	init_joystick();

	g_mouse.sensitivity = MOUSE_DEFAULT_SENSITIVITY;

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
    gl_width = width;
    gl_height = height;
    gl_aspect_ratio = (float)gl_width/gl_height;
    int fb_w, fb_h;
	glfwGetFramebufferSize(window, &fb_w, &fb_h);
	glViewport(0, 0, fb_w, fb_h);
}
