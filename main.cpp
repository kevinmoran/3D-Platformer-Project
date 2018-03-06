#define GL_LITE_IMPLEMENTATION
#include "gl_lite.h"
#undef GL_LITE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include "init_gl.h"
#include "GameMaths.h"
#include "Input.h"
#include "Camera3D.h"
#include "Shader.h"
#include "Player.h"
#include "load_obj.h"
#include "DebugDrawing.h"

#include "Input.cpp"
#include "Camera3D.cpp"
#include "Shader.cpp"
#include "Player.cpp"
#include "load_obj.cpp"
#include "DebugDrawing.cpp"
#include "string_functions.cpp"

int main(){
	GLFWwindow* window = NULL;
	WindowData window_data = {};
	window_data.handle = &window;
	window_data.width = 400;
	window_data.height = 300;
	window_data.aspect_ratio = (float)window_data.width/(float)window_data.height;

	RawInput raw_input[2] = {};

	PlatformData platform_data = {&window_data, &raw_input[0], &raw_input[1]};

	if(!init_gl(&platform_data, "3D Platformer")){ return 1; }

	//Load player mesh
	GLuint player_vao;
	unsigned int player_num_indices = 0;
	{
		float* vp = NULL;
		float* vn = NULL;
		float* vt = NULL;
		uint16_t* indices = NULL;
		unsigned int num_verts = 0;
		load_obj_indexed("capsule.obj", &vp, &vt, &vn, &indices, &num_verts, &player_num_indices);

		glGenVertexArrays(1, &player_vao);
		glBindVertexArray(player_vao);
		
		GLuint points_vbo;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_verts*3*sizeof(float), vp, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VP_ATTRIB_LOC);
		glVertexAttribPointer(VP_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		free(vp);

		free(vt);

		GLuint normals_vbo;
		glGenBuffers(1, &normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_verts*3*sizeof(float), vn, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VN_ATTRIB_LOC);
		glVertexAttribPointer(VN_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		free(vn);

		GLuint index_vbo;
		glGenBuffers(1, &index_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, player_num_indices*sizeof(uint16_t), indices, GL_STATIC_DRAW);
		free(indices);
	}

	//Load cube mesh
	GLuint cube_vao;
	unsigned int cube_num_indices = 0;
	{
		float* vp = NULL;
		float* vn = NULL;
		float* vt = NULL;
		uint16_t* indices = NULL;
		unsigned int num_verts = 0;
		load_obj_indexed("cube.obj", &vp, &vt, &vn, &indices, &num_verts, &cube_num_indices);

		glGenVertexArrays(1, &cube_vao);
		glBindVertexArray(cube_vao);
		
		GLuint points_vbo;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_verts*3*sizeof(float), vp, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VP_ATTRIB_LOC);
		glVertexAttribPointer(VP_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		free(vp);

		free(vt);

		GLuint normals_vbo;
		glGenBuffers(1, &normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
		glBufferData(GL_ARRAY_BUFFER, num_verts*3*sizeof(float), vn, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VN_ATTRIB_LOC);
		glVertexAttribPointer(VN_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		free(vn);

		GLuint index_vbo;
		glGenBuffers(1, &index_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_num_indices*sizeof(uint16_t), indices, GL_STATIC_DRAW);
		free(indices);
	}

	Camera3D camera = {};
	init_camera(&camera, vec3{0,2,5}, vec3{0,0,0});
	bool freecam_mode = false;

	Player player;
	init_player(&player);
	
	DebugDraw debug_draw_data;
	init_debug_draw(&debug_draw_data);

    //Load shaders
	Shader basic_shader = init_shader("MVP.vert", "uniform_colour_sunlight.frag");
	GLuint colour_loc = glGetUniformLocation(basic_shader.id, "colour");

	check_gl_error();

    double curr_time = glfwGetTime();
	//-------------------------------------------------------------------------------------//
	//-------------------------------------MAIN LOOP---------------------------------------//
	//-------------------------------------------------------------------------------------//
	while(!glfwWindowShouldClose(window))
	{
		//Get dt
		double prev_time = curr_time;
		curr_time = glfwGetTime();
		double dt = curr_time - prev_time;
		if(dt > 0.1) dt = 0.1;
		
		//Get Input
		copy_memory(platform_data.old_input, platform_data.new_input, sizeof(RawInput));
		glfwPollEvents();

		{
			vec2 new_mouse_pos = {(f32)platform_data.new_input->mouse.xpos, (f32)platform_data.new_input->mouse.ypos};
			vec2 old_mouse_pos = {(f32)platform_data.old_input->mouse.xpos, (f32)platform_data.old_input->mouse.ypos};

			//TODO: this threshold is completely ad-hoc, check it
			const float MOUSE_MOVED_INTENTIONALLY_THRESHOLD = 25.0f;
			if(length2(new_mouse_pos - old_mouse_pos) > MOUSE_MOVED_INTENTIONALLY_THRESHOLD) 
				platform_data.new_input->use_controller = false;
		}

		poll_joystick(platform_data.new_input);

		RawInput* new_input = platform_data.new_input;
		RawInput* old_input = platform_data.old_input;

		if(new_input->use_controller != old_input->use_controller)
			printf(new_input->use_controller ? "Use Controller\n" : "Don't Use Controller\n");
		
		//Check miscellaneous button presses
		{
			if(new_input->keyboard_input[KEY_ESCAPE]) {
				glfwSetWindowShouldClose(window, 1);
			}

			//Tab to toggle player_cam/freecam
			if(new_input->keyboard_input[KEY_TAB] && !old_input->keyboard_input[KEY_TAB]) {
				freecam_mode = !freecam_mode;
			}

			//M to toggle between mouse/arrow key controls for camera
			if(new_input->keyboard_input[KEY_M] && !old_input->keyboard_input[KEY_M]) {
				camera.use_mouse_controls = !camera.use_mouse_controls;
			}

			//Ctrl/Command-F to toggle fullscreen
			//Note: window_resize_callback takes care of resizing viewport
			if(new_input->keyboard_input[KEY_F] && !old_input->keyboard_input[KEY_F])
			{
				//TODO: Handle Control/Command switcheroo with RawInput
				if(glfwGetKey(window, CTRL_KEY_LEFT) || glfwGetKey(window, CTRL_KEY_RIGHT))
				{
					window_data.is_fullscreen = !window_data.is_fullscreen;
					static int old_win_x, old_win_y, old_win_w, old_win_h;
					if(window_data.is_fullscreen)
					{
						glfwGetWindowPos(window, &old_win_x, &old_win_y);
						glfwGetWindowSize(window, &old_win_w, &old_win_h);
						GLFWmonitor* mon = glfwGetPrimaryMonitor();
						const GLFWvidmode* vidMode = glfwGetVideoMode(mon);
						glfwSetWindowMonitor(window, mon, 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
					}
					else glfwSetWindowMonitor(window, NULL, old_win_x, old_win_y, old_win_w, old_win_h, GLFW_DONT_CARE);
				}
			}
		}

		GameInput game_input = {};
		//Process raw input
		{
			ControllerState* controller = &new_input->controller;
			ControllerState* old_controller = &old_input->controller;

			if(new_input->use_controller)
			{
				//Denoise analogue sticks
				for(int i=0; i<4; ++i){
					if(fabsf(controller->axis[i])<0.1) controller->axis[i] = 0;
				}
				
				game_input.move_input[MOVE_FORWARD]   = CLAMP( controller->axis[XBOX_LEFT_STICK_VERT], 0, 1);
				game_input.move_input[MOVE_LEFT]      = CLAMP(-controller->axis[XBOX_LEFT_STICK_HOR], 0, 1);
				game_input.move_input[MOVE_BACK]      = CLAMP(-controller->axis[XBOX_LEFT_STICK_VERT], 0, 1);
				game_input.move_input[MOVE_RIGHT]     = CLAMP( controller->axis[XBOX_LEFT_STICK_HOR], 0, 1);
				game_input.move_input[TILT_CAM_UP]    = CLAMP( controller->axis[XBOX_RIGHT_STICK_VERT], 0, 1);
				game_input.move_input[TILT_CAM_DOWN]  = CLAMP(-controller->axis[XBOX_RIGHT_STICK_VERT], 0, 1);
				game_input.move_input[TURN_CAM_LEFT]  = CLAMP(-controller->axis[XBOX_RIGHT_STICK_HOR], 0, 1);
				game_input.move_input[TURN_CAM_RIGHT] = CLAMP( controller->axis[XBOX_RIGHT_STICK_HOR], 0, 1);
				game_input.is_down[JUMP]              = controller->button[XBOX_BUTTON_A];
				game_input.is_down[RAISE_CAM]         = controller->button[XBOX_BUTTON_RB];
				game_input.is_down[LOWER_CAM]         = controller->button[XBOX_BUTTON_LB];
				game_input.was_down[JUMP]             = old_controller->button[XBOX_BUTTON_A];
				game_input.was_down[RAISE_CAM]        = old_controller->button[XBOX_BUTTON_RB];
				game_input.was_down[LOWER_CAM]        = old_controller->button[XBOX_BUTTON_LB];
			}
			else //use mouse and keyboard
			{
				game_input.move_input[MOVE_FORWARD] = new_input->keyboard_input[KEY_W];
				game_input.move_input[MOVE_LEFT]    = new_input->keyboard_input[KEY_A];
				game_input.move_input[MOVE_BACK]    = new_input->keyboard_input[KEY_S];
				game_input.move_input[MOVE_RIGHT]   = new_input->keyboard_input[KEY_D];

				if(!camera.use_mouse_controls)
				{
					game_input.move_input[TILT_CAM_UP]    = new_input->keyboard_input[KEY_UP];
					game_input.move_input[TILT_CAM_DOWN]  = new_input->keyboard_input[KEY_DOWN];
					game_input.move_input[TURN_CAM_LEFT]  = new_input->keyboard_input[KEY_LEFT];
					game_input.move_input[TURN_CAM_RIGHT] = new_input->keyboard_input[KEY_RIGHT];
				}
				else 
				{
					float mouse_dx = new_input->mouse.xpos - old_input->mouse.xpos;
					float mouse_dy = new_input->mouse.ypos - old_input->mouse.ypos;
					const float MOUSE_DEFAULT_SENSITIVITY = 0.5f;
					game_input.move_input[TILT_CAM_UP]    = MAX( mouse_dy * MOUSE_DEFAULT_SENSITIVITY, 0);
					game_input.move_input[TILT_CAM_DOWN]  = MAX(-mouse_dy * MOUSE_DEFAULT_SENSITIVITY, 0);
					game_input.move_input[TURN_CAM_LEFT]  = MAX( mouse_dx * MOUSE_DEFAULT_SENSITIVITY, 0);
					game_input.move_input[TURN_CAM_RIGHT] = MAX(-mouse_dx * MOUSE_DEFAULT_SENSITIVITY, 0);
				}

				game_input.is_down[JUMP]       = new_input->keyboard_input[KEY_SPACE];
				game_input.is_down[RAISE_CAM]  = new_input->keyboard_input[KEY_E];
				game_input.is_down[LOWER_CAM]  = new_input->keyboard_input[KEY_Q];
				game_input.was_down[JUMP]      = old_input->keyboard_input[KEY_SPACE];
				game_input.was_down[RAISE_CAM] = old_input->keyboard_input[KEY_E];
				game_input.was_down[LOWER_CAM] = old_input->keyboard_input[KEY_Q];
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Move player
		if(!freecam_mode) update_player(&player, game_input, camera, dt);
		if(player.pos.y < 0){
			player.is_jumping = false;
			player.is_on_ground = true;
			player.pos.y = 0;
			player.vel.y = 0;
		}
		
		//Update camera
		CameraMode cam_mode = CAM_MODE_FOLLOW_PLAYER;
		if(freecam_mode) cam_mode = CAM_MODE_DEBUG;
		
		update_camera(&camera, cam_mode, game_input, player.pos, dt);

		camera.P = perspective(90, window_data.aspect_ratio, NEAR_PLANE_Z, FAR_PLANE_Z);

		add_vec(&debug_draw_data, player.pos+vec3{0,0.75,0}, player.fwd);

		glUseProgram(basic_shader.id);
		glUniformMatrix4fv(basic_shader.V_loc, 1, GL_FALSE, camera.V.m);
		glUniformMatrix4fv(basic_shader.P_loc, 1, GL_FALSE, camera.P.m);

		//Draw player
		glBindVertexArray(player_vao);
		glUniform4fv(colour_loc, 1, player.colour.v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, player.M.m);
        glDrawElements(GL_TRIANGLES, player_num_indices, GL_UNSIGNED_SHORT, 0);

		//Draw ground
		glBindVertexArray(cube_vao);
		glUniform4fv(colour_loc, 1, vec4{0.8f, 0.1f, 0.2f, 1}.v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale_mat4(vec3{25, 0.1, 25}), vec3{0, -0.25 ,0}).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);

		//Draw some boxes
		glUniform4fv(colour_loc, 1, vec4{0.2f, 0.1f, 0.8f, 1}.v);

		#define NUM_BOXES 5
		mat4 box_model_mat[NUM_BOXES] = 
		{
			translate(scale_mat4(vec3{5, 1, 5}), vec3{-7, 0, -3}),
			translate(scale_mat4(vec3{5, 1, 5}), vec3{11, 0, -4}),
			translate(scale_mat4(vec3{5, 10, 5}), vec3{0, 0, -11}),
			translate(scale_mat4(vec3{5, 1, 5}), vec3{-5, 0, 4}),
			translate(scale_mat4(vec3{5, 1, 5}), vec3{3, 0, -6})
		};

		for(int32 i=0; i < NUM_BOXES; ++i){
			glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, box_model_mat[i].m);
			glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);
		}

		debug_draw_flush(&debug_draw_data, camera);

		glfwSwapBuffers(window);

		check_gl_error();
	}//end main loop

    return 0;
}
