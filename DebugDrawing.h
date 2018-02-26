#pragma once

#include "gl_lite.h"
#include "GameMaths.h"
#include "Shader.h"

struct DebugDrawPoint {
    vec3 pos;
    float size;
    vec4 colour;
};
struct DebugDrawLine {
    vec3 origin;
    vec3 dir;
    vec4 colour;
};

#define MAX_NUM_DEBUG_DRAW_ELEMENTS 64
struct DebugDraw {
    GLuint quad_vao;
    GLuint quad_vbo;
    Shader shader;
    GLuint colour_uniform_loc;
    uint32_t num_queued_points;
    uint32_t num_queued_lines;
    DebugDrawPoint points_queue[MAX_NUM_DEBUG_DRAW_ELEMENTS/2];
    DebugDrawLine lines_queue[MAX_NUM_DEBUG_DRAW_ELEMENTS/2];
};

// Simple functions to draw debug/visualisation stuff
void init_debug_draw(DebugDraw* draw_data);
void add_point(DebugDraw* draw_data, vec3 pos, float size, vec4 colour = vec4{0.8f, 0, 0, 1});
void add_vec(DebugDraw* draw_data, vec3 origin, vec3 dir, vec4 colour = vec4{0.8f, 0, 0, 1});
void debug_draw_flush(DebugDraw* draw_data, Camera3D &cam);
