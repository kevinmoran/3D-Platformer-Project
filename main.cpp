#define GL_LITE_IMPLEMENTATION
#include "gl_lite.h"
#undef GL_LITE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "utils.h"

#include "init_gl.h"
#include "GameMaths.h"
#include "Input.h"
#include "Camera3D.h"
#include "Shader.h"
#include "Player.h"
#include "load_obj.h"
#include "DebugDrawing.h"
#include "Mesh.h"
#include "Animation.h"

#include "Input.cpp"
#include "Camera3D.cpp"
#include "Shader.cpp"
#include "Player.cpp"
#include "load_obj.cpp"
#include "DebugDrawing.cpp"
#include "string_functions.cpp"
#include "Mesh.cpp"
#include "Animation.cpp"

int main(){
	GLFWwindow* window = NULL;
	GLFWWindowData window_data = {};
	window_data.handle = &window;
	window_data.width = 400;
	window_data.height = 300;
	window_data.aspect_ratio = (float)window_data.width/(float)window_data.height;

	RawInput* raw_input = (RawInput*)calloc(2, sizeof(RawInput));

	GLFWData glfw_data = {&window_data, &raw_input[0], &raw_input[1]};

	if(!init_gl(&glfw_data, "3D Platformer")){ return 1; }

	//Load meshes
	Mesh player_mesh;
	load_mesh(&player_mesh, "capsule.obj");
	Mesh cube_mesh;
	load_mesh(&cube_mesh, "cube.obj");

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

#if 0 // WIP: Animation

	Shader skinningShader = init_shader("Skinning.vert", "uniform_colour_sunlight.frag");
	int32 pose_mats_locs[MAX_NUM_BONES];

	//Get uniform locations for pose mats
	{
		glUseProgram(skinningShader.id);
		char name[16];

		for (uint32 i = 0; i < MAX_NUM_BONES; i++) {
			sprintf(name, "poseMats[%u]", i);
			pose_mats_locs[i] = glGetUniformLocation(skinningShader.id, name);
			if (pose_mats_locs[i] < 0) {
				printf("ERROR getting uniform location: '%s' not found in shader\n", name);
			}
		}
	}
	KmxSkinnedMesh* kmxMesh = NULL;
	GLuint kmx_vao;
	uint32 kmx_indexCount;
	{
		FILE* fp = fopen("/Users/kevin/Desktop/test.kmx", "rb");
		assert(fp);
		fseek(fp, 0, SEEK_END);
		uint64 file_length = ftell(fp);
		rewind(fp);

		void* file_bytes = malloc(file_length);
		assert(file_bytes);

		fread(file_bytes, file_length, 1, fp);
		fclose(fp);

		kmxMesh = (KmxSkinnedMesh*)file_bytes;
		assert(kmxMesh->magic == *(uint32*)("KMX "));

		kmx_indexCount = kmxMesh->indexCount;

		uint16* indices = (uint16*)(&kmxMesh->data + kmxMesh->indexOffset);
		float* vp = (float*)(&kmxMesh->data + kmxMesh->vpOffset);
		float* vn = (float*)(&kmxMesh->data + kmxMesh->vnOffset);
		// float* vt = (float*)(&kmxMesh->data + kmxMesh->vtOffset);
		uint32* vbone_ids = (uint32*)(&kmxMesh->data + kmxMesh->vboneIdOffset);
		float* vbone_weights = (float*)(&kmxMesh->data + kmxMesh->vboneWeightOffset);
		// mat4* ibp_mats = (mat4*)(&kmxMesh->data + kmxMesh->inverseBindPosesOffset);

		// for(uint32 i=0; i < kmxMesh->vertCount; ++i)
		// {
		// 	printf("%i %i %i %i   ", vbone_ids[4*i], vbone_ids[4*i + 1], vbone_ids[4*i + 2], vbone_ids[4*i + 3]);
		// 	printf("%f %f %f %f\n", vbone_weights[4*i], vbone_weights[4*i + 1], vbone_weights[4*i + 2], vbone_weights[4*i + 3]);
		// }

		glGenVertexArrays(1, &kmx_vao);
        glBindVertexArray(kmx_vao);

		GLuint pos_vbo, norm_vbo, bone_ids_vbo, bone_weights_vbo, index_vbo;
        
        glGenBuffers(1, &index_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, kmxMesh->indexCount*sizeof(uint16), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &pos_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
        glBufferData(GL_ARRAY_BUFFER, kmxMesh->vertCount*3*sizeof(float), vp, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VP_ATTRIB_LOC);
        glVertexAttribPointer(VP_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glGenBuffers(1, &norm_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
        glBufferData(GL_ARRAY_BUFFER, kmxMesh->vertCount*3*sizeof(float), vn, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VN_ATTRIB_LOC);
        glVertexAttribPointer(VN_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glGenBuffers(1, &bone_ids_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, bone_ids_vbo);
        glBufferData(GL_ARRAY_BUFFER, kmxMesh->vertCount*4*sizeof(uint32), vbone_ids, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VBONE_IDS_ATTRIB_LOC);
        glVertexAttribIPointer(VBONE_IDS_ATTRIB_LOC, 4, GL_UNSIGNED_INT, 0, NULL);

        glGenBuffers(1, &bone_weights_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, bone_weights_vbo);
        glBufferData(GL_ARRAY_BUFFER, kmxMesh->vertCount*4*sizeof(float), vbone_weights, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VBONE_WEIGHTS_ATTRIB_LOC);
        glVertexAttribPointer(VBONE_WEIGHTS_ATTRIB_LOC, 4, GL_FLOAT, GL_FALSE, 0, NULL);

        check_gl_error();
	}

	KmxSkeleton* skeleton = NULL;
	{
		FILE* fp = fopen("/Users/kevin/Desktop/test_skel2.kmx", "rb");
		assert(fp);
		fseek(fp, 0, SEEK_END);
		uint64 file_length = ftell(fp);
		rewind(fp);

		void* file_bytes = malloc(file_length);
		assert(file_bytes);
		fread(file_bytes, file_length, 1, fp);
		fclose(fp);

		skeleton = (KmxSkeleton*)file_bytes;
		assert(skeleton->magic == *(uint32*)("KMX "));

		// KmxBone* bones = (KmxBone*)(&skeleton->data + skeleton->bonesOffset);
		// KmxAnimation* anims = (KmxAnimation*)(&skeleton->data + skeleton->animationsOffset);

		// printf("Bones:\n");
		// for(uint32 i=0; i < skeleton->numBones; ++i)
		// {
		// 	printf("%u name: %s, parent: %i\n", i, bones[i].name, bones[i].parentIndex);
		// }

		// printf("Animations:\n");
		// for(uint32 i=0; i < skeleton->numAnimations; ++i)
		// {
		// 	printf("%u name: %s, duration: %f\n", i, anims[i].name, anims[i].duration);

		// 	printf("Keyframe data:\n");
		// 	KmxBoneKeyFrames* keys = (KmxBoneKeyFrames*)(&skeleton->data + anims[i].keyFramesOffset);
		// 	for(uint32 boneIndex=0; boneIndex < skeleton->numBones; ++boneIndex)
		// 	{
		// 		KmxBoneKeyFrames* currBoneKeyFrames = &keys[boneIndex];
		// 		printf("Bone %u:\n", boneIndex);
		// 		float* traKeyTimes = (float*)(&skeleton->data + currBoneKeyFrames->traKeyTimesOffset);
		// 		vec3* traKeys = (vec3*)(&skeleton->data + currBoneKeyFrames->traKeysOffset);

		// 		for(int k=0; k < currBoneKeyFrames->numTraKeys; ++k)
		// 		{
		// 			printf("t: %f, tra: [%f, %f, %f]\n", traKeyTimes[k], traKeys[k].x, traKeys[k].y, traKeys[k].z);
		// 		}

		// 		float* rotKeyTimes = (float*)(&skeleton->data + currBoneKeyFrames->rotKeyTimesOffset);
		// 		versor* rotKeys = (versor*)(&skeleton->data + currBoneKeyFrames->rotKeysOffset);

		// 		for(int k=0; k < currBoneKeyFrames->numRotKeys; ++k)
		// 		{
		// 			printf("t: %f, rot: [%f, %f, %f, %f]\n", rotKeyTimes[k], rotKeys[k].q[0], rotKeys[k].q[1], rotKeys[k].q[2], rotKeys[k].q[3]);
		// 		}
		// 	}
		// }
	}

	// mat4* ibp_mats = (mat4*)(&kmxMesh->data + kmxMesh->inverseBindPosesOffset);
	// for(int i=0; i<skeleton->numBones; ++i){
	// 	print(ibp_mats[i]);
	// }

	mat4* poseMats = (mat4*)malloc(skeleton->numBones * sizeof(mat4));
#endif

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
		float dt = (float)(curr_time - prev_time);
		if(dt > 0.1f) dt = 0.1f;
		
		//Get Input
		RawInput* new_input = glfw_data.new_input;
		RawInput* old_input = glfw_data.old_input;
		copy_memory(old_input, new_input, sizeof(RawInput));
		glfwPollEvents();

		{
			vec2 new_mouse_pos = {(f32)new_input->mouse.xpos, (f32)new_input->mouse.ypos};
			vec2 old_mouse_pos = {(f32)old_input->mouse.xpos, (f32)old_input->mouse.ypos};

			//TODO: this threshold is completely ad-hoc, check it
			const float MOUSE_MOVED_INTENTIONALLY_THRESHOLD = 25.0f;
			if(length2(new_mouse_pos - old_mouse_pos) > MOUSE_MOVED_INTENTIONALLY_THRESHOLD) 
				new_input->use_controller = false;
		}

		poll_joystick(new_input);

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
					if(window_data.is_fullscreen)
					{
						glfwGetWindowPos(window, &window_data.old_win_x, &window_data.old_win_y);
						glfwGetWindowSize(window, &window_data.old_win_w, &window_data.old_win_h);
						GLFWmonitor* mon = glfwGetPrimaryMonitor();
						const GLFWvidmode* vidMode = glfwGetVideoMode(mon);
						glfwSetWindowMonitor(window, mon, 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
					}
					else glfwSetWindowMonitor(window, NULL, window_data.old_win_x, window_data.old_win_y, window_data.old_win_w, window_data.old_win_h, GLFW_DONT_CARE);
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

		camera.P = perspective(90.0f, window_data.aspect_ratio, NEAR_PLANE_Z, FAR_PLANE_Z);

		add_vec(&debug_draw_data, player.pos + vec3{0, 0.75f, 0}, player.fwd);

		glUseProgram(basic_shader.id);
		glUniformMatrix4fv(basic_shader.V_loc, 1, GL_FALSE, camera.V.m);
		glUniformMatrix4fv(basic_shader.P_loc, 1, GL_FALSE, camera.P.m);

		//Draw player
		glBindVertexArray(player_mesh.vao);
		glUniform4fv(colour_loc, 1, player.colour.v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, player.M.m);
        glDrawElements(GL_TRIANGLES, player_mesh.num_indices, GL_UNSIGNED_SHORT, 0);

		//Draw ground
		glBindVertexArray(cube_mesh.vao);
		glUniform4fv(colour_loc, 1, vec4{0.8f, 0.1f, 0.2f, 1}.v);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(scale_mat4(vec3{25, 0.1, 25}), vec3{0, -0.25 ,0}).m);
        glDrawElements(GL_TRIANGLES, cube_mesh.num_indices, GL_UNSIGNED_SHORT, 0);

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
			glDrawElements(GL_TRIANGLES, cube_mesh.num_indices, GL_UNSIGNED_SHORT, 0);
		}

#if 0 // WIP: Animation
		glBindVertexArray(kmx_vao);
		glUniformMatrix4fv(basic_shader.M_loc, 1, GL_FALSE, translate(identity_mat4(), vec3{-3,2,0}).m);
		glDrawElements(GL_TRIANGLES, kmx_indexCount, GL_UNSIGNED_SHORT, 0);

		{
			static float animTime = 0.0f;
			animTime += dt;

			uint32 CURRENT_ANIM_INDEX = 1;

			KmxAnimation* animations = (KmxAnimation*)(&skeleton->data + skeleton->animationsOffset);
			KmxAnimation* animation = &animations[CURRENT_ANIM_INDEX];
			if(animTime > animation->duration)
				animTime -= animation->duration;

			mat4* inverseBindPoses = (mat4*)(&kmxMesh->data + kmxMesh->inverseBindPosesOffset);

			animate(*skeleton, CURRENT_ANIM_INDEX, animTime, inverseBindPoses, &poseMats);

			glUseProgram(skinningShader.id);

			// {
			// 	uint32* vbone_ids = (uint32*)(&kmxMesh->data + kmxMesh->vboneIdOffset);
			// 	float* vbone_weights = (float*)(&kmxMesh->data + kmxMesh->vboneWeightOffset);

			// 	for(uint32 i=0; i < kmxMesh->vertCount; ++i)
			// 	{
			// 		mat4 pose0 = poseMats[vbone_ids[4*i]];
			// 		mat4 pose1 = poseMats[vbone_ids[4*i + 1]];
			// 		mat4 pose2 = poseMats[vbone_ids[4*i + 2]];
			// 		mat4 pose3 = poseMats[vbone_ids[4*i + 3]];

			// 		mat4 pose = pose0 * vbone_weights[4*i]
			// 				  + pose1 * vbone_weights[4*i + 1]
			// 				  + pose2 * vbone_weights[4*i + 2]
			// 				  + pose3 * vbone_weights[4*i + 3];

			// 		assert(pose == identity_mat4());
			// 	}
			// 	return 0;
			// }

			for(uint32 i = 0; i < skeleton->numBones; i++) {
				glUniformMatrix4fv(pose_mats_locs[i], 1, GL_FALSE, poseMats[i].m);
			}

			glUniformMatrix4fv(skinningShader.M_loc, 1, GL_FALSE, translate(identity_mat4(), vec3{0,2,0}).m);
			glUniformMatrix4fv(skinningShader.V_loc, 1, GL_FALSE, camera.V.m);
			glUniformMatrix4fv(skinningShader.P_loc, 1, GL_FALSE, camera.P.m);
			glUniform4fv(glGetUniformLocation(skinningShader.id, "colour"), 1, vec4{0.8f, 0.1f, 0.6f, 1}.v);

			glBindVertexArray(kmx_vao);
			glDrawElements(GL_TRIANGLES, kmx_indexCount, GL_UNSIGNED_SHORT, 0);

		}
#endif
		debug_draw_flush(&debug_draw_data, camera);

		glfwSwapBuffers(window);

		check_gl_error();
	}//end main loop

    return 0;
}
