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

	GameInput game_input = {};
	RawInput raw_input = {};
	raw_input.mouse.sensitivity = MOUSE_DEFAULT_SENSITIVITY;

	PlatformData platform_data = {&window_data, &raw_input};

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
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Get dt
		double prev_time = curr_time;
		curr_time = glfwGetTime();
		double dt = curr_time - prev_time;
		if(dt > 0.1) dt = 0.1;
		
		//Get Input
		raw_input.mouse.prev_xpos = raw_input.mouse.xpos;
    	raw_input.mouse.prev_ypos = raw_input.mouse.ypos;
		glfwPollEvents();
		poll_joystick(platform_data.input);
		process_raw_input(&raw_input, &game_input);
		
		//Check miscellaneous button presses
		static bool freecam_mode = false;
		{
			if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
				glfwSetWindowShouldClose(window, 1);
			}

			//Tab to toggle player_cam/freecam
			static bool tab_was_pressed = false;
			if(glfwGetKey(window, GLFW_KEY_TAB)) {
				if(!tab_was_pressed) { freecam_mode = !freecam_mode; }
				tab_was_pressed = true;
			}
			else tab_was_pressed = false;

			//M to toggle between mouse/arrow key controls for camera
			static bool m_was_pressed = false;
			if(glfwGetKey(window, GLFW_KEY_M)) {
				if(!m_was_pressed) { camera.use_mouse_controls = !camera.use_mouse_controls; }
				m_was_pressed = true;
			}
			else m_was_pressed = false;

			//Ctrl/Command-F to toggle fullscreen
			//Note: window_resize_callback takes care of resizing viewport
			static bool F_was_pressed = false;
			if(glfwGetKey(window, GLFW_KEY_F)) {
				if(!F_was_pressed){
					if(glfwGetKey(window, CTRL_KEY_LEFT) || glfwGetKey(window, CTRL_KEY_RIGHT)){
						window_data.is_fullscreen = !window_data.is_fullscreen;
						static int old_win_x, old_win_y, old_win_w, old_win_h;
						if(window_data.is_fullscreen){
							glfwGetWindowPos(window, &old_win_x, &old_win_y);
							glfwGetWindowSize(window, &old_win_w, &old_win_h);
							GLFWmonitor* mon = glfwGetPrimaryMonitor();
							const GLFWvidmode* vidMode = glfwGetVideoMode(mon);
							glfwSetWindowMonitor(window, mon, 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
						}
						else glfwSetWindowMonitor(window, NULL, old_win_x, old_win_y, old_win_w, old_win_h, GLFW_DONT_CARE);
					}
				}
				F_was_pressed = true;
			}
			else F_was_pressed = false;
		}

		//Timer for updating game simulation with fixed time step
		static double sim_time = 0;
		const double FIXED_TIME_STEP = 1.0/240;
		sim_time += dt;
		
		//Simulation
		while(sim_time>0){
			// Note: Because once per frame we simulate with dt < fixed time step, the game 
			// will not be 100% deterministic. Use a different method if that matters!
			double sim_dt = MIN(sim_time, FIXED_TIME_STEP);
			
			//Move player
			if(!freecam_mode) update_player(&player, game_input, camera, sim_dt);
			if(player.pos.y<0){
				player.is_jumping = false;
				player.is_on_ground = true;
				player.pos.y = 0;
				player.vel.y = 0;
			}
			
			//Update camera
			CameraMode cam_mode = CAM_MODE_FOLLOW_PLAYER;
			if(freecam_mode) cam_mode = CAM_MODE_DEBUG;
			
			update_camera(&camera, cam_mode, game_input, raw_input.mouse, player.pos, sim_dt);

			sim_time -= sim_dt;
		}

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
