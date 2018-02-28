#pragma once

struct GLFWwindow;
struct RawInput;

struct WindowData
{
	GLFWwindow** handle;
	int width;
	int height;
	float aspect_ratio;
	bool is_fullscreen;
};

struct PlatformData
{
	WindowData* window;
	RawInput* new_input;
	RawInput* old_input;
};
