#pragma once

#include "GameMaths.h"
struct Camera3D;

struct Player {
    vec3 pos;
    vec3 scale;
    mat4 R;
    mat4 M;
    vec3 vel;
    vec3 fwd;
    bool is_on_ground;
    bool is_jumping;
    vec4 colour;
};
extern Player g_player;

void init_player(Player* player);
void update_player(Player* player, const Camera3D &camera, double dt);
