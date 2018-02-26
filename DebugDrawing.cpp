#include "DebugDrawing.h"

#include "Camera3D.h"
#include "utils.h"

void init_debug_draw(DebugDraw* draw_data)
{
    float points[] = {
		-0.5f,	0.5f, 0,
		-0.5f, -0.5f, 0,
		 0.5f, -0.5f, 0,
		-0.5f,	0.5f, 0,
		 0.5f, -0.5f, 0,
		 0.5f,  0.5f, 0
	};

	glGenVertexArrays(1, &draw_data->quad_vao);
	glBindVertexArray(draw_data->quad_vao);

	glGenBuffers(1, &draw_data->quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, draw_data->quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VP_ATTRIB_LOC);
	glBindBuffer(GL_ARRAY_BUFFER, draw_data->quad_vbo);
	glVertexAttribPointer(VP_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    draw_data->shader = init_shader("MVP_depth_bias.vert", "uniform_colour.frag");
    draw_data->colour_uniform_loc = glGetUniformLocation(draw_data->shader.id, "colour");

    draw_data->num_queued_points = 0;
    draw_data->num_queued_lines = 0;
}

void add_point(DebugDraw* draw_data, vec3 pos, float size, vec4 colour)
{
    assert(draw_data->num_queued_points + draw_data->num_queued_lines < MAX_NUM_DEBUG_DRAW_ELEMENTS);
    DebugDrawPoint point = {pos, size, colour};
    draw_data->points_queue[draw_data->num_queued_points++] = point;
}

void add_vec(DebugDraw* draw_data, vec3 origin, vec3 dir, vec4 colour)
{
    assert(draw_data->num_queued_points + draw_data->num_queued_lines < MAX_NUM_DEBUG_DRAW_ELEMENTS);
    DebugDrawLine line = {origin, dir, colour};
    draw_data->lines_queue[draw_data->num_queued_lines++] = line;

    add_point(draw_data, origin+dir, 0.15f, colour);
}

void debug_draw_flush(DebugDraw* draw_data, Camera3D &cam)
{
    mat4 model_mats[MAX_NUM_DEBUG_DRAW_ELEMENTS];
    vec4 colour_uniforms[MAX_NUM_DEBUG_DRAW_ELEMENTS];
    uint32_t drawables_count = 0;

    //Calculate inverse camera rotation matrix
    mat4 inverse_cam_rot_mat;
    {
        mat4 cam_rot_mat = cam.V;
        cam_rot_mat.m[12] = 0; 
        cam_rot_mat.m[13] = 0;
        cam_rot_mat.m[14] = 0;
        // Note: cam view matrix is orthogonal so can use transpose for a cheap inverse. 
        // Doesn't work for translation, but we only want rotation anyway so just clear it first
        inverse_cam_rot_mat = transpose(cam_rot_mat);
    }

    for(uint32_t point_index = 0; point_index < draw_data->num_queued_points; ++point_index)
    {
        DebugDrawPoint* point = &draw_data->points_queue[point_index];

        // Spherical Billboarding (quad normal is always -cam.fwd)
        mat4 M = inverse_cam_rot_mat;
        M = scale(M, point->size);
        M = translate(M, point->pos);

        model_mats[drawables_count] = M;
        colour_uniforms[drawables_count] = point->colour;
        ++drawables_count;
    }

    for(uint32_t line_index = 0; line_index < draw_data->num_queued_lines; ++line_index)
    {
        DebugDrawLine* line = &draw_data->lines_queue[line_index];

        mat4 M = translate(identity_mat4(), vec3{0, 0.5f, 0}); // move base of quad to origin
        float dist = length(line->dir);
        M = scale(M, vec3{0.05f, dist, 0.05f});

        // First rotate quad so its 'up' vector is dir
        mat4 R = rotate_align_mat4(vec3{0, 1, 0}, normalise(line->dir));

        // Billboarding - rotate quad around dir so its normal is facing camera (as much as possible)
        // From https://www.opengl.org/discussion_boards/archive/index.php/t-147495.html
        // "If you constrain your billboard to be rotating about only one axis, then the billboard's normal 
        // sweeps out a [disc on a] plane during this rotation. If we project the vector [to] the camera [from] the 
        // center of the billboard onto this plane, that will be the vector we want to [align the billboard normal with]."

        // Cool trick to project vector onto a plane: project it onto plane's normal
        // and subtract that projection from the original vector
        vec3 rot_sweep_plane_norm = (R * vec4{0,-1,0,0}).xyz; // normal of plane swept by rotating billboard's normal around dir
        vec3 billboard_center = line->origin + (0.5f * line->dir);
        vec3 v_to_cam = cam.pos - billboard_center;
        float norm_proj = dot(v_to_cam, rot_sweep_plane_norm); // project vec onto norm
        vec3 plane_proj = v_to_cam - (rot_sweep_plane_norm*norm_proj); // find projection onto plane (our target quad norm!)

        vec3 quad_norm = (R*vec4{0,0,1,0}).xyz;
        
        mat4 billboard_mat = rotate_align_mat4(quad_norm, normalise(plane_proj));

        M = billboard_mat * R * M;
        M = translate(M, line->origin);

        model_mats[drawables_count] = M;
        colour_uniforms[drawables_count] = line->colour;
        ++drawables_count;
    }

    glUseProgram(draw_data->shader.id);
    glBindVertexArray(draw_data->quad_vao);

    glUniformMatrix4fv(draw_data->shader.V_loc, 1, GL_FALSE, cam.V.m);
    glUniformMatrix4fv(draw_data->shader.P_loc, 1, GL_FALSE, cam.P.m);

    for(uint32_t drawable_index = 0; drawable_index < drawables_count; ++drawable_index)
    {
        glUniform4fv(draw_data->colour_uniform_loc, 1, colour_uniforms[drawable_index].v);
        glUniformMatrix4fv(draw_data->shader.M_loc, 1, GL_FALSE, model_mats[drawable_index].m);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    draw_data->num_queued_points = 0;
    draw_data->num_queued_lines = 0;
}
