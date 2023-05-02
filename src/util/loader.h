#ifndef _loader_h_
#define _loader_h_

#include <glad/glad.h>

typedef struct {
    GLuint array;
    GLuint vertex_buffer;
    GLuint index_buffer;
    unsigned int vertex_count;
} vao_t;

typedef struct {
    GLuint texture;
} texture_t;

typedef struct {
    GLuint ssbo;
    unsigned int size;
} ssbo_t;

vao_t* loader_load_vao(float* positions, float* tcoords, float* normals, unsigned int* indices, unsigned int n_vertices, unsigned int n_indices);

texture_t* loader_load_texture(const char* path);

ssbo_t* loader_load_ssbo(void* data, unsigned int size);

void loader_update_ssbo(ssbo_t* ssbo, void* data, unsigned int size);

void loader_get_ssbo(ssbo_t* ssbo, void* buffer, unsigned int size);

void loader_free_vao(vao_t* vao);

void loader_free_texture(texture_t* texture);

void loader_free_ssbo(ssbo_t* ssbo);

#endif