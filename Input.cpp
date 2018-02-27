#include "Input.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Platform.h"
#include "GameMaths.h"
#include "string_functions.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    RawInput* input = platform_data->input;

    //Immutable input keys:
    bool is_pressed = (action != GLFW_RELEASE);
    switch(key){
        case GLFW_KEY_SPACE:        input->keyboard_input[KEY_SPACE] = is_pressed; return;
        case GLFW_KEY_APOSTROPHE:   input->keyboard_input[KEY_APOSTROPHE] = is_pressed; return;
        case GLFW_KEY_COMMA:        input->keyboard_input[KEY_COMMA] = is_pressed; return;
        case GLFW_KEY_MINUS:        input->keyboard_input[KEY_MINUS] = is_pressed; return;
        case GLFW_KEY_PERIOD:       input->keyboard_input[KEY_PERIOD] = is_pressed; return;
        case GLFW_KEY_SLASH:        input->keyboard_input[KEY_SLASH] = is_pressed; return;
        case GLFW_KEY_0:            input->keyboard_input[KEY_0] = is_pressed; return;
        case GLFW_KEY_1:            input->keyboard_input[KEY_1] = is_pressed; return;
        case GLFW_KEY_2:            input->keyboard_input[KEY_2] = is_pressed; return;
        case GLFW_KEY_3:            input->keyboard_input[KEY_3] = is_pressed; return;
        case GLFW_KEY_4:            input->keyboard_input[KEY_4] = is_pressed; return;
        case GLFW_KEY_5:            input->keyboard_input[KEY_5] = is_pressed; return;
        case GLFW_KEY_6:            input->keyboard_input[KEY_6] = is_pressed; return;
        case GLFW_KEY_7:            input->keyboard_input[KEY_7] = is_pressed; return;
        case GLFW_KEY_8:            input->keyboard_input[KEY_8] = is_pressed; return;
        case GLFW_KEY_9:            input->keyboard_input[KEY_9] = is_pressed; return;
        case GLFW_KEY_SEMICOLON:    input->keyboard_input[KEY_SEMICOLON] = is_pressed; return;
        case GLFW_KEY_EQUAL:        input->keyboard_input[KEY_EQUAL] = is_pressed; return;
        case GLFW_KEY_A:            input->keyboard_input[KEY_A] = is_pressed; return;
        case GLFW_KEY_B:            input->keyboard_input[KEY_B] = is_pressed; return;
        case GLFW_KEY_C:            input->keyboard_input[KEY_C] = is_pressed; return;
        case GLFW_KEY_D:            input->keyboard_input[KEY_D] = is_pressed; return;
        case GLFW_KEY_E:            input->keyboard_input[KEY_E] = is_pressed; return;
        case GLFW_KEY_F:            input->keyboard_input[KEY_F] = is_pressed; return;
        case GLFW_KEY_G:            input->keyboard_input[KEY_G] = is_pressed; return;
        case GLFW_KEY_H:            input->keyboard_input[KEY_H] = is_pressed; return;
        case GLFW_KEY_I:            input->keyboard_input[KEY_I] = is_pressed; return;
        case GLFW_KEY_J:            input->keyboard_input[KEY_J] = is_pressed; return;
        case GLFW_KEY_K:            input->keyboard_input[KEY_K] = is_pressed; return;
        case GLFW_KEY_L:            input->keyboard_input[KEY_L] = is_pressed; return;
        case GLFW_KEY_M:            input->keyboard_input[KEY_M] = is_pressed; return;
        case GLFW_KEY_N:            input->keyboard_input[KEY_N] = is_pressed; return;
        case GLFW_KEY_O:            input->keyboard_input[KEY_O] = is_pressed; return;
        case GLFW_KEY_P:            input->keyboard_input[KEY_P] = is_pressed; return;
        case GLFW_KEY_Q:            input->keyboard_input[KEY_Q] = is_pressed; return;
        case GLFW_KEY_R:            input->keyboard_input[KEY_R] = is_pressed; return;
        case GLFW_KEY_S:            input->keyboard_input[KEY_S] = is_pressed; return;
        case GLFW_KEY_T:            input->keyboard_input[KEY_T] = is_pressed; return;
        case GLFW_KEY_U:            input->keyboard_input[KEY_U] = is_pressed; return;
        case GLFW_KEY_V:            input->keyboard_input[KEY_V] = is_pressed; return;
        case GLFW_KEY_W:            input->keyboard_input[KEY_W] = is_pressed; return;
        case GLFW_KEY_X:            input->keyboard_input[KEY_X] = is_pressed; return;
        case GLFW_KEY_Y:            input->keyboard_input[KEY_Y] = is_pressed; return;
        case GLFW_KEY_Z:            input->keyboard_input[KEY_Z] = is_pressed; return;
        case GLFW_KEY_LEFT_BRACKET: input->keyboard_input[KEY_LEFT_BRACKET] = is_pressed; return;
        case GLFW_KEY_BACKSLASH:    input->keyboard_input[KEY_BACKSLASH] = is_pressed; return;
        case GLFW_KEY_RIGHT_BRACKET:input->keyboard_input[KEY_RIGHT_BRACKET] = is_pressed; return;
        case GLFW_KEY_GRAVE_ACCENT: input->keyboard_input[KEY_GRAVE_ACCENT] = is_pressed; return;
        case GLFW_KEY_WORLD_1:      input->keyboard_input[KEY_WORLD_1] = is_pressed; return;
        case GLFW_KEY_WORLD_2:      input->keyboard_input[KEY_WORLD_2] = is_pressed; return;
        case GLFW_KEY_ESCAPE:       input->keyboard_input[KEY_ESCAPE] = is_pressed; return;
        case GLFW_KEY_ENTER:        input->keyboard_input[KEY_ENTER] = is_pressed; return;
        case GLFW_KEY_TAB:          input->keyboard_input[KEY_TAB] = is_pressed; return;
        case GLFW_KEY_BACKSPACE:    input->keyboard_input[KEY_BACKSPACE] = is_pressed; return;
        case GLFW_KEY_INSERT:       input->keyboard_input[KEY_INSERT] = is_pressed; return;
        case GLFW_KEY_DELETE:       input->keyboard_input[KEY_DELETE] = is_pressed; return;
        case GLFW_KEY_RIGHT:        input->keyboard_input[KEY_RIGHT] = is_pressed; return;
        case GLFW_KEY_LEFT:         input->keyboard_input[KEY_LEFT] = is_pressed; return;
        case GLFW_KEY_DOWN:         input->keyboard_input[KEY_DOWN] = is_pressed; return;
        case GLFW_KEY_UP:           input->keyboard_input[KEY_UP] = is_pressed; return;
        case GLFW_KEY_PAGE_UP:      input->keyboard_input[KEY_PAGE_UP] = is_pressed; return;
        case GLFW_KEY_PAGE_DOWN:    input->keyboard_input[KEY_PAGE_DOWN] = is_pressed; return;
        case GLFW_KEY_HOME:         input->keyboard_input[KEY_HOME] = is_pressed; return;
        case GLFW_KEY_END:          input->keyboard_input[KEY_END] = is_pressed; return;
        case GLFW_KEY_CAPS_LOCK:    input->keyboard_input[KEY_CAPS_LOCK] = is_pressed; return;
        case GLFW_KEY_SCROLL_LOCK:  input->keyboard_input[KEY_SCROLL_LOCK] = is_pressed; return;
        case GLFW_KEY_NUM_LOCK:     input->keyboard_input[KEY_NUM_LOCK] = is_pressed; return;
        case GLFW_KEY_PRINT_SCREEN: input->keyboard_input[KEY_PRINT_SCREEN] = is_pressed; return;
        case GLFW_KEY_PAUSE:        input->keyboard_input[KEY_PAUSE] = is_pressed; return;
        case GLFW_KEY_F1:           input->keyboard_input[KEY_F1] = is_pressed; return;
        case GLFW_KEY_F2:           input->keyboard_input[KEY_F2] = is_pressed; return;
        case GLFW_KEY_F3:           input->keyboard_input[KEY_F3] = is_pressed; return;
        case GLFW_KEY_F4:           input->keyboard_input[KEY_F4] = is_pressed; return;
        case GLFW_KEY_F5:           input->keyboard_input[KEY_F5] = is_pressed; return;
        case GLFW_KEY_F6:           input->keyboard_input[KEY_F6] = is_pressed; return;
        case GLFW_KEY_F7:           input->keyboard_input[KEY_F7] = is_pressed; return;
        case GLFW_KEY_F8:           input->keyboard_input[KEY_F8] = is_pressed; return;
        case GLFW_KEY_F9:           input->keyboard_input[KEY_F9] = is_pressed; return;
        case GLFW_KEY_F10:          input->keyboard_input[KEY_F10] = is_pressed; return;
        case GLFW_KEY_F11:          input->keyboard_input[KEY_F11] = is_pressed; return;
        case GLFW_KEY_F12:          input->keyboard_input[KEY_F12] = is_pressed; return;
        case GLFW_KEY_KP_0:         input->keyboard_input[KEY_NUMPAD_0] = is_pressed; return;
        case GLFW_KEY_KP_1:         input->keyboard_input[KEY_NUMPAD_1] = is_pressed; return;
        case GLFW_KEY_KP_2:         input->keyboard_input[KEY_NUMPAD_2] = is_pressed; return;
        case GLFW_KEY_KP_3:         input->keyboard_input[KEY_NUMPAD_3] = is_pressed; return;
        case GLFW_KEY_KP_4:         input->keyboard_input[KEY_NUMPAD_4] = is_pressed; return;
        case GLFW_KEY_KP_5:         input->keyboard_input[KEY_NUMPAD_5] = is_pressed; return;
        case GLFW_KEY_KP_6:         input->keyboard_input[KEY_NUMPAD_6] = is_pressed; return;
        case GLFW_KEY_KP_7:         input->keyboard_input[KEY_NUMPAD_7] = is_pressed; return;
        case GLFW_KEY_KP_8:         input->keyboard_input[KEY_NUMPAD_8] = is_pressed; return;
        case GLFW_KEY_KP_9:         input->keyboard_input[KEY_NUMPAD_9] = is_pressed; return;
        case GLFW_KEY_KP_DECIMAL:   input->keyboard_input[KEY_NUMPAD_DECIMAL] = is_pressed; return;
        case GLFW_KEY_KP_DIVIDE:    input->keyboard_input[KEY_NUMPAD_DIVIDE] = is_pressed; return;
        case GLFW_KEY_KP_MULTIPLY:  input->keyboard_input[KEY_NUMPAD_MULTIPLY] = is_pressed; return;
        case GLFW_KEY_KP_SUBTRACT:  input->keyboard_input[KEY_NUMPAD_SUBTRACT] = is_pressed; return;
        case GLFW_KEY_KP_ADD:       input->keyboard_input[KEY_NUMPAD_ADD] = is_pressed; return;
        case GLFW_KEY_KP_ENTER:     input->keyboard_input[KEY_NUMPAD_ENTER] = is_pressed; return;
        case GLFW_KEY_KP_EQUAL:     input->keyboard_input[KEY_NUMPAD_EQUAL] = is_pressed; return;
        case GLFW_KEY_LEFT_SHIFT:   input->keyboard_input[KEY_LEFT_SHIFT] = is_pressed; return;
        case GLFW_KEY_LEFT_CONTROL: input->keyboard_input[KEY_LEFT_CONTROL] = is_pressed; return;
        case GLFW_KEY_LEFT_ALT:     input->keyboard_input[KEY_LEFT_ALT] = is_pressed; return;
        case GLFW_KEY_LEFT_SUPER:   input->keyboard_input[KEY_LEFT_SUPER] = is_pressed; return;
        case GLFW_KEY_RIGHT_SHIFT:  input->keyboard_input[KEY_RIGHT_SHIFT] = is_pressed; return;
        case GLFW_KEY_RIGHT_CONTROL:input->keyboard_input[KEY_RIGHT_CONTROL] = is_pressed; return;
        case GLFW_KEY_RIGHT_ALT:    input->keyboard_input[KEY_RIGHT_ALT] = is_pressed; return;
        case GLFW_KEY_RIGHT_SUPER:  input->keyboard_input[KEY_RIGHT_SUPER] = is_pressed; return;
        case GLFW_KEY_MENU:         input->keyboard_input[KEY_MENU] = is_pressed; return;
        default:                    input->keyboard_input[KEY_UNKNOWN] = is_pressed; return;;
    }
}

