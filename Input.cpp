#include "Input.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Platform.h"
#include "GameMaths.h"
#include "string_functions.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    GameInput* input = platform_data->input;

    //Immutable input keys:
    bool is_pressed = (action != GLFW_RELEASE);
    switch(key){
        case GLFW_KEY_W:        input->move_input[MOVE_FORWARD]   = is_pressed; return;
        case GLFW_KEY_A:        input->move_input[MOVE_LEFT]      = is_pressed; return;
        case GLFW_KEY_S:        input->move_input[MOVE_BACK]      = is_pressed; return;
        case GLFW_KEY_D:        input->move_input[MOVE_RIGHT]     = is_pressed; return;
        case GLFW_KEY_UP:       input->move_input[TILT_CAM_UP]    = is_pressed; return;
        case GLFW_KEY_DOWN:     input->move_input[TILT_CAM_DOWN]  = is_pressed; return;
        case GLFW_KEY_LEFT:     input->move_input[TURN_CAM_LEFT]  = is_pressed; return;
        case GLFW_KEY_RIGHT:    input->move_input[TURN_CAM_RIGHT] = is_pressed; return;
        case GLFW_KEY_SPACE:    input->button_input[JUMP]         = is_pressed; return;
        case GLFW_KEY_Q:        input->button_input[RAISE_CAM]    = is_pressed; return;
        case GLFW_KEY_E:        input->button_input[LOWER_CAM]    = is_pressed; return;
        default: break;
    }
    //Custom key mapping:
    // if(key == input->key_mapping[DASH_MOVE]){  input->button_input[DASH_MOVE] = is_pressed; return; }
    // if(key == input->key_mapping[JUMP_MOVE]){  input->button_input[JUMP_MOVE] = is_pressed; return; }
}

//Mouse stuff

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    GameInput* input = platform_data->input;

    bool is_pressed = (action != GLFW_RELEASE);
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT:  input->mouse.click_left = is_pressed;  return;
        case GLFW_MOUSE_BUTTON_RIGHT: input->mouse.click_right = is_pressed; return;
        default: return;
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos){
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    GameInput* input = platform_data->input;

    input->mouse.xpos = xpos;
    input->mouse.ypos = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    GameInput* input = platform_data->input;

    input->mouse.xscroll = xoffset;
    input->mouse.yscroll = yoffset;
}

void cursor_enter_callback(GLFWwindow* window, int entered){
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    GameInput* input = platform_data->input;

    input->mouse.is_in_window = entered;
}

//Joystick stuff

void init_joystick(ControllerState* controller)
{
    controller->is_connected = true;
    
    const char* joystick_name = glfwGetJoystickName(GLFW_JOYSTICK_1);

    if(strings_are_equal(joystick_name, "XBox 360 Controller")) 
        controller->controller_type = CONTROLLER_TYPE_XBOX;
    else if(strings_are_equal(joystick_name, "Wireless Controller")) 
        controller->controller_type = CONTROLLER_TYPE_PS4;
    else {
        printf("Warning: Unknown joystick %s\n", joystick_name);
        return;
    }
}

