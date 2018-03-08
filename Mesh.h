#pragma once
#include "utils.h"
#include "gl_lite.h"

#define MESH_FILENAME_LENGTH 32

struct Mesh
{
    char filename[MESH_FILENAME_LENGTH];
    GLuint vao;
    GLuint pos_vbo;
    GLuint uvs_vbo;
    GLuint norm_vbo;
    GLuint index_vbo;

    float* vp;
    float* vn;
    float* vt;
    uint16* indices;

    uint32 num_indices;
    uint32 num_verts;
};

bool load_mesh(Mesh* mesh, const char* obj_filename);
void clear_mesh(Mesh* mesh);
