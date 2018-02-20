#pragma once

#define VP_ATTRIB_LOC 0
#define VT_ATTRIB_LOC 1
#define VN_ATTRIB_LOC 2

struct Shader {
    GLuint id;
    GLuint M_loc, V_loc, P_loc;
    bool compiled;
};

Shader init_shader(const char* vert_file, const char* frag_file);
bool reload_shader_program(const char* vert_file, const char* frag_file, Shader* s);
void delete_program(Shader* s);
