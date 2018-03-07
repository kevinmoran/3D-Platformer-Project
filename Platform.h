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
	int old_win_x, old_win_y, old_win_w, old_win_h; //for restoring from fullscreen
};

struct PlatformData
{
	WindowData* window;
	RawInput* new_input;
	RawInput* old_input;
};
