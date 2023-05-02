#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../display/display.h"
#include "../render/render.h"
#include "../render/shader.h"
#include "../sim/sim.h"
#include "../util/loader.h"

// Vertex data (only quads are rendered)
static const float vertices[] = { 
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

static const float tcoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

static const unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
};

int main(int argc, char** argv) {
    // Window
    GLFWwindow* window = display_create(800, 500, "Boat");

    // OpenGL objects
    shader_t* geometry_buffer_shader = shader_create("res/shader/billboard.vert", "res/shader/billboard_normals.frag");
    shader_t* splat_buffer_shader = shader_create("res/shader/billboard.vert", "res/shader/billboard_depth.frag");
    shader_t* bilateral_filter_shader = shader_create("res/shader/post.vert", "res/shader/post_bilateral.frag");
    shader_t* deferred_shader = shader_create("res/shader/post.vert", "res/shader/post_deferred.frag");
    vao_t* vao = loader_load_vao(vertices, tcoords, NULL, indices, 4, 6);
    fbo_t* geometry_buffer = loader_load_framebuffer(800, 500);
    fbo_t* splat_buffer = loader_load_framebuffer(800, 500);
    fbo_t* smoothed_geometry_buffer = loader_load_framebuffer(800, 500);

    // Uniforms and objects to store them
    GLint location_geometry_projection = uniform_find(geometry_buffer_shader, "u_projection_matrix");
    GLint location_splat_projection = uniform_find(splat_buffer_shader, "u_projection_matrix");
    GLint location_bilateral_color = uniform_find(bilateral_filter_shader, "u_color");
    GLint location_bilateral_depth = uniform_find(bilateral_filter_shader, "u_depth");
    GLint location_bilateral_texel_size = uniform_find(bilateral_filter_shader, "u_texel_size");
    GLint location_deferred_color = uniform_find(deferred_shader, "u_color");
    GLint location_deferred_depth = uniform_find(deferred_shader, "u_depth");
    GLint location_deferred_light_dir = uniform_find(deferred_shader, "u_light_dir");
    GLint location_deferred_scatter_color = uniform_find(deferred_shader, "u_scatter_color");
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 eye_light_dir;
    vec3 scatter_color;
    
    // Behavior objects
    vec3 camera_trans = { 24.0f, -24.0f, -24.0f };
    sim_t* sim = sim_create(5000);
    
    while (!display_should_close(window)) {
        // Run the simulation
        sim_step(sim);

        // Calculate proj matrix
        float aspect = display_aspect(window);
        glm_perspective_default(aspect, proj);

        // Calculate view matrix
        glm_mat4_identity(view);
        glm_rotate_x(view, 0.5f, view);
        glm_rotate_y(view, 0.8f, view);
        glm_translate(view, camera_trans);

        // Calculate light dir
        eye_light_dir[0] = 0.0f;
        eye_light_dir[1] = 1.0f;
        eye_light_dir[2] = 1.0f;
        glm_mat4_mulv3(view, eye_light_dir, 0.0, eye_light_dir);
        glm_normalize(eye_light_dir);

        // Scatter color
        scatter_color[0] = 0.4f;
        scatter_color[1] = 0.7f;
        scatter_color[2] = 0.9f;

        // Calculate model and MV matrices
        for (unsigned int i = 0; i < sim->n_particles; i++) {
            glm_translate_make(model, sim->particles[i].x);
            glm_rotate_y(model, -0.8f, model);
            glm_rotate_x(model, -0.5f, model);
            glm_scale_uni(model, 2.0f);
            glm_mat4_mul(view, model, sim->particles[i].model_view_matrix);
        }

        // Upload to SSBO
        loader_update_ssbo(sim->ssbo, sim->particles, sim->n_particles * sizeof(particle_t));
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // Resize FBOs
        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);
        loader_resize_framebuffer(geometry_buffer, width, height);
        loader_resize_framebuffer(splat_buffer, width, height);
        loader_resize_framebuffer(smoothed_geometry_buffer, width, height);

        // Draw geometry buffer
        render_prepare(geometry_buffer);
        shader_bind(geometry_buffer_shader);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, sim->ssbo->ssbo);
        uniform_mat4(location_geometry_projection, proj);
        glDrawElementsInstanced(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0, sim->n_particles);

        // Draw splat buffer
        render_prepare(splat_buffer);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        shader_bind(splat_buffer_shader);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, sim->ssbo->ssbo);
        uniform_mat4(location_splat_projection, proj);
        glDrawElementsInstanced(GL_TRIANGLES, vao->vertex_count, GL_UNSIGNED_INT, 0, sim->n_particles);
        glDisable(GL_BLEND);

        // Silhouette sensitive blur on the geometry buffer
        render_prepare(smoothed_geometry_buffer);
        shader_bind(bilateral_filter_shader);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, geometry_buffer->depth_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, geometry_buffer->color_texture);
        uniform_int(location_bilateral_color, 0);
        uniform_int(location_bilateral_depth, 1);
        vec2 texel_size = { 1.0f / width, 1.0f / height };
        uniform_vec2(location_bilateral_texel_size, texel_size);
        render_vao(vao);

        // Draw to screen
        render_prepare(NULL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader_bind(deferred_shader);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, splat_buffer->color_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, smoothed_geometry_buffer->color_texture);
        uniform_int(location_deferred_color, 0);
        uniform_int(location_deferred_depth, 1);
        uniform_vec3(location_deferred_light_dir, eye_light_dir);
        uniform_vec3(location_deferred_scatter_color, scatter_color);
        render_vao(vao);
        glDisable(GL_BLEND);

        // Refresh display
        display_refresh(window);
    }

    // Clean up
    sim_free(sim);
    loader_free_framebuffer(smoothed_geometry_buffer);
    loader_free_framebuffer(splat_buffer);
    loader_free_framebuffer(geometry_buffer);
    loader_free_vao(vao);
    shader_free(deferred_shader);
    shader_free(bilateral_filter_shader);
    shader_free(splat_buffer_shader);
    shader_free(geometry_buffer_shader);
    display_free(window);
    return 0;
}