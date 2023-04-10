#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../display/display.h"
#include "../render/render.h"
#include "../render/shader.h"
#include "../sim/sim.h"
#include "../util/loader.h"

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
    texture_t* texture = loader_load_texture("res/texture/normal_sphere.png");

    sim_t* sim = sim_create(64);

    shader_bind(shader);
    GLint location_mvp = uniform_find(shader, "u_mvp_matrix");

    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 vp;
    mat4 mvp;
    vec3 camera_trans = { -0.5f, -0.5f, -5.0f };
    glm_translate_make(view, camera_trans);
    while (!display_should_close(window)) {
        sim_step(sim);

        render_prepare();
        render_bind_texture(texture);
        float aspect = display_aspect(window);
        glm_perspective_default(aspect, proj);
        glm_mat4_mul(proj, view, vp);
        for (unsigned int i = 0; i < sim->n_particles; i++) {
            glm_translate_make(model, sim->particles[i].x);
            glm_scale_uni(model, 0.1f);
            glm_mat4_mul(vp, model, mvp);
            uniform_mat4(location_mvp, mvp);
            render_vao(vao);
        }

        display_refresh(window);
    }

    sim_free(sim);

    loader_free_texture(texture);
    loader_free_vao(vao);
    shader_free(shader);
    display_free(window);
    return 0;
}