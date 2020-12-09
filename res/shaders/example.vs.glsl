#version 460 core

layout (location = 0) in vec2 aPos;
uniform mat2 rot;

void main() {
    vec2 tmp = rot * aPos;
    gl_Position = vec4(tmp.x, tmp.y, 0.0f, 1.0f);
}
