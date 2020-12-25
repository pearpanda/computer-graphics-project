#version 460 core

struct Material {
    sampler2D texture_diffuse1;
};

out vec4 FragColor;
in vec2 tex_coords;

uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse1, tex_coords);
}