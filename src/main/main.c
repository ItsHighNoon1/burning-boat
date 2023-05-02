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

    GLuint location_particles = ssbo_find(shader, "s_particles");
    
    GLint location_projection = uniform_find(shader, "u_projection_matrix");
    mat4 model;
    mat4 view;
    mat4 proj;
    
    vec3 camera_trans = { 12.0f, -12.0f, -12.0f };

    sim_t* sim = sim_create(5000);
    
    while (!display_should_close(window)) {
        sim_step(sim);

        glm_mat4_identity(view);
        glm_rotate_x(view, 0.5f, view);
        glm_rotate_y(view, 0.8f, view);
        glm_translate(view, camera_trans);
        float aspect = display_aspect(window);
        glm_perspective_default(aspect, proj);

        render_prepare();
        render_bind_texture(texture);
        shader_bind(shader);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, sim->ssbo->ssbo);

        uniform_mat4(location_projection, proj);

        for (unsigned int i = 0; i < sim->n_particles; i++) {
            glm_translate_make(model, sim->particles[i].x);
            glm_rotate_y(model, -0.8f, model);
            glm_rotate_x(model, -0.5f, model);
            glm_scale_uni(model, 1.0f);
            mat4 mv;
            glm_mat4_mul(view, model, mv);
            glm_mat4_copy(mv, sim->particles[i].model_view_matrix);
        }

        loader_update_ssbo(sim->ssbo, sim->particles, sim->n_particles * sizeof(particle_t));
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        glDrawElementsInstanced(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0, sim->n_particles);

        display_refresh(window);
    }

    sim_free(sim);

    loader_free_texture(texture);
    loader_free_vao(vao);
    shader_free(shader);
    display_free(window);
    return 0;
}