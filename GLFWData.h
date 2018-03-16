#pragma once

struct GLFWwindow;
struct RawInput;

struct GLFWWindowData
{
	GLFWwindow** handle;
	int width;
	int height;
	float aspect_ratio;
	bool is_fullscreen;
	int old_win_x, old_win_y, old_win_w, old_win_h; //for restoring from fullscreen
};

struct GLFWData
{
	GLFWWindowData* window;
	RawInput* new_input;
	RawInput* old_input;
};
