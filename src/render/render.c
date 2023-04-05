#include "render.h"

#include <glad/glad.h>

#include "rentypes.h"

void render_vao(vao_t* vao) {
    glBindVertexArray(vao->array);
    glDrawElements(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0);
}