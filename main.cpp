#define GL_LITE_IMPLEMENTATION
#include "gl_lite.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLFWwindow* window = NULL;
int gl_width = 1080;
int gl_height = 720;
float gl_aspect_ratio = (float)gl_width/gl_height;
bool gl_fullscreen = false;

#include "GameMaths.h"
#include "Input.h"
#include "Camera3D.h"
#include "init_gl.h"
#include "load_obj.h"
#include "Shader.h"
#include "DebugDrawing.h"
#include "Player.h"

int main(){
	if(!init_gl(window, "3D Platformer", gl_width, gl_height)){ return 1; }

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, player_num_indices*sizeof(unsigned short), indices, GL_STATIC_DRAW);
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
		load_obj_indexed("cube.obj", &vp, &vt, &vn, &indices, &num_verts, &cube_num_indices, false);

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_num_indices*sizeof(unsigned short), indices, GL_STATIC_DRAW);
		free(indices);
	}

	init_camera(&g_camera, vec3(0,2,5), vec3(0,0,0));
	
	init_debug_draw();

    //Load shaders
	Shader basic_shader = init_shader("MVP.vert", "uniform_colour_sunlight.frag");
	GLuint colour_loc = glGetUniformLocation(basic_shader.id, "colour");

	check_gl_error();

    double curr_time = glfwGetTime(), prev_time, dt;
	//-------------------------------------------------------------------------------------//
	//-------------------------------------MAIN LOOP---------------------------------------//
	//-------------------------------------------------------------------------------------//
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Get dt
		prev_time = curr_time;
		curr_time = glfwGetTime();
		dt = curr_time - prev_time;
		if(dt > 0.1) dt = 0.1;
		
		//Get Input
		g_mouse.prev_xpos = g_mouse.xpos;
    	g_mouse.prev_ypos = g_mouse.ypos;
		glfwPollEvents();
		poll_joystick();
		
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
				if(!m_was_pressed) { cam_mouse_controls = !cam_mouse_controls; }
				m_was_pressed = true;
			}
			else m_was_pressed = false;

			//Ctrl/Command-F to toggle fullscreen
			//Note: window_resize_callback takes care of resizing viewport/recalculating P matrix
			static bool F_was_pressed = false;
			if(glfwGetKey(window, GLFW_KEY_F)) {
				if(!F_was_pressed){
					if(glfwGetKey(window, CTRL_KEY_LEFT) || glfwGetKey(window, CTRL_KEY_RIGHT)){
						gl_fullscreen = !gl_fullscreen;
						static int old_win_x, old_win_y, old_win_w, old_win_h;
						if(gl_fullscreen){
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
		const double FIXED_TIME_STEP = 1.0/200;
		sim_time += dt;
		
		//Simulation
		while(sim_time>0){
			// Note: Because once per frame we simulate with dt < fixed time step, the game 
			// will not be 100% deterministic. Use a different method if that matters!
			double sim_dt = MIN(sim_time, FIXED_TIME_STEP);
			
			//Move player
			if(!freecam_mode) player_update(sim_dt);
			if(player_pos.y<0){
				player_is_jumping = false;
				player_is_on_ground = true;
				player_pos.y = 0;
				player_vel.y = 0;
			}
			
			//Update camera
			if(freecam_mode)update_camera_debug(&g_camera, sim_dt);
			else update_camera_player(&g_camera, player_pos, sim_dt);

			sim_time -= sim_dt;
		}


		draw_vec(player_pos+vec3(0,0.75,0), player_fwd);

		glUseProgram(basic_shader.id);
		glUniformMatrix4fv(basic_shader.V_loc, 1, GL_FALSE, g_camera.V.m);
		glUniformMatrix4fv(basic_shader.P_loc, 1, GL_FALSE, g_camera.P.m);

		//Draw player
		glBindVertexArray(player_vao);
		glUniform4fv(colour_loc, 1, player_colour.v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, player_M.m);
        glDrawElements(GL_TRIANGLES, player_num_indices, GL_UNSIGNED_SHORT, 0);

		//Draw ground
		glBindVertexArray(cube_vao);
		glUniform4fv(colour_loc, 1, vec4(0.8,0.1,0.2,1).v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale(identity_mat4(), vec3(25,0.1,25)),vec3(0,-0.25,0)).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);

		//Draw some boxes
		glUniform4fv(colour_loc, 1, vec4(0.2,0.1,0.8,1).v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale(identity_mat4(), vec3(5,1,5)),vec3(-7,0,-3)).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale(identity_mat4(), vec3(5,1,5)),vec3(11,0,-4)).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale(identity_mat4(), vec3(5,10,5)),vec3(0,0,-11)).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale(identity_mat4(), vec3(5,1,5)),vec3(-5,0,4)).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale(identity_mat4(), vec3(5,1,5)),vec3(3,0,-6)).m);
        glDrawElements(GL_TRIANGLES, cube_num_indices, GL_UNSIGNED_SHORT, 0);

		glfwSwapBuffers(window);

		check_gl_error();
	}//end main loop

    return 0;
}
