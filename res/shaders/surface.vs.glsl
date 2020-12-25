#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 tex;

void main() {
    gl_Position = model * vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    tex_coords = (tex * vec4(aTexCoords.x, aTexCoords.y, 0.0f, 1.0f)).xy;
}
