#pragma once
// #include "GameMaths.h"
// #include "Input.h"

bool cam_mouse_controls = false;
const float near_plane = 0.1f;
const float far_plane = 300.0f;

struct Camera3D {
    vec3 pos;
	vec3 fwd, up, rgt;
	float pitch, yaw;
	float move_speed, turn_speed;
	mat4 V, P;
};
Camera3D g_camera;

void init_camera(Camera3D* cam);
void init_camera(Camera3D* cam, vec3 cam_pos);
void init_camera(Camera3D* cam, vec3 cam_pos, vec3 target_pos);
void update_camera_debug(Camera3D* cam, double dt);
void update_camera_player(Camera3D* cam, vec3 player_pos, double dt);

void init(Camera3D* cam){
   cam->pos = vec3(0,1,5);
	cam->fwd = vec3(0,0,-1);
	cam->up = vec3(0,1,0);
	cam->rgt = vec3(1,0,0);
	cam->yaw = 0;
	cam->pitch = 0;
	cam->move_speed = 10;
	cam->turn_speed = 100;
	cam->V = look_at(cam->pos, cam->pos+cam->fwd, cam->up);
	cam->P = perspective(90/gl_aspect_ratio, gl_aspect_ratio, near_plane, far_plane);

    if(cam_mouse_controls) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}

void init_camera(Camera3D* cam, vec3 cam_pos){
    cam->pos = cam_pos;
    cam->V = look_at(cam_pos, cam_pos+vec3(0,0,-1), vec3(0,1,0));
	cam->P = perspective(90/gl_aspect_ratio, gl_aspect_ratio, near_plane, far_plane);
	cam->rgt = vec3(cam->V.m[0], cam->V.m[4], cam->V.m[8]);
	cam->up  = vec3(cam->V.m[1], cam->V.m[5], cam->V.m[9]);
    cam->fwd = vec3(-cam->V.m[2], -cam->V.m[6], -cam->V.m[10]);
    cam->yaw   =  acos(cam->V.m[0])*ONE_RAD_IN_DEG;
    cam->pitch = -acos(cam->V.m[5])*ONE_RAD_IN_DEG; //no idea why you negate this and not yaw, it just works
	cam->move_speed = 10;
	cam->turn_speed = 100;

    if(cam_mouse_controls) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}

void init_camera(Camera3D* cam, vec3 cam_pos, vec3 target_pos){
    cam->pos = cam_pos;
    cam->V = look_at(cam_pos, target_pos, vec3(0,1,0));
	cam->P = perspective(90/gl_aspect_ratio, gl_aspect_ratio, near_plane, far_plane);
	cam->rgt = vec3(cam->V.m[0], cam->V.m[4], cam->V.m[8]);
	cam->up  = vec3(cam->V.m[1], cam->V.m[5], cam->V.m[9]);
    cam->fwd = vec3(-cam->V.m[2], -cam->V.m[6], -cam->V.m[10]);
    cam->yaw   =  acos(cam->V.m[0])*ONE_RAD_IN_DEG;
    cam->pitch = -acos(cam->V.m[5])*ONE_RAD_IN_DEG; //no idea why you negate this and not yaw, it just works
	cam->move_speed = 10;
	cam->turn_speed = 100;

    if(cam_mouse_controls) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}