//Mouse stuff

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    RawInput* input = platform_data->input;

    bool is_pressed = (action != GLFW_RELEASE);
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT:  input->mouse.click_left = is_pressed;  return;
        case GLFW_MOUSE_BUTTON_RIGHT: input->mouse.click_right = is_pressed; return;
        default: return;
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos){
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    RawInput* input = platform_data->input;

    input->mouse.xpos = xpos;
    input->mouse.ypos = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    RawInput* input = platform_data->input;

    input->mouse.xscroll = xoffset;
    input->mouse.yscroll = yoffset;
}

void cursor_enter_callback(GLFWwindow* window, int entered){
	PlatformData* platform_data = (PlatformData*)glfwGetWindowUserPointer(window);
    RawInput* input = platform_data->input;

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
        controller->controller_type = CONTROLLER_TYPE_UNKNOWN;
        printf("Warning: Unknown joystick %s\n", joystick_name);
        return;
    }
}

void poll_joystick(RawInput* input)
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
}

void process_raw_input(RawInput* raw_input, GameInput* game_input)
{
    //Denoise analogue sticks
    for(int i=0; i<4; ++i){
        if(fabsf(raw_input->controller.axis[i])<0.1) raw_input->controller.axis[i] = 0;
    }
    
    // TODO: Clever way of deciding whether to prioritise controller or keyboard
    game_input->move_input[MOVE_FORWARD]   = CLAMP( raw_input->controller.axis[XBOX_LEFT_STICK_VERT], 0, 1);
    game_input->move_input[MOVE_LEFT]      = CLAMP(-raw_input->controller.axis[XBOX_LEFT_STICK_HOR], 0, 1);
    game_input->move_input[MOVE_BACK]      = CLAMP(-raw_input->controller.axis[XBOX_LEFT_STICK_VERT], 0, 1);
    game_input->move_input[MOVE_RIGHT]     = CLAMP( raw_input->controller.axis[XBOX_LEFT_STICK_HOR], 0, 1);
    game_input->move_input[TILT_CAM_UP]    = CLAMP( raw_input->controller.axis[XBOX_RIGHT_STICK_VERT], 0, 1);
    game_input->move_input[TILT_CAM_DOWN]  = CLAMP(-raw_input->controller.axis[XBOX_RIGHT_STICK_VERT], 0, 1);
    game_input->move_input[TURN_CAM_LEFT]  = CLAMP(-raw_input->controller.axis[XBOX_RIGHT_STICK_HOR], 0, 1);
    game_input->move_input[TURN_CAM_RIGHT] = CLAMP( raw_input->controller.axis[XBOX_RIGHT_STICK_HOR], 0, 1);
    game_input->button_input[JUMP]         = raw_input->controller.button[XBOX_BUTTON_A];
    game_input->button_input[RAISE_CAM]    = raw_input->controller.button[XBOX_BUTTON_RB];
    game_input->button_input[LOWER_CAM]    = raw_input->controller.button[XBOX_BUTTON_LB];

    game_input->move_input[MOVE_FORWARD]   = raw_input->keyboard_input[KEY_W];
    game_input->move_input[MOVE_LEFT]      = raw_input->keyboard_input[KEY_A];
    game_input->move_input[MOVE_BACK]      = raw_input->keyboard_input[KEY_S];
    game_input->move_input[MOVE_RIGHT]     = raw_input->keyboard_input[KEY_D];
    game_input->move_input[TILT_CAM_UP]    = raw_input->keyboard_input[KEY_UP];
    game_input->move_input[TILT_CAM_DOWN]  = raw_input->keyboard_input[KEY_DOWN];
    game_input->move_input[TURN_CAM_LEFT]  = raw_input->keyboard_input[KEY_LEFT];
    game_input->move_input[TURN_CAM_RIGHT] = raw_input->keyboard_input[KEY_RIGHT];
    game_input->button_input[JUMP]         = raw_input->keyboard_input[KEY_SPACE];
    game_input->button_input[RAISE_CAM]    = raw_input->keyboard_input[KEY_E];
    game_input->button_input[LOWER_CAM]    = raw_input->keyboard_input[KEY_Q];
}
