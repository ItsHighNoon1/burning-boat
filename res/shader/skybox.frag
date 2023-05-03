#version 460 core

in vec3 v_tcoords;

out vec4 out_frag_color;

uniform samplerCube u_skybox;

void main() {
    out_frag_color = texture(u_skybox, v_tcoords);
}