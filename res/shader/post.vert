#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tcoords;

out vec2 v_tcoords;

void main() {
    gl_Position = vec4(a_position * 2.0, 1.0);
    v_tcoords = a_tcoords;
}