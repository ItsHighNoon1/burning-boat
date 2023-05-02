#version 460 core

struct particle_t {
    mat4 model_view_matrix;
    vec3 x;
    float rho;
    vec3 vh;
    float pad1;
    vec3 v;
    float pad2;
    vec3 a;
    float pad3;
};

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tcoords;
layout(location = 2) in vec3 a_normal;

layout(std430, binding = 1) readonly buffer s_particles {
    particle_t particles[];
};

out vec3 v_eye_pos;
out vec2 v_tcoords;
out float v_sphere_radius;

uniform mat4 u_projection_matrix;

void main() {
    vec4 eye_pos = particles[gl_InstanceID].model_view_matrix * vec4(a_position, 1.0);
    vec4 clip_pos = u_projection_matrix * eye_pos;

    v_eye_pos = eye_pos.xyz;
    v_tcoords = a_tcoords;
    v_sphere_radius = abs(max(a_position.x, a_position.y)) / clip_pos.w;
    v_sphere_radius = clip_pos.z / clip_pos.w;
    gl_Position = clip_pos;
}