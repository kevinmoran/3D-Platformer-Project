#include "Camera3D.h"
#include "Input.h"

#define NEAR_PLANE_Z 0.1f
#define FAR_PLANE_Z 300.0f

void init_camera(Camera3D* cam, vec3 cam_pos, vec3 target_pos){
    cam->pos = cam_pos;
    cam->V = look_at(cam_pos, target_pos, vec3{0,1,0});
	cam->rgt = vec3{cam->V.m[0], cam->V.m[4], cam->V.m[8]};
	cam->up  = vec3{cam->V.m[1], cam->V.m[5], cam->V.m[9]};
    cam->fwd = vec3{-cam->V.m[2], -cam->V.m[6], -cam->V.m[10]};
    //Note: these Euler angle calculations assume roll is 0
    cam->yaw   = RAD2DEG( acosf(cam->V.m[0]));
    cam->pitch = RAD2DEG(-acosf(cam->V.m[5])); //no idea why you negate this and not yaw, it just works
	cam->move_speed = 10;
	cam->turn_speed = 100;
}

void update_camera(Camera3D* cam, CameraMode cam_mode, GameInput &game_input, Mouse &mouse, vec3 player_pos, double dt)
{
    if(cam_mode == CAM_MODE_DEBUG)
    {
        //WASD Movement (constrained to the x-z plane)
        if(game_input.move_input[MOVE_FORWARD]) {
            vec3 xz_proj = normalise(vec3{cam->fwd.x, 0, cam->fwd.z});
            cam->pos += xz_proj*cam->move_speed*dt;
        }
        if(game_input.move_input[MOVE_LEFT]) {
            vec3 xz_proj = normalise(vec3{cam->rgt.x, 0, cam->rgt.z});
            cam->pos -= xz_proj*cam->move_speed*dt;
        }
        if(game_input.move_input[MOVE_BACK]) {
            vec3 xz_proj = normalise(vec3{cam->fwd.x, 0, cam->fwd.z});
            cam->pos -= xz_proj*cam->move_speed*dt;			
        }
        if(game_input.move_input[MOVE_RIGHT]) {
            vec3 xz_proj = normalise(vec3{cam->rgt.x, 0, cam->rgt.z});
            cam->pos += xz_proj*cam->move_speed*dt;			
        }
        //Increase/decrease elevation
        if(game_input.button_input[RAISE_CAM]) {
            cam->pos.y += cam->move_speed*dt;			
        }
        if(game_input.button_input[LOWER_CAM]) {
            cam->pos.y -= cam->move_speed*dt;			
        }
    }
    //Rotation
    if(!cam->use_mouse_controls){
        if(game_input.move_input[TURN_CAM_LEFT]) {
            cam->yaw += game_input.move_input[TURN_CAM_LEFT]*cam->turn_speed*dt;			
        }
        if(game_input.move_input[TURN_CAM_RIGHT]) {
            cam->yaw -= game_input.move_input[TURN_CAM_RIGHT]*cam->turn_speed*dt;			
        }
        if(game_input.move_input[TILT_CAM_UP]) {
            cam->pitch += game_input.move_input[TILT_CAM_UP]*cam->turn_speed*dt;			
        }
        if(game_input.move_input[TILT_CAM_DOWN]) {
            cam->pitch -= game_input.move_input[TILT_CAM_DOWN]*cam->turn_speed*dt;			
        }
    }
    else {
        cam->yaw   += (mouse.prev_xpos - mouse.xpos) * mouse.sensitivity * cam->turn_speed*dt;
        cam->pitch += (mouse.prev_ypos - mouse.ypos) * mouse.sensitivity * cam->turn_speed*dt;
    }
    while(cam->yaw >=360.f) cam->yaw -= 360.f;
    cam->pitch = CLAMP(cam->pitch, -85, 80);

    //Update matrices
    mat4 R = rotate_y_deg_mat4(cam->yaw) * rotate_x_deg_mat4(cam->pitch);
    cam->rgt = (R*vec4{1,0,0,0}).xyz;
    cam->up  = (R*vec4{0,1,0,0}).xyz;
    cam->fwd = (R*vec4{0,0,-1,0}).xyz;

    if(cam_mode == CAM_MODE_FOLLOW_PLAYER)
        cam->pos = player_pos - (5*cam->fwd) + (2*cam->up);

    cam->V = translate(identity_mat4(), -cam->pos);
    cam->V = transpose(R)*cam->V;
}