void update_camera_debug(Camera3D* cam, double dt){
    //WASD Movement (constrained to the x-z plane)
    if(g_move_input[MOVE_FORWARD]) {
        vec3 xz_proj = normalise(vec3(cam->fwd.x, 0, cam->fwd.z));
        cam->pos += xz_proj*cam->move_speed*dt;
    }
    if(g_move_input[MOVE_LEFT]) {
        vec3 xz_proj = normalise(vec3(cam->rgt.x, 0, cam->rgt.z));
        cam->pos -= xz_proj*cam->move_speed*dt;
    }
    if(g_move_input[MOVE_BACK]) {
        vec3 xz_proj = normalise(vec3(cam->fwd.x, 0, cam->fwd.z));
        cam->pos -= xz_proj*cam->move_speed*dt;			
    }
    if(g_move_input[MOVE_RIGHT]) {
        vec3 xz_proj = normalise(vec3(cam->rgt.x, 0, cam->rgt.z));
        cam->pos += xz_proj*cam->move_speed*dt;			
    }
    //Increase/decrease elevation
    if(g_input[RAISE_CAM]) {
        cam->pos.y += cam->move_speed*dt;			
    }
    if(g_input[LOWER_CAM]) {
        cam->pos.y -= cam->move_speed*dt;			
    }
    //Rotation
    if(!cam_mouse_controls){
        if(g_move_input[TURN_CAM_LEFT]) {
            cam->yaw += g_move_input[TURN_CAM_LEFT]*cam->turn_speed*dt;			
        }
        if(g_move_input[TURN_CAM_RIGHT]) {
            cam->yaw -= g_move_input[TURN_CAM_RIGHT]*cam->turn_speed*dt;			
        }
        if(g_move_input[TILT_CAM_UP]) {
            cam->pitch += g_move_input[TILT_CAM_UP]*cam->turn_speed*dt;			
        }
        if(g_move_input[TILT_CAM_DOWN]) {
            cam->pitch -= g_move_input[TILT_CAM_DOWN]*cam->turn_speed*dt;			
        }
    }
    else {
        cam->yaw   += (g_mouse.prev_xpos-g_mouse.xpos) * g_mouse.sensitivity * cam->turn_speed*dt;
        cam->pitch += (g_mouse.prev_ypos-g_mouse.ypos) * g_mouse.sensitivity * cam->turn_speed*dt;
    }
    //Update matrices
    cam->pitch = MIN(MAX(cam->pitch, -89), 80); //Clamp pitch 
    mat4 R = rotate_x_deg(rotate_y_deg(identity_mat4(), -cam->yaw), -cam->pitch); //how does this work? 
    //Surely rotating by the yaw will misalign the x-axis for the pitch rotation and screw this up?
    //Seems to work for now! ¯\_(ツ)_/¯
    cam->rgt = inverse(R)*vec4(1,0,0,0); //I guess it makes sense that you have to invert
    cam->up  = inverse(R)*vec4(0,1,0,0);  //R to calculate these??? I don't know anymore
    cam->fwd = inverse(R)*vec4(0,0,-1,0);

    cam->V = translate(identity_mat4(), cam->pos*(-1));
    cam->V = R*cam->V;
}

void update_camera_player(Camera3D* cam, vec3 player_pos, double dt){
    //Rotation
    if(!cam_mouse_controls){
        if(g_move_input[TURN_CAM_LEFT]) {
            cam->yaw += g_move_input[TURN_CAM_LEFT]*cam->turn_speed*dt;			
        }
        if(g_move_input[TURN_CAM_RIGHT]) {
            cam->yaw -= g_move_input[TURN_CAM_RIGHT]*cam->turn_speed*dt;			
        }
        if(g_move_input[TILT_CAM_UP]) {
            cam->pitch += g_move_input[TILT_CAM_UP]*cam->turn_speed*dt;			
        }
        if(g_move_input[TILT_CAM_DOWN]) {
            cam->pitch -= g_move_input[TILT_CAM_DOWN]*cam->turn_speed*dt;			
        }
    }
    else {
        cam->yaw   += (g_mouse.prev_xpos-g_mouse.xpos) * g_mouse.sensitivity * cam->turn_speed*dt;
        cam->pitch += (g_mouse.prev_ypos-g_mouse.ypos) * g_mouse.sensitivity * cam->turn_speed*dt;
    }
    //Update matrices
    cam->pitch = MIN(MAX(cam->pitch, -89), 80); //Clamp pitch 
    mat4 R = rotate_x_deg(rotate_y_deg(identity_mat4(), -cam->yaw), -cam->pitch); //how does this work? 
    //Surely rotating by the yaw will misalign the x-axis for the pitch rotation and screw this up?
    //Seems to work for now! ¯\_(ツ)_/¯
    cam->rgt = inverse(R)*vec4(1,0,0,0); //I guess it makes sense that you have to invert
    cam->up  = inverse(R)*vec4(0,1,0,0);  //R to calculate these??? I don't know anymore
    cam->fwd = inverse(R)*vec4(0,0,-1,0);

    cam->pos = player_pos-cam->fwd*5 + cam->up*2;

    cam->V = translate(identity_mat4(), cam->pos*(-1));
    cam->V = R*cam->V;
}

void window_resize_callback(GLFWwindow* window, int width, int height){
    gl_width = width;
    gl_height = height;
    gl_aspect_ratio = (float)gl_width/gl_height;
    g_camera.P = perspective(90/gl_aspect_ratio, gl_aspect_ratio, near_plane, far_plane);
    int fb_w, fb_h;
	glfwGetFramebufferSize(window,&fb_w,&fb_h);
	glViewport(0,0,fb_w,fb_h);
}
