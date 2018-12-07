#include "Shader.h"

#include <stdio.h>

#include "utils.h"
#include "string_functions.h"

#define SHADERS_FOLDER "Shaders/"

//Internal functions
static bool _load_shader_program(Shader* shader, const char* vert_file, const char* frag_file);
static bool _load_and_compile_shader(GLuint* handle, const char* filename, GLuint shader_type);

Shader init_shader(const char* vert_file, const char* frag_file)
{
    Shader result = {};
    result.vert_file = vert_file;
    result.frag_file = frag_file;

    if(!_load_shader_program(&result, vert_file, frag_file)){
        //handle failure? default shader program?
        return result;
    }
    result.compiled = true;
    result.M_loc = glGetUniformLocation(result.id, "M");
    result.V_loc = glGetUniformLocation(result.id, "V");
    result.P_loc = glGetUniformLocation(result.id, "P");
    return result;
}

bool reload_shader_program(Shader* shader){
    delete_program(shader);
    if(!_load_shader_program(shader, shader->vert_file, shader->frag_file)) {
        fprintf(stderr, "ERROR in reload_shader_program using vert shader %s and frag shader %s", shader->vert_file, shader->frag_file);
        return false;
    }
    shader->M_loc = glGetUniformLocation(shader->id, "M");
    shader->V_loc = glGetUniformLocation(shader->id, "V");
    shader->P_loc = glGetUniformLocation(shader->id, "P");
    return true;
}

void delete_program(Shader* s){
    if(s->compiled) {
        glDeleteProgram(s->id);

        s->id = -1;
        s->M_loc = -1;
        s->V_loc = -1;
        s->P_loc = -1;
        s->compiled = false;
    }
}

static bool _load_shader_program(Shader* shader, const char* vert_file, const char* frag_file)
{
    GLuint vs, fs;
    if(!_load_and_compile_shader(&vs, vert_file, GL_VERTEX_SHADER))
        return false;
    if(!_load_and_compile_shader(&fs, frag_file, GL_FRAGMENT_SHADER))
        return false;
    
    //Create and link program
    shader->id = glCreateProgram();
    glAttachShader(shader->id, vs);
    glAttachShader(shader->id, fs);

    glBindAttribLocation(shader->id, VP_ATTRIB_LOC, "vp");
    glBindAttribLocation(shader->id, VT_ATTRIB_LOC, "vt");
    glBindAttribLocation(shader->id, VN_ATTRIB_LOC, "vn");
    glBindAttribLocation(shader->id, VBONE_IDS_ATTRIB_LOC, "boneIDs");
    glBindAttribLocation(shader->id, VBONE_WEIGHTS_ATTRIB_LOC, "boneWeights");

    glLinkProgram(shader->id);
    
    bool result = true;

    //Check for linking errors
    {
        GLint params = -1;
        glGetProgramiv(shader->id, GL_LINK_STATUS, &params);
        if(params != GL_TRUE){
            int log_length = 0;
            char prog_log[2048];
            glGetProgramInfoLog(shader->id, 2048, &log_length, prog_log);
            fprintf(stderr, "ERROR linking shader program. Program info log:\n%s\n", prog_log);
            result = false;
        }
    }
    glDetachShader(shader->id, vs);
    glDetachShader(shader->id, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return result;
}

static bool _load_and_compile_shader(GLuint* handle, const char* filename, GLuint shader_type)
{
    assert((shader_type == GL_VERTEX_SHADER) || (shader_type == GL_FRAGMENT_SHADER));

    //Read shader file into buffer
    const int SHADER_BUFFER_LENGTH = 65536;
    char shader_string[SHADER_BUFFER_LENGTH];
    {
        shader_string[0] = '\0';
        const int FILENAME_BUFFER_LENGTH = 64;
        char full_file_path[FILENAME_BUFFER_LENGTH];
        concat_strings(SHADERS_FOLDER, filename, full_file_path);
        
        FILE *fp = fopen(full_file_path, "rb");
        if(!fp){
            fprintf(stderr, "ERROR opening shader file: %s\n", full_file_path);
            return false;
        }
        
        fseek(fp, 0, SEEK_END);
        int file_length = ftell(fp);
        assert(file_length < (SHADER_BUFFER_LENGTH - 1));
        // char *shader_string = malloc(file_length + 1);

        rewind(fp);
        fread(shader_string, SHADER_BUFFER_LENGTH, 1, fp);
        shader_string[file_length] = 0;

        fclose(fp);
    }
    //Compile shader
    {
        *handle = glCreateShader(shader_type);
        const char* temp = shader_string;
        glShaderSource(*handle, 1, &temp, NULL);
        glCompileShader(*handle);
        
        //Check for compilation errors
        GLint params = -1;
        glGetShaderiv(*handle, GL_COMPILE_STATUS, &params);
        if(params != GL_TRUE){
            int log_length = 0;
            char shader_log[2048];
            glGetShaderInfoLog(*handle, 2048, &log_length, shader_log);
            fprintf(stderr, "ERROR: shader `%s` did not compile. Shader info log:\n%s\n", filename, shader_log);
            glDeleteShader(*handle);
            return false;
        }
    }
    return true;
}
