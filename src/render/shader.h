#ifndef _shader_h_
#define _shader_h_

#include <cglm/cglm.h>
#include <glad/glad.h>

typedef struct {
    GLuint program;
} shader_t;

shader_t* shader_create(const char* vert_path, const char* frag_path);

shader_t* shader_create_compute(const char* path);

void shader_bind(shader_t* shader);

void shader_free(shader_t* shader);

GLint uniform_find(shader_t* shader, const char* name);

void uniform_mat4(GLint location, mat4 m);

void uniform_vec3(GLint location, vec3 v);

void uniform_float(GLint location, float f);

void uniform_int(GLint location, int i);

GLuint ssbo_find(shader_t* shader, const char* name);

#endif