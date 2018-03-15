#pragma once
//Kevin's Input Layer using GLFW 

//List of all possible commands in the game!
enum MOVE_INPUT {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FORWARD,
    MOVE_BACK,
    TILT_CAM_DOWN,
    TILT_CAM_UP,
    TURN_CAM_LEFT,
    TURN_CAM_RIGHT,
    NUM_MOVE_INPUTS
};
enum BUTTON_INPUT {
    JUMP,
    RAISE_CAM,
    LOWER_CAM,
    NUM_BUTTON_INPUTS
};

enum KEYBOARD_KEYS {
    KEY_UNKNOWN = 0,
    KEY_SPACE,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_BRACKET, // [
    KEY_BACKSLASH,
    KEY_RIGHT_BRACKET, // ]
    KEY_GRAVE_ACCENT, // `
    KEY_WORLD_1,
    KEY_WORLD_2,

    /* Function keys */
    KEY_ESCAPE, 
    KEY_ENTER, 
    KEY_TAB, 
    KEY_BACKSPACE, 
    KEY_INSERT, 
    KEY_DELETE, 
    KEY_RIGHT, 
    KEY_LEFT, 
    KEY_DOWN, 
    KEY_UP, 
    KEY_PAGE_UP, 
    KEY_PAGE_DOWN, 
    KEY_HOME, 
    KEY_END, 
    KEY_CAPS_LOCK, 
    KEY_SCROLL_LOCK, 
    KEY_NUM_LOCK, 
    KEY_PRINT_SCREEN, 
    KEY_PAUSE, 
    KEY_F1, 
    KEY_F2, 
    KEY_F3, 
    KEY_F4, 
    KEY_F5, 
    KEY_F6, 
    KEY_F7, 
    KEY_F8, 
    KEY_F9, 
    KEY_F10, 
    KEY_F11, 
    KEY_F12,
    KEY_NUMPAD_0, 
    KEY_NUMPAD_1, 
    KEY_NUMPAD_2, 
    KEY_NUMPAD_3, 
    KEY_NUMPAD_4, 
    KEY_NUMPAD_5, 
    KEY_NUMPAD_6, 
    KEY_NUMPAD_7, 
    KEY_NUMPAD_8, 
    KEY_NUMPAD_9, 
    KEY_NUMPAD_DECIMAL, 
    KEY_NUMPAD_DIVIDE, 
    KEY_NUMPAD_MULTIPLY, 
    KEY_NUMPAD_SUBTRACT, 
    KEY_NUMPAD_ADD, 
    KEY_NUMPAD_ENTER, 
    KEY_NUMPAD_EQUAL, 
    KEY_LEFT_SHIFT, 
    KEY_LEFT_CONTROL, 
    KEY_LEFT_ALT, 
    KEY_LEFT_SUPER, 
    KEY_RIGHT_SHIFT, 
    KEY_RIGHT_CONTROL, 
    KEY_RIGHT_ALT, 
    KEY_RIGHT_SUPER, 
    KEY_MENU, 
    KEYBOARD_NUM_KEYS
};

struct Mouse {
    bool click_left;
    bool click_right;
    double xpos, ypos;
    double xscroll, yscroll;
    bool is_in_window;
};

enum XBOX_CONTROLLER_AXES {
    XBOX_LEFT_STICK_HOR,
    XBOX_LEFT_STICK_VERT,
    XBOX_RIGHT_STICK_HOR,
    XBOX_RIGHT_STICK_VERT,
    XBOX_LT,
    XBOX_RT,
    XBOX_CONTROLLER_NUM_AXES
};

enum XBOX_BUTTONS {
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

enum PS4_BUTTONS {
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
    PS4_BUTTON_DPAD_LEFT,
    PS4_CONTROLLER_NUM_BUTTONS
};

enum CONTROLLER_TYPE {
    CONTROLLER_TYPE_UNKNOWN,
    CONTROLLER_TYPE_XBOX,
    CONTROLLER_TYPE_PS4
};

struct ControllerState {
    CONTROLLER_TYPE type;
    bool is_initialised;
    float axis[XBOX_CONTROLLER_NUM_AXES];
    bool button[XBOX_CONTROLLER_NUM_BUTTONS];
};

struct RawInput {
    bool use_controller;
    Mouse mouse;
    bool keyboard_input[KEYBOARD_NUM_KEYS];
    ControllerState controller;
};

struct GameInput {
    float move_input[NUM_MOVE_INPUTS];
    bool is_down[NUM_BUTTON_INPUTS];
    bool was_down[NUM_BUTTON_INPUTS];
};

//Use Command instead of Control on Mac
#ifdef __APPLE__
#define CTRL_KEY_LEFT GLFW_KEY_LEFT_SUPER
#define CTRL_KEY_RIGHT GLFW_KEY_RIGHT_SUPER
#else
#define CTRL_KEY_LEFT GLFW_KEY_LEFT_CONTROL
#define CTRL_KEY_RIGHT GLFW_KEY_RIGHT_CONTROL
#endif

struct GLFWwindow;
struct PlatformData;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Mouse callbacks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_enter_callback(GLFWwindow* window, int entered);

//Joystick functions
void init_joystick(ControllerState* controller);
void poll_joystick(RawInput* input);
