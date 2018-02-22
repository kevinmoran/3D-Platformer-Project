#pragma once

#include "GameMaths.h"

// Simple functions to draw debug/visualisation stuff
void init_debug_draw();
void draw_point(vec3 pos, float size, vec4 colour = vec4{0.8f, 0, 0, 1});
void draw_vec(vec3 origin, vec3 dir, vec4 colour = vec4{0.8f, 0, 0, 1});

#ifdef DEBUG_DRAWING_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include "GameMaths.h"

// Internal data
static GLuint quad_vao;
static Shader debug_shader;

void init_debug_draw()
{
    float points[] = {
		-0.5f,	0.5f, 0,
		-0.5f, -0.5f, 0,
		 0.5f, -0.5f, 0,
		-0.5f,	0.5f, 0,
		 0.5f, -0.5f, 0,
		 0.5f,  0.5f, 0
	};

	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);
	glEnableVertexAttribArray(VP_ATTRIB_LOC);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(VP_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    debug_shader = init_shader("MVP_depth_bias.vert", "uniform_colour.frag");
}

void draw_point(vec3 pos, float size, vec4 colour)
{
    glUseProgram(debug_shader.id);

	GLuint col_loc = glGetUniformLocation(debug_shader.id, "colour");
	glUniform4fv(col_loc, 1, colour.v);

    // Spherical Billboarding (quad normal is always -cam.fwd)
    mat4 cam_rot_mat = g_camera.V;
    cam_rot_mat.m[12] = 0; 
    cam_rot_mat.m[13] = 0;
    cam_rot_mat.m[14] = 0;
    // Note: cam view matrix is orthogonal so can use transpose for a cheap inverse. 
    // Doesn't work for translation, but we only want rotation anyway so just clear it first
    mat4 M = transpose(cam_rot_mat);

    M = translate(scale(M, size), pos);

	glUniformMatrix4fv(debug_shader.M_loc, 1, GL_FALSE, M.m);
    glUniformMatrix4fv(debug_shader.V_loc, 1, GL_FALSE, g_camera.V.m);
    glUniformMatrix4fv(debug_shader.P_loc, 1, GL_FALSE, g_camera.P.m);

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void draw_vec(vec3 origin, vec3 dir, vec4 colour)
{
    mat4 M = translate(identity_mat4(), vec3{0, 0.5f, 0}); // move base of quad to origin
    float dist = length(dir);
    M = scale(M, vec3{0.05f, dist, 0.05f});

    // First rotate quad so its 'up' vector is dir
    mat4 R = rotate_align_mat4(vec3{0, 1, 0}, normalise(dir));

    // Billboarding - rotate quad around dir so its normal is facing camera (as much as possible)
    // From https://www.opengl.org/discussion_boards/archive/index.php/t-147495.html
    // "If you constrain your billboard to be rotating about only one axis, then the billboard's normal 
    // sweeps out a [disc on a] plane during this rotation. If we project the vector [to] the camera [from] the 
    // center of the billboard onto this plane, that will be the vector we want to [align the billboard normal with]."

    // Cool trick to project vector onto a plane: project it onto plane's normal
    // and subtract that projection from the original vector
    vec3 rot_sweep_plane_norm = (R * vec4{0,-1,0,0}).xyz; // normal of plane swept by rotating billboard's normal around dir
    vec3 billboard_center = origin + 0.5f*dir;
    vec3 v_to_cam = g_camera.pos - billboard_center;
    float norm_proj = dot(v_to_cam, rot_sweep_plane_norm); // project vec onto norm
    vec3 plane_proj = v_to_cam - (rot_sweep_plane_norm*norm_proj); // find projection onto plane (our target quad norm!)

    vec3 quad_norm = (R*vec4{0,0,1,0}).xyz;
    
    mat4 billboard_mat = rotate_align_mat4(quad_norm, normalise(plane_proj));

    M = billboard_mat * R * M;
    M = translate(M, origin);

    glUseProgram(debug_shader.id);
	GLuint col_loc = glGetUniformLocation(debug_shader.id, "colour");
	glUniform4fv(col_loc, 1, colour.v);
	glUniformMatrix4fv(debug_shader.M_loc, 1, GL_FALSE, M.m);
    glUniformMatrix4fv(debug_shader.V_loc, 1, GL_FALSE, g_camera.V.m);
    glUniformMatrix4fv(debug_shader.P_loc, 1, GL_FALSE, g_camera.P.m);

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    draw_point(origin+dir, 0.15f, colour);
}

// to avoid multiple includes when doing a single translation unit build
#undef DEBUG_DRAWING_IMPLEMENTATION
#endif //DEBUG_DRAWING_IMPLEMENTATION
