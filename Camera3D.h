#pragma once
#include "GameMaths.h"

extern bool cam_mouse_controls;
extern const float near_plane;
extern const float far_plane;

struct Camera3D {
    vec3 pos;
	vec3 fwd, up, rgt;
	float pitch, yaw;
	float move_speed, turn_speed;
	mat4 V, P;
};

enum CameraMode {
    CAM_MODE_DEBUG,
    CAM_MODE_FOLLOW_PLAYER
};

void init_camera(Camera3D* cam, vec3 cam_pos, vec3 target_pos);
void init_camera(Camera3D* cam, vec3 cam_pos);
void update_camera(Camera3D* cam, CameraMode cam_mode, vec3 player_pos, double dt);
