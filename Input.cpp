#include "Input.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

#include "GameMaths.h"

Mouse g_mouse = {};

//Global input state for game code to query, e.g.   if(g_input[MOVE_LEFT]) move_left();
float g_move_input[NUM_MOVEMENTS] = {0};
bool g_input[NUM_INPUT_COMMANDS] = {0};

//For custom user key mappings (e.g.  g_key_mapping[DASH_MOVE] returns GLFW_KEY_ENTER)
//int g_key_mapping[NUM_INPUT_COMMANDS];

//glfwSetKeyCallback(window, key_callback);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //Immutable input keys:
    bool is_pressed = (action != GLFW_RELEASE);
    switch(key){
        case GLFW_KEY_W:        g_move_input[MOVE_FORWARD]   = is_pressed; return;
        case GLFW_KEY_A:        g_move_input[MOVE_LEFT]      = is_pressed; return;
        case GLFW_KEY_S:        g_move_input[MOVE_BACK]      = is_pressed; return;
        case GLFW_KEY_D:        g_move_input[MOVE_RIGHT]     = is_pressed; return;
        case GLFW_KEY_UP:       g_move_input[TILT_CAM_UP]    = is_pressed; return;
        case GLFW_KEY_DOWN:     g_move_input[TILT_CAM_DOWN]  = is_pressed; return;
        case GLFW_KEY_LEFT:     g_move_input[TURN_CAM_LEFT]  = is_pressed; return;
        case GLFW_KEY_RIGHT:    g_move_input[TURN_CAM_RIGHT] = is_pressed; return;
        case GLFW_KEY_SPACE:    g_input[JUMP]           = is_pressed; return;
        case GLFW_KEY_Q:        g_input[RAISE_CAM]      = is_pressed; return;
        case GLFW_KEY_E:        g_input[LOWER_CAM]      = is_pressed; return;
        default: break;
    }
    //Custom key mapping:
    // if(key == g_key_mapping[DASH_MOVE]){  g_input[DASH_MOVE] = is_pressed; return; }
    // if(key == g_key_mapping[JUMP_MOVE]){  g_input[JUMP_MOVE] = is_pressed; return; }
}

//Mouse stuff

//glfwSetMouseButtonCallback(window, mouse_button_callback);
//Or poll with: if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)))
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    bool is_pressed = (action != GLFW_RELEASE);
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT:  g_mouse.click_left = is_pressed;  return;
        case GLFW_MOUSE_BUTTON_RIGHT: g_mouse.click_right = is_pressed; return;
        default:
            printf("How the heck did we get here?\n");
            printf("Mouse button: %d\n", button);
    }
}

//glfwSetCursorPosCallback(window, cursor_pos_callback);
//Just poll with glfwGetCursorPos(window, &xpos, &ypos);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos){
    g_mouse.xpos = xpos;
    g_mouse.ypos = ypos;
}

//glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    g_mouse.xscroll = xoffset;
    g_mouse.yscroll = yoffset;
}

//glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered){
    g_mouse.is_in_window = entered;
}

//Joystick stuff

ControllerState g_controller = {};

void init_joystick(){
    if(!glfwJoystickPresent(GLFW_JOYSTICK_1)){
        g_controller.is_connected = false;        
        return;
    }

    g_controller.is_connected = true;
    
    const char* joystick_name = glfwGetJoystickName(GLFW_JOYSTICK_1);

    if(strcmp(joystick_name, "XBox 360 Controller")==0) 
        g_controller.controller_type = CONTROLLER_TYPE_XBOX;
    else if(strcmp(joystick_name, "Wireless Controller")==0) 
        g_controller.controller_type = CONTROLLER_TYPE_PS4;
    else {
        printf("Warning: Unknown joystick %s\n", joystick_name);
        return;
    }
}

