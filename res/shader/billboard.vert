#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tcoords;
layout(location = 2) in vec3 a_normal;

out vec3 v_eye_pos;
out vec2 v_tcoords;
out float v_sphere_radius;

uniform mat4 u_model_view_matrix;
uniform mat4 u_projection_matrix;

void main() {
    vec4 eye_pos = u_model_view_matrix * vec4(a_position, 1.0);
    vec4 clip_pos = u_projection_matrix * eye_pos;

    v_eye_pos = eye_pos.xyz;
    v_tcoords = a_tcoords;
    v_sphere_radius = abs(max(a_position.x, a_position.y)) / clip_pos.w;
    v_sphere_radius = clip_pos.z / clip_pos.w;
    gl_Position = clip_pos;
}