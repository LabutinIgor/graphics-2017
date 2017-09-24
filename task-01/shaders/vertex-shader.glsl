#version 330 core

in vec3 position;
out vec2 pos;

void main() {
    pos = position.xy;
    gl_Position.xyz = position;
    gl_Position.w = 1.0;
}
