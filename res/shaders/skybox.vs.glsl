#version 460 core
layout(location = 0) in vec3 aPos;

out vec3 tex_coords;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    vec4 pos = projection_matrix * view_matrix * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;
    tex_coords = aPos;
}