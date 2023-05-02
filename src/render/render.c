#include "render.h"

#include <glad/glad.h>

#include "../util/loader.h"

void render_prepare(fbo_t* fbo) {
    if (fbo) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo->framebuffer);
    } else {
        glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void render_vao(vao_t* vao) {
    glBindVertexArray(vao->array);
    glDrawElements(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0);
}