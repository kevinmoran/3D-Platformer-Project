#include "Mesh.h"

#include <stdlib.h> //free

#include "load_obj.h"
#include "Shader.h"
#include "string_functions.h"

bool load_mesh(Mesh* mesh, const char* obj_filename)
{
    copy_string(obj_filename, mesh->filename, MESH_FILENAME_LENGTH);

    bool result = load_obj_indexed(obj_filename, &mesh->vp, &mesh->vt, &mesh->vn, &mesh->indices, &mesh->num_verts, &mesh->num_indices);

    if(result)
    {
        glGenVertexArrays(1, &mesh->vao);
        glBindVertexArray(mesh->vao);
        
        glGenBuffers(1, &mesh->pos_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->pos_vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->num_verts*3*sizeof(float), mesh->vp, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VP_ATTRIB_LOC);
        glVertexAttribPointer(VP_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glGenBuffers(1, &mesh->norm_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->norm_vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->num_verts*3*sizeof(float), mesh->vn, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VN_ATTRIB_LOC);
        glVertexAttribPointer(VN_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glGenBuffers(1, &mesh->index_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices*sizeof(uint16), mesh->indices, GL_STATIC_DRAW);

        check_gl_error();
    }
    return result;
}

void clear_mesh(Mesh* mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->pos_vbo);
    glDeleteBuffers(1, &mesh->uvs_vbo);
    glDeleteBuffers(1, &mesh->norm_vbo);
    glDeleteBuffers(1, &mesh->index_vbo);

    free(mesh->vp);
    free(mesh->vn);
    free(mesh->vt);
    free(mesh->indices);

    mesh = {};
}
