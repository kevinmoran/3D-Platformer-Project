#include "Player.h"

#include "GameMaths.h"
#include "Camera3D.h"
#include "Input.h"

Player g_player;

static float player_turn_speed = 720;
static float player_max_stand_slope = 60;

//Physics stuff
//Thanks to Kyle Pittman for his GDC talk:
// http://www.gdcvault.com/play/1023559/Math-for-Game-Programmers-Building
static float player_top_speed = 10.0f;
static float player_time_till_top_speed = 0.25f; //Human reaction time?
static float player_acc = player_top_speed/player_time_till_top_speed;
static float friction_factor = 0.3f; //higher is slippier
static float player_jump_height = 3.0f;
static float player_jump_dist_to_peak = 2.0f; //how far on xz-plane the player moves before reaching peak jump height
static float player_gravity = -2*player_jump_height*player_top_speed*player_top_speed/(player_jump_dist_to_peak*player_jump_dist_to_peak);
static float jump_vel = 2*player_jump_height*player_top_speed/player_jump_dist_to_peak;

void init_player(Player* player){
    player->pos = {};
    player->scale = {0.25, 0.5, 0.25};
    player->R = identity_mat4();
    player->M = translate(player->R * scale_mat4(player->scale), player->pos);
    player->vel = {};
    player->fwd = {0,0,-1};
    player->is_on_ground = false;
    player->is_jumping = false;
    player->colour = {0.1f, 0.8f, 0.3f, 1.0f};
}

void update_player(Player* player, const Camera3D &camera, double dt){

    bool player_moved = false;

    //WASD Movement (constrained to the x-z plane)
    vec3 player_movement_dir = {};
    {
        //Find player's forward and right movement directions
        vec3 cam_fwd_xz_proj = normalise(vec3{camera.fwd.x, 0, camera.fwd.z});
        vec3 cam_rgt_xz_proj = normalise(vec3{camera.rgt.x, 0, camera.rgt.z});

        float decel_factor = 1.;

        if(g_move_input[MOVE_FORWARD]) {
            player_movement_dir += cam_fwd_xz_proj*g_move_input[MOVE_FORWARD];
            player_moved = true;
        }
        else if(dot(player->vel, cam_fwd_xz_proj)>0.0001) player->vel -= cam_fwd_xz_proj*player_acc*decel_factor*dt;

        if(g_move_input[MOVE_LEFT]) {
            player_movement_dir += -cam_rgt_xz_proj*g_move_input[MOVE_LEFT];
            player_moved = true;
        }
        else if(dot(player->vel, -cam_rgt_xz_proj)>0.0001) player->vel += cam_rgt_xz_proj*player_acc*decel_factor*dt;
        
        if(g_move_input[MOVE_BACK]) {
            player_movement_dir += -cam_fwd_xz_proj*g_move_input[MOVE_BACK];
            player_moved = true;			
        }
        else if(dot(player->vel, -cam_fwd_xz_proj)>0.0001) player->vel += cam_fwd_xz_proj*player_acc*decel_factor*dt;
        
        if(g_move_input[MOVE_RIGHT]) {
            player_movement_dir += cam_rgt_xz_proj*g_move_input[MOVE_RIGHT];
            player_moved = true;		
        }
        else if(dot(player->vel, cam_rgt_xz_proj)>0.0001) player->vel -= cam_rgt_xz_proj*player_acc*decel_factor*dt;      
    }
    player_movement_dir = normalise(player_movement_dir);

    //Make player turn to face direction of travel
    if(length2(player_movement_dir)>0.00001)
    {
        float alignment = CLAMP(dot(player->fwd, player_movement_dir), -1, 1);
        if(!cmpf(alignment, 1, 0.00001))
        {
            float rotation_amount = player_turn_speed*dt;
            float angle_remaining = RAD2DEG(acosf(alignment));
            
            rotation_amount = MIN(rotation_amount, angle_remaining); //Clamp so we never rotate past target

            vec3 cross_prod = cross(player->fwd, player_movement_dir);
            if(cross_prod.y<0) rotation_amount *= -1;
            
            player->R = rotate_y_deg_mat4(rotation_amount) * player->R;
            player->fwd = -vec3{player->R.m[8], player->R.m[9], player->R.m[10]};
        }
    }

    //Accelerate player
    player->vel += player_movement_dir*player_acc*dt;

    if(player->is_on_ground){
        //Clamp player speed
        float player_speed = length(player->vel);
        if(player_speed > player_top_speed){
            player->vel = player->vel*player_top_speed/player_speed;
        }
        if(player_speed<0.0001) player->vel = {};

        //Deceleration
        if(!player_moved) player->vel *= friction_factor;

        static bool jump_button_was_pressed = false;
        if(g_input[JUMP]){
            if(!jump_button_was_pressed){
                player->vel.y += jump_vel;
                player->is_on_ground = false;
                player->is_jumping = true;
                jump_button_was_pressed = true;
            }
        }
        else jump_button_was_pressed = false;
    }
    else { //Player is not on ground
        if(player->is_jumping){
            //If you don't hold jump you don't jump as high
            if(!g_input[JUMP] && player->vel.y>0) player->vel.y += player_gravity*dt;
        }

        //Clamp player's xz speed
        vec3 xz_vel = {player->vel.x, 0, player->vel.z};
        if(length(xz_vel) > player_top_speed) {
            xz_vel = normalise(xz_vel);
            xz_vel *= player_top_speed;
            player->vel.x = xz_vel.x;
            player->vel.z = xz_vel.z;
        }
        player->vel.y += player_gravity*dt;
    }

    //Update player position
    player->pos += player->vel*dt;

    //Update matrices
    player->M = translate(player->R * scale_mat4(player->scale), player->pos);
}
