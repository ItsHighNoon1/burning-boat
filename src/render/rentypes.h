#ifndef _rentypes_h_
#define _rentypes_h_

#include <glad/glad.h>

typedef struct {
    GLuint array;
    GLuint vertex_buffer;
    GLuint index_buffer;
    unsigned int vertex_count;
} vao_t;

#endif