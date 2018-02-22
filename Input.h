#pragma once
//Kevin's Input Layer using GLFW 

//List of all possible commands in the game!
enum INPUT_MOVEMENT{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FORWARD,
    MOVE_BACK,
    TILT_CAM_DOWN,
    TILT_CAM_UP,
    TURN_CAM_LEFT,
    TURN_CAM_RIGHT,
    NUM_MOVEMENTS
};
enum INPUT_COMMANDS{
    JUMP,
    RAISE_CAM,
    LOWER_CAM,
    NUM_INPUT_COMMANDS
};

const float MOUSE_DEFAULT_SENSITIVITY = 0.2f;
struct Mouse {
    bool click_left;
    bool click_right;
    double xpos, ypos;
    double prev_xpos, prev_ypos;
    double xscroll, yscroll;
    float sensitivity;
    bool is_in_window;
};
extern Mouse g_mouse;

//Global input state for game code to query, e.g.   if(g_input[MOVE_LEFT]) move_left();
extern float g_move_input[NUM_MOVEMENTS];
extern bool g_input[NUM_INPUT_COMMANDS];

//For custom user key mappings (e.g.  g_key_mapping[DASH_MOVE] returns GLFW_KEY_ENTER)
//extern int g_key_mapping[NUM_INPUT_COMMANDS];

//Use Command instead of Control on Mac
#ifdef __APPLE__
#define CTRL_KEY_LEFT GLFW_KEY_LEFT_SUPER
#define CTRL_KEY_RIGHT GLFW_KEY_RIGHT_SUPER
#else
#define CTRL_KEY_LEFT GLFW_KEY_LEFT_CONTROL
#define CTRL_KEY_RIGHT GLFW_KEY_RIGHT_CONTROL
#endif

struct GLFWwindow;

//glfwSetKeyCallback(window, key_callback);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Mouse stuff

//glfwSetMouseButtonCallback(window, mouse_button_callback);
//Or poll with: if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)))
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//glfwSetCursorPosCallback(window, cursor_pos_callback);
//Just poll with glfwGetCursorPos(window, &xpos, &ypos);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);

//glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered);

//Joystick stuff

enum XBOX_CONTROLLER_AXES {
    XBOX_LEFT_STICK_HOR,
    XBOX_LEFT_STICK_VERT,
    XBOX_RIGHT_STICK_HOR,
    XBOX_RIGHT_STICK_VERT,
    XBOX_LT,
    XBOX_RT,
    XBOX_CONTROLLER_NUM_AXES
};

enum XBOX_polled_button_values {
    XBOX_BUTTON_A,
    XBOX_BUTTON_B,
    XBOX_BUTTON_X,
    XBOX_BUTTON_Y,
    XBOX_BUTTON_LB,
    XBOX_BUTTON_RB,
    XBOX_BUTTON_BACK,
    XBOX_BUTTON_START,
    XBOX_BUTTON_LS,
    XBOX_BUTTON_RS,
    XBOX_BUTTON_DPAD_UP,
    XBOX_BUTTON_DPAD_RIGHT,
    XBOX_BUTTON_DPAD_DOWN,
    XBOX_BUTTON_DPAD_LEFT,
    XBOX_CONTROLLER_NUM_BUTTONS
};

enum PS4_CONTROLLER_AXES {
    PS4_LEFT_STICK_HOR,
    PS4_LEFT_STICK_VERT,
    PS4_RIGHT_STICK_HOR,
    PS4_L2,
    PS4_R2,
    PS4_RIGHT_STICK_VERT,    
    PS4_NUM_CONTROLLER_AXES
};

enum PS4_polled_button_values {
    PS4_BUTTON_SQUARE,
    PS4_BUTTON_CROSS,
    PS4_BUTTON_CIRCLE,
    PS4_BUTTON_TRIANGLE,
    PS4_BUTTON_L1,
    PS4_BUTTON_R1,
    PS4_BUTTON_L2,
    PS4_BUTTON_R2,
    PS4_BUTTON_SHARE,
    PS4_BUTTON_OPTIONS,
    PS4_BUTTON_L3,
    PS4_BUTTON_R3,
    PS4_BUTTON_HOME,
    PS4_BUTTON_TOUCHPAD,
    PS4_BUTTON_DPAD_UP,
    PS4_BUTTON_DPAD_RIGHT,
    PS4_BUTTON_DPAD_DOWN,
    PS4_BUTTON_DPAD_LEFT
};

enum CONTROLLER_TYPE {
    CONTROLLER_TYPE_XBOX,
    CONTROLLER_TYPE_PS4
};

struct ControllerState {
    float axis[XBOX_CONTROLLER_NUM_AXES];
    bool button[XBOX_CONTROLLER_NUM_BUTTONS];
    bool is_connected;
    CONTROLLER_TYPE controller_type;
};
extern ControllerState g_controller;

void init_joystick();

void poll_joystick();

//glfwSetJoystickCallback(joystick_callback);
void joystick_callback(int joy, int event);


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
    for(int i = 0;  i < count;  ++i) handle_dropped_file(paths[i]);
}
*/
