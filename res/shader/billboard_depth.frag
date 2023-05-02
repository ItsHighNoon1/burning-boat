#version 460 core

in vec3 v_eye_pos;
in vec2 v_tcoords;
in float v_sphere_radius;

out vec4 out_frag_color;

uniform mat4 u_projection_matrix;

void main() {
    vec3 normal;
    normal.xy = v_tcoords * 2.0 - 1.0;
    float r2 = dot(normal.xy, normal.xy);
    if (r2 > 1.0) {
        discard;
    }

    out_frag_color = vec4(1.0 - r2) * 0.01;
}