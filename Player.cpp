#include "Player.h"

#include "GameMaths.h"
#include "Camera3D.h"
#include "Input.h"

void init_player(Player* player)
{
    player->pos = {};
    player->scale = {0.25f, 0.5f, 0.25f};
    player->R = identity_mat4();
    player->M = translate(player->R * scale_mat4(player->scale), player->pos);
    player->vel = {};
    player->fwd = {0,0,-1};
    player->is_on_ground = false;
    player->is_jumping = false;
    player->colour = {0.1f, 0.8f, 0.3f, 1.0f};
}

void update_player(Player* player, const GameInput &game_input, const Camera3D &camera, double dt)
{
    vec3 player_move_dir = {};
    vec3 cam_fwd_xz_proj = normalise(vec3{camera.fwd.x, 0.0f, camera.fwd.z});
    vec3 cam_rgt_xz_proj = normalise(vec3{camera.rgt.x, 0.0f, camera.rgt.z});

    player_move_dir += cam_fwd_xz_proj * game_input.move_input[MOVE_FORWARD];
    player_move_dir -= cam_rgt_xz_proj * game_input.move_input[MOVE_LEFT];
    player_move_dir -= cam_fwd_xz_proj * game_input.move_input[MOVE_BACK];
    player_move_dir += cam_rgt_xz_proj * game_input.move_input[MOVE_RIGHT];

    float player_move_length2 = length2(player_move_dir);
    if(player_move_length2 > 1.0f)
        player_move_dir *= (1.0f / sqrtf(player_move_length2));

    //Make player turn to face direction of travel
    if(player_move_length2 > 0.00001f)
    {
        float alignment = dot(player->fwd, player_move_dir);
        alignment = CLAMP(alignment, -1.0f, 1.0f);

        if(!almost_equal(alignment, 1.0f, 0.00001f))
        {
            const float player_turn_speed_rads = 4.0f * PI32;
            float rotation_amount = player_turn_speed_rads * dt;
            float angle_remaining_rads = acosf(alignment);
            
            rotation_amount = MIN(rotation_amount, angle_remaining_rads); //Clamp so we never rotate past target

            vec3 cross_prod = cross(player->fwd, player_move_dir);
            if(cross_prod.y < 0.0f) rotation_amount *= -1.0f;
            
            player->R = rotate_y_rad(player->R, rotation_amount);
            player->fwd = -vec3{player->R.m[8], player->R.m[9], player->R.m[10]};
        }
    }

    const float player_top_speed = 12.0f;
    const float friction_factor = 8.0f;
    const float player_acc_value = friction_factor * player_top_speed;

    vec3 player_accel = player_move_dir * player_acc_value;
    player_accel -= friction_factor * vec3{player->vel.x, 0.0f, player->vel.z};

    player->vel += player_accel * dt;

    // Parameters for jump physics
    // Thanks to J. Kyle Pittman for his GDC talk: http://www.gdcvault.com/play/1023559/Math-for-Game-Programmers-Building
    const float player_jump_height = 3.0f;
    const float player_jump_dist_to_peak = 4.0f; //how far on xz-plane the player moves before reaching peak jump height
    const float player_jump_time_to_peak = (player_top_speed / player_jump_dist_to_peak);
    const float player_gravity = -2.0f * player_jump_height * (player_jump_time_to_peak * player_jump_time_to_peak);
    const float jump_vel = 2.0f * player_jump_height * player_jump_time_to_peak;

    if(player->is_on_ground)
    {
        if(game_input.is_down[JUMP] && !game_input.was_down[JUMP])
        {
            player->vel.y += jump_vel;
            player->is_on_ground = false;
            player->is_jumping = true;
        }
    }
    else //Player is not on ground 
    {
        float gravity_scale = 1.0f;
        if(player->is_jumping){
            //If you don't hold down the jump button you don't jump as high
            if(!game_input.is_down[JUMP] && (player->vel.y > 0.0f)) 
                gravity_scale = 4.0f;
        }
        
        player->vel.y += gravity_scale * player_gravity * dt;
    }

    //Update player position
    player->pos += player->vel * dt;

    //Update matrices
    player->M = translate(player->R * scale_mat4(player->scale), player->pos);
}
