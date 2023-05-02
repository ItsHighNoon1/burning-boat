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
    GLuint color_texture;
    GLuint depth_texture;
    GLuint framebuffer;
    int width;
    int height;
} fbo_t;

typedef struct {
    GLuint ssbo;
    unsigned int size;
} ssbo_t;

vao_t* loader_load_vao(const float* positions, const float* tcoords, const float* normals, const unsigned int* indices, unsigned int n_vertices, unsigned int n_indices);

texture_t* loader_load_texture(const char* path);

fbo_t* loader_load_framebuffer(int width, int height);

ssbo_t* loader_load_ssbo(void* data, unsigned int size);

void loader_resize_framebuffer(fbo_t* fbo, int width, int height);

void loader_update_ssbo(ssbo_t* ssbo, void* data, unsigned int size);

void loader_get_ssbo(ssbo_t* ssbo, void* buffer, unsigned int size);

void loader_free_vao(vao_t* vao);

void loader_free_texture(texture_t* texture);

void loader_free_framebuffer(fbo_t* fbo);

void loader_free_ssbo(ssbo_t* ssbo);

#endif