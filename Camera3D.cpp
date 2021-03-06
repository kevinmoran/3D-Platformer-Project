#include "Camera3D.h"
#include "Input.h"

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

void update_camera(Camera3D* cam, CameraMode cam_mode, GameInput &game_input, vec3 player_pos, float dt)
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
        if(game_input.is_down[RAISE_CAM]) {
            cam->pos.y += cam->move_speed*dt;			
        }
        if(game_input.is_down[LOWER_CAM]) {
            cam->pos.y -= cam->move_speed*dt;			
        }
    }
    //Rotation
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

    while(cam->yaw >= 360.f) 
        cam->yaw -= 360.f;

    cam->pitch = CLAMP(cam->pitch, -85, 80);

    //Update matrices
    mat4 R = rotate_y_deg_mat4(cam->yaw) * rotate_x_deg_mat4(cam->pitch);
    cam->rgt = {R.m[0], R.m[1], R.m[2]};
    cam->up  = {R.m[4], R.m[5], R.m[6]};
    cam->fwd = {-R.m[8], -R.m[9], -R.m[10]};

    if(cam_mode == CAM_MODE_FOLLOW_PLAYER)
        cam->pos = player_pos - (5*cam->fwd) + (2*cam->up);

    cam->V = translate(identity_mat4(), -cam->pos);
    cam->V = transpose(R)*cam->V;
}
