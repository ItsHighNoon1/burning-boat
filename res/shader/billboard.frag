#version 460 core

in vec2 v_tcoords;

out vec4 out_frag_color;

uniform sampler2D u_albedo;

void main() {
    vec4 tex_sample = texture(u_albedo, v_tcoords);
    if (tex_sample.a < 0.5) {
        discard;
    }
    out_frag_color = tex_sample;
}