#version 460 core

in vec2 v_tcoords;

out vec4 out_frag_color;

uniform sampler2D u_albedo;

void main() {
    out_frag_color = texture(u_albedo, v_tcoords);
}