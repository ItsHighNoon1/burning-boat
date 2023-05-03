#version 460 core

in vec2 v_tcoords;

out vec4 out_frag_color;

uniform sampler2D u_color;
uniform sampler2D u_depth;
uniform samplerCube u_environment;
uniform mat4 u_inverse_view;
uniform vec3 u_light_dir;
uniform vec3 u_scatter_color;

void main() {
    vec3 in_color = texture(u_color, v_tcoords).xyz;
    if (in_color == vec3(0.0, 0.0, 0.0)) {
        discard;
    }
    vec3 normal = normalize(in_color * 2.0 - 1.0);
    vec3 reflect_dir = reflect(vec3(0.0, 0.0, -1.0), normal);
    vec3 true_reflect_dir = (u_inverse_view * vec4(reflect_dir, 0.0)).xyz;

    // Surface color (low angle)
    vec3 reflect_color = texture(u_environment, true_reflect_dir).rgb;
    float specular_strength = clamp(pow(dot(reflect_dir, u_light_dir), 31), 0.0, 1.0);
    vec3 specular_color = vec3(specular_strength) * 0.6;
    vec4 surface_color = vec4(reflect_color + specular_color, 1.0);

    // Look through color (high angle)
    float water_thickness = texture(u_depth, v_tcoords).r;
    vec4 through_color = vec4(u_scatter_color * water_thickness, water_thickness);

    // Fresnel
    float fresnel_factor = pow(1 - dot(vec3(0.0, 0.0, 1.0), normal), 1);
    out_frag_color = mix(through_color, surface_color, fresnel_factor);
    //out_frag_color = surface_color;
}