#version 460 core

in vec2 v_tcoords;

out vec4 out_frag_color;

uniform sampler2D u_color;
uniform sampler2D u_depth;
uniform vec3 u_light_dir;
uniform vec3 u_scatter_color;

void main() {
    /* Blinn phong (not good for water shading but helpful to debug)
    vec3 normal = normalize(texture(u_color, v_tcoords).xyz * 2.0 - 1.0);
    vec3 halfway = normalize(vec3(0.0, 0.0, 1.0) + normal);
    float diffuse_strength = dot(normal, u_light_dir);
    float specular_strength = clamp(pow(dot(normal, halfway), 32), 0.0, 1.0);
    vec3 diffuse_color = diffuse_strength * (1.0 - u_scatter_color);
    vec3 specular_color = vec3(specular_strength) * 0.6;
    */

    float water_thickness = texture(u_depth, v_tcoords).r;
    out_frag_color = vec4(u_scatter_color, water_thickness);
}