void poll_joystick(GameInput* input)
{
    if(!glfwJoystickPresent(GLFW_JOYSTICK_1)){
        input->controller.is_connected = false;        
        return;
    }

    if(!input->controller.is_connected) 
        init_joystick(&input->controller);

    int axis_count;
    const float* polled_axis_values = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
    int button_count;
    const unsigned char* polled_button_values = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);

    //XBox layout is default, just copy values
    if(input->controller.controller_type == CONTROLLER_TYPE_XBOX)
    {
        for(int i=0; i<axis_count; ++i){
            input->controller.axis[i] = polled_axis_values[i];
        }

        for(int i=0; i<button_count; ++i){
            input->controller.button[i] = (bool)polled_button_values[i];
        }
    }
    //Convert PS4 input to XBox input
    else if(input->controller.controller_type == CONTROLLER_TYPE_PS4)
    {
        input->controller.axis[XBOX_LEFT_STICK_HOR]   = polled_axis_values[PS4_LEFT_STICK_HOR];
        input->controller.axis[XBOX_LEFT_STICK_VERT]  = -polled_axis_values[PS4_LEFT_STICK_VERT];
        input->controller.axis[XBOX_RIGHT_STICK_HOR]  = polled_axis_values[PS4_RIGHT_STICK_HOR];
        input->controller.axis[XBOX_RIGHT_STICK_VERT] = -polled_axis_values[PS4_RIGHT_STICK_VERT];
        input->controller.axis[XBOX_LT]               = polled_axis_values[PS4_L2];
        input->controller.axis[XBOX_RT]               = polled_axis_values[PS4_R2];

        input->controller.button[XBOX_BUTTON_A]          = polled_button_values[PS4_BUTTON_CROSS];
        input->controller.button[XBOX_BUTTON_B]          = polled_button_values[PS4_BUTTON_CIRCLE];
        input->controller.button[XBOX_BUTTON_X]          = polled_button_values[PS4_BUTTON_SQUARE];
        input->controller.button[XBOX_BUTTON_Y]          = polled_button_values[PS4_BUTTON_TRIANGLE];
        input->controller.button[XBOX_BUTTON_LB]         = polled_button_values[PS4_BUTTON_L1];
        input->controller.button[XBOX_BUTTON_RB]         = polled_button_values[PS4_BUTTON_R1];
        input->controller.button[XBOX_BUTTON_BACK]       = polled_button_values[PS4_BUTTON_SHARE];
        input->controller.button[XBOX_BUTTON_START]      = polled_button_values[PS4_BUTTON_OPTIONS];
        input->controller.button[XBOX_BUTTON_LS]         = polled_button_values[PS4_BUTTON_L3];
        input->controller.button[XBOX_BUTTON_RS]         = polled_button_values[PS4_BUTTON_R3];
        input->controller.button[XBOX_BUTTON_DPAD_UP]    = polled_button_values[PS4_BUTTON_DPAD_UP];
        input->controller.button[XBOX_BUTTON_DPAD_RIGHT] = polled_button_values[PS4_BUTTON_DPAD_RIGHT];
        input->controller.button[XBOX_BUTTON_DPAD_DOWN]  = polled_button_values[PS4_BUTTON_DPAD_DOWN];
        input->controller.button[XBOX_BUTTON_DPAD_LEFT]  = polled_button_values[PS4_BUTTON_DPAD_LEFT];
    }

    //Denoise analogue sticks
    for(int i=0; i<4; ++i){
        if(fabsf(input->controller.axis[i])<0.1) input->controller.axis[i] = 0;
    }
    
    input->move_input[MOVE_LEFT]    = CLAMP(-input->controller.axis[XBOX_LEFT_STICK_HOR], 0, 1);
    input->move_input[MOVE_RIGHT]   = CLAMP( input->controller.axis[XBOX_LEFT_STICK_HOR], 0, 1);
    input->move_input[MOVE_FORWARD] = CLAMP( input->controller.axis[XBOX_LEFT_STICK_VERT], 0, 1);
    input->move_input[MOVE_BACK]    = CLAMP(-input->controller.axis[XBOX_LEFT_STICK_VERT], 0, 1);
    input->move_input[TURN_CAM_LEFT]  = CLAMP(-input->controller.axis[XBOX_RIGHT_STICK_HOR], 0, 1);
    input->move_input[TURN_CAM_RIGHT] = CLAMP( input->controller.axis[XBOX_RIGHT_STICK_HOR], 0, 1);
    input->move_input[TILT_CAM_DOWN]  = CLAMP( input->controller.axis[XBOX_RIGHT_STICK_VERT], 0, 1);
    input->move_input[TILT_CAM_UP]    = CLAMP(-input->controller.axis[XBOX_RIGHT_STICK_VERT], 0, 1);

    input->button_input[JUMP] = input->controller.button[XBOX_BUTTON_A];
}
