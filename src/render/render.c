#include "render.h"

#include <glad/glad.h>

#include "loader.h"

void render_prepare() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_vao(vao_t* vao) {
    glBindVertexArray(vao->array);
    glDrawElements(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0);
}

void render_bind_texture(texture_t* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->texture);
}