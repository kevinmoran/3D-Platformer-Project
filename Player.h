#pragma once

#include "GameMaths.h"
#include "Input.h"

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

void init_player(Player* player);
void update_player(Player* player, GameInput &game_input, const Camera3D &camera, double dt);