void poll_joystick(){
    if(!g_controller.is_connected) return;

    int axis_count;
    const float* polled_axis_values = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
    int button_count;
    const unsigned char* polled_button_values = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);

    //XBox layout is default, just copy values
    if(g_controller.controller_type == CONTROLLER_TYPE_XBOX){
        for(int i=0; i<axis_count; i++){
            g_controller.axis[i] = polled_axis_values[i];
        }

        for(int i=0; i<button_count; i++){
            g_controller.button[i] = (bool)polled_button_values[i];
        }
    }
    //Convert PS4 input to XBox input
    else if(g_controller.controller_type == CONTROLLER_TYPE_PS4){
        g_controller.axis[XBOX_LEFT_STICK_HOR]   = polled_axis_values[PS4_LEFT_STICK_HOR];
        g_controller.axis[XBOX_LEFT_STICK_VERT]  = -polled_axis_values[PS4_LEFT_STICK_VERT];
        g_controller.axis[XBOX_RIGHT_STICK_HOR]  = polled_axis_values[PS4_RIGHT_STICK_HOR];
        g_controller.axis[XBOX_RIGHT_STICK_VERT] = -polled_axis_values[PS4_RIGHT_STICK_VERT];
        g_controller.axis[XBOX_LT]               = polled_axis_values[PS4_L2];
        g_controller.axis[XBOX_RT]               = polled_axis_values[PS4_R2];

        g_controller.button[XBOX_BUTTON_A]          = polled_button_values[PS4_BUTTON_CROSS];
        g_controller.button[XBOX_BUTTON_B]          = polled_button_values[PS4_BUTTON_CIRCLE];
        g_controller.button[XBOX_BUTTON_X]          = polled_button_values[PS4_BUTTON_SQUARE];
        g_controller.button[XBOX_BUTTON_Y]          = polled_button_values[PS4_BUTTON_TRIANGLE];
        g_controller.button[XBOX_BUTTON_LB]         = polled_button_values[PS4_BUTTON_L1];
        g_controller.button[XBOX_BUTTON_RB]         = polled_button_values[PS4_BUTTON_R1];
        g_controller.button[XBOX_BUTTON_BACK]       = polled_button_values[PS4_BUTTON_SHARE];
        g_controller.button[XBOX_BUTTON_START]      = polled_button_values[PS4_BUTTON_OPTIONS];
        g_controller.button[XBOX_BUTTON_LS]         = polled_button_values[PS4_BUTTON_L3];
        g_controller.button[XBOX_BUTTON_RS]         = polled_button_values[PS4_BUTTON_R3];
        g_controller.button[XBOX_BUTTON_DPAD_UP]    = polled_button_values[PS4_BUTTON_DPAD_UP];
        g_controller.button[XBOX_BUTTON_DPAD_RIGHT] = polled_button_values[PS4_BUTTON_DPAD_RIGHT];
        g_controller.button[XBOX_BUTTON_DPAD_DOWN]  = polled_button_values[PS4_BUTTON_DPAD_DOWN];
        g_controller.button[XBOX_BUTTON_DPAD_LEFT]  = polled_button_values[PS4_BUTTON_DPAD_LEFT];
    }

    //Denoise analogue sticks
    for(int i=0; i<4; i++){
        if(fabsf(g_controller.axis[i])<0.1) g_controller.axis[i] = 0;
    }
    
    g_move_input[MOVE_LEFT]    = CLAMP(-g_controller.axis[XBOX_LEFT_STICK_HOR], 0, 1);
    g_move_input[MOVE_RIGHT]   = CLAMP( g_controller.axis[XBOX_LEFT_STICK_HOR], 0, 1);
    g_move_input[MOVE_FORWARD] = CLAMP( g_controller.axis[XBOX_LEFT_STICK_VERT], 0, 1);
    g_move_input[MOVE_BACK]    = CLAMP(-g_controller.axis[XBOX_LEFT_STICK_VERT], 0, 1);
    g_move_input[TURN_CAM_LEFT]  = CLAMP(-g_controller.axis[XBOX_RIGHT_STICK_HOR], 0, 1);
    g_move_input[TURN_CAM_RIGHT] = CLAMP( g_controller.axis[XBOX_RIGHT_STICK_HOR], 0, 1);
    g_move_input[TILT_CAM_DOWN]  = CLAMP( g_controller.axis[XBOX_RIGHT_STICK_VERT], 0, 1);
    g_move_input[TILT_CAM_UP]    = CLAMP(-g_controller.axis[XBOX_RIGHT_STICK_VERT], 0, 1);

    g_input[JUMP] = g_controller.button[XBOX_BUTTON_A];
}

//glfwSetJoystickCallback(joystick_callback);
void joystick_callback(int joy, int event){
    if(joy != GLFW_JOYSTICK_1) return;
    init_joystick();
}


//Clipboard Stuff
/* //COPY
glfwSetClipboardString(window, "A string with words in it");

//PASTE
const char* text = glfwGetClipboardString(window);
if(text) insert_text(text);
*/

/* //File/Path Drop
//glfwSetDropCallback(window, drop_callback);
void drop_callback(GLFWwindow* window, int count, const char** paths){
    for(int i = 0;  i < count;  i++) handle_dropped_file(paths[i]);
}
*/
