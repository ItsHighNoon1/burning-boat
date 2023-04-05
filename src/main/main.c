#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../display/display.h"
#include "../render/loader.h"
#include "../render/render.h"
#include "../render/rentypes.h"

int main(int argc, char** argv) {
    GLFWwindow* window = display_create(800, 500, "Boat");

    float vertices[] = { 
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    vao_t* vao = loader_load_vao(vertices, NULL, NULL, indices, 4, 6);

    while (!display_should_close(window)) {
        render_vao(vao);
        display_refresh(window);
    }

    loader_free_vao(vao);
    display_free(window);
    return 0;
}