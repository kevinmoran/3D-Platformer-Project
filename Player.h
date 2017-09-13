#pragma once

//Player data
vec3 player_pos = vec3(0,0,0);
vec3 player_scale = vec3(0.25, 0.5, 0.25);
mat4 player_R = identity_mat4();
mat4 player_M = translate(player_R*scale(identity_mat4(), player_scale), player_pos);
vec3 player_vel = vec3(0,0,0);
vec3 player_fwd = vec3(0,0,-1);
float player_turn_speed = 720;
vec4 player_colour = vec4(0.1f, 0.8f, 0.3f, 1.0f);
bool player_is_on_ground = false;
bool player_is_jumping = false;
float player_max_stand_slope = 60;
//Physics stuff
//Thanks to Kyle Pittman for his GDC talk:
// http://www.gdcvault.com/play/1023559/Math-for-Game-Programmers-Building
float player_top_speed = 10.0f;
float player_time_till_top_speed = 0.25f; //Human reaction time?
float player_acc = player_top_speed/player_time_till_top_speed;
float friction_factor = 0.3f; //higher is slippier
float player_jump_height = 3.0f;
float player_jump_dist_to_peak = 2.0f; //how far on xz p moves before reaching peak jump height
float g = -2*player_jump_height*player_top_speed*player_top_speed/(player_jump_dist_to_peak*player_jump_dist_to_peak);
float jump_vel = 2*player_jump_height*player_top_speed/player_jump_dist_to_peak;

void player_update(double dt){

    bool player_moved = false;

    //WASD Movement (constrained to the x-z plane)
    vec3 player_movement_dir = vec3(0,0,0);
    {
        //Find player's forward and right movement directions
        vec3 cam_fwd_xz_proj = normalise(vec3(g_camera.fwd.x, 0, g_camera.fwd.z));
        vec3 cam_rgt_xz_proj = normalise(vec3(g_camera.rgt.x, 0, g_camera.rgt.z));

        float decel_factor = 1.;

        if(g_move_input[MOVE_FORWARD]) {
            player_movement_dir += cam_fwd_xz_proj*g_move_input[MOVE_FORWARD];
            player_moved = true;
        }
        else if(dot(player_vel, cam_fwd_xz_proj)>0.0001) player_vel -= cam_fwd_xz_proj*player_acc*decel_factor*dt;

        if(g_move_input[MOVE_LEFT]) {
            player_movement_dir += -cam_rgt_xz_proj*g_move_input[MOVE_LEFT];
            player_moved = true;
        }
        else if(dot(player_vel, -cam_rgt_xz_proj)>0.0001) player_vel += cam_rgt_xz_proj*player_acc*decel_factor*dt;
        
        if(g_move_input[MOVE_BACK]) {
            player_movement_dir += -cam_fwd_xz_proj*g_move_input[MOVE_BACK];
            player_moved = true;			
        }
        else if(dot(player_vel, -cam_fwd_xz_proj)>0.0001) player_vel += cam_fwd_xz_proj*player_acc*decel_factor*dt;
        
        if(g_move_input[MOVE_RIGHT]) {
            player_movement_dir += cam_rgt_xz_proj*g_move_input[MOVE_RIGHT];
            player_moved = true;		
        }
        else if(dot(player_vel, cam_rgt_xz_proj)>0.0001) player_vel -= cam_rgt_xz_proj*player_acc*decel_factor*dt;      
    }
    player_movement_dir = normalise(player_movement_dir);

    //Make player turn to face direction of travel
    if(length2(player_movement_dir)>0.00001)
    {
        float alignment = CLAMP(dot(player_fwd, player_movement_dir), -1, 1);
        if(!cmpf_e(alignment, 1, 0.00001))
        {
            float rotation_amount = player_turn_speed*dt;
            float angle_remaining = RAD2DEG(acosf(alignment));
            
            rotation_amount = MIN(rotation_amount, angle_remaining); //Clamp so we never rotate past target

            vec3 cross_prod = cross(player_fwd, player_movement_dir);
            if(cross_prod.y<0) rotation_amount *= -1;
            
            player_R = rotate_y_deg(player_R,rotation_amount);
            player_fwd = -vec3(player_R.m[8],player_R.m[9],player_R.m[10]);
        }
    }

    //Accelerate player
    player_vel += player_movement_dir*player_acc*dt;

    if(player_is_on_ground){
        //Clamp player speed
        float player_speed = length(player_vel);
        if(player_speed > player_top_speed){
            player_vel = player_vel*player_top_speed/player_speed;
        }
        if(player_speed<0.0001) player_vel = vec3(0,0,0);

        //Deceleration
        if(!player_moved) player_vel *= friction_factor;

        static bool jump_button_was_pressed = false;
        if(g_input[JUMP]){
            if(!jump_button_was_pressed){
                player_vel.y += jump_vel;
                player_is_on_ground = false;
                player_is_jumping = true;
                jump_button_was_pressed = true;
            }
        }
        else jump_button_was_pressed = false;
    }
    else { //Player is not on ground
        if(player_is_jumping){
            //If you don't hold jump you don't jump as high
            if(!g_input[JUMP] && player_vel.y>0) player_vel.y += g*dt;
        }

        //Clamp player's xz speed
        vec3 xz_vel = vec3(player_vel.x, 0, player_vel.z);
        if(length(xz_vel) > player_top_speed) {
            xz_vel = normalise(xz_vel);
            xz_vel *= player_top_speed;
            player_vel.x = xz_vel.x;
            player_vel.z = xz_vel.z;
        }
        player_vel.y += g*dt;
    }

    //Update player position
    player_pos += player_vel*dt;

    //Update matrices
    player_M = translate(player_R*scale(identity_mat4(), player_scale), player_pos);
}
