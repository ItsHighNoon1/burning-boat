#version 460 core

layout(location = 0) in vec3 a_position;

out vec3 v_tcoords;

uniform mat4 u_mvp_matrix;

void main() {
    gl_Position = u_mvp_matrix * vec4(a_position, 1.0);
    v_tcoords = a_position;
}