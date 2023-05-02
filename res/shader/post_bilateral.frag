#version 460 core

in vec2 v_tcoords;

out vec4 out_frag_color;

uniform sampler2D u_color;
uniform sampler2D u_depth;
uniform vec2 u_texel_size;

const vec2 smoothing_rad = 10.0 * u_texel_size;

void main() {
    // This is not a bilateral filter but it will work
    vec3 color_sum = vec3(0.0);
    float depth_sum = 0.0;
    float w_sum = 0.0;

    float base_depth = texture(u_depth, v_tcoords).r;

    for (float x = -smoothing_rad.x; x <= smoothing_rad.x; x += u_texel_size.x) {
        for (float y = -smoothing_rad.y; y <= smoothing_rad.y; y += u_texel_size.y) {
            vec2 offset_coords = v_tcoords + vec2(x, y);
            float this_depth = texture(u_depth, offset_coords).r;
            float depth_diff = this_depth - base_depth;

            float should_add = float(base_depth != 1.0 && this_depth != 1.0 && depth_diff < 0.00005);
            float weight = exp(-(x * x + y * y)) * exp(-(depth_diff * depth_diff)) * should_add;
            color_sum += texture(u_color, clamp(offset_coords, 0.0, 1.0)).rgb * weight;
            depth_sum += this_depth * weight;
            w_sum += weight;
        }
    }

    out_frag_color = vec4(color_sum / w_sum, 1.0);
    gl_FragDepth = depth_sum / w_sum;
}