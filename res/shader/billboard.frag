#version 460 core

in vec3 v_eye_pos;
in vec2 v_tcoords;
in float v_sphere_radius;

out vec4 out_frag_color;

uniform mat4 u_projection_matrix;
uniform vec3 u_debug;

void main() {
    vec3 normal;
    normal.xy = v_tcoords * 2.0 - 1.0;
    float r2 = dot(normal.xy, normal.xy);
    if (r2 > 1.0) {
        discard;
    }
    normal.z = sqrt(1.0 - r2);

    vec4 clip_pos = u_projection_matrix * vec4(v_eye_pos + normal * v_sphere_radius, 1.0);
    gl_FragDepth = clip_pos.z / clip_pos.w;

    out_frag_color = vec4(u_debug * max(normal.y, 0.1), 1.0);
}