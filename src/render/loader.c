#include "loader.h"

#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <stb_image.h>

#define N_POSITIONS 3
#define N_TCOORDS 2
#define N_NORMALS 3

vao_t* loader_load_vao(float* positions, float* tcoords, float* normals, unsigned int* indices, unsigned int n_vertices, unsigned int n_indices) {
    // Allocate objects
    vao_t* vao = malloc(sizeof(vao_t));
    GLuint buffers[2];
    glGenBuffers(2, buffers);
    glGenVertexArrays(1, &(vao->array));
    glBindVertexArray(vao->array);
    vao->vertex_buffer = buffers[0];
    vao->index_buffer = buffers[1];

    // Calculate stride
    unsigned int stride = 0;
    if (positions) stride += N_POSITIONS;
    if (tcoords) stride += N_TCOORDS;
    if (normals) stride += N_NORMALS;

    // Pack vertex data
    unsigned int tcoords_offset = 0;
    unsigned int normals_offset = 0;
    float* vertex_data = malloc(n_vertices * stride * sizeof(float));
    if (positions) {
        tcoords_offset += N_POSITIONS;
        normals_offset += N_POSITIONS;
        for (int i = 0; i < n_vertices; i++) {
            vertex_data[i * stride] = positions[i * N_POSITIONS];
            vertex_data[i * stride + 1] = positions[i * N_POSITIONS + 1];
            vertex_data[i * stride + 2] = positions[i * N_POSITIONS + 2];
        }
    }
    if (tcoords) {
        normals_offset += N_TCOORDS;
        for (int i = 0; i < n_vertices; i++) {
            vertex_data[i * stride] = tcoords[i * N_TCOORDS];
            vertex_data[i * stride + 1] = tcoords[i * N_TCOORDS + 1];
        }
    }
    if (normals) {
        for (int i = 0; i < n_vertices; i++) {
            vertex_data[i * stride] = normals[i * N_NORMALS];
            vertex_data[i * stride + 1] = normals[i * N_NORMALS + 1];
            vertex_data[i * stride + 2] = normals[i * N_NORMALS + 2];
        }
    }

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vao->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, n_vertices * stride * sizeof(float), vertex_data, GL_STATIC_DRAW);
    free(vertex_data);

    // Set vertex attribs
    if (positions) {
        glVertexAttribPointer(0, N_POSITIONS, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    if (tcoords) {
        glVertexAttribPointer(1, N_TCOORDS, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(tcoords_offset * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    if (normals) {
        glVertexAttribPointer(2, N_NORMALS, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(normals_offset * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    vao->vertex_count = n_indices;

    return vao;
}

texture_t* loader_load_texture(const char* path) {
    // stb load
    int width;
    int height;
    int comp;
    stbi_uc* data = stbi_load(path, &width, &height, &comp, 4);

    // Allocate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // stb free
    stbi_image_free(data);

    // Return texture info
    texture_t* texture_struct = malloc(sizeof(texture_t));
    texture_struct->texture = texture;
    return texture_struct;
}

void loader_free_vao(vao_t* vao) {
    // Free objects
    GLuint buffers[] = { vao->vertex_buffer, vao->index_buffer };
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &(vao->array));
    free(vao);
}

void loader_free_texture(texture_t* texture) {
    glDeleteTextures(1, &(texture->texture));
    free(texture);
}