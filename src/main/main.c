#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../display/display.h"
#include "../render/loader.h"
#include "../render/render.h"
#include "../render/shader.h"

int main(int argc, char** argv) {
    GLFWwindow* window = display_create(800, 500, "Boat");

    float vertices[] = { 
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    float tcoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    shader_t* shader = shader_create("res/shader/billboard.vert", "res/shader/billboard.frag");
    vao_t* vao = loader_load_vao(vertices, tcoords, NULL, indices, 4, 6);
    texture_t* texture = loader_load_texture("res/texture/hienka.gif");

    shader_bind(shader);
    GLint location_mvp = uniform_find(shader, "u_mvp_matrix");
    mat4 mvp;
    glm_mat4_identity(mvp);
    while (!display_should_close(window)) {
        uniform_mat4(location_mvp, mvp);
        render_prepare();
        render_bind_texture(texture);
        render_vao(vao);
        display_refresh(window);
    }

    loader_free_texture(texture);
    loader_free_vao(vao);
    shader_free(shader);
    display_free(window);
    return 0;
}