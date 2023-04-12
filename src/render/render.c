#include "render.h"

#include <glad/glad.h>

#include "../util/loader.h"

void render_prepare() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void render_vao(vao_t* vao) {
    glBindVertexArray(vao->array);
    glDrawElements(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0);
}

void render_bind_texture(texture_t* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->texture);
}