#version 460 core

struct Material {
    sampler2D texture_diffuse1;
};

in vec2 tex_coords;
out vec4 frag_color;
uniform Material material;

void main() {
    frag_color = texture(material.texture_diffuse1, tex_coords);
}
