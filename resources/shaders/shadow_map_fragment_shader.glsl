#version 330

in vec4 pos;

layout(location = 0) out vec3 p;

void main() {
    p = 0.5 * pos.xyz / pos.w + 0.5;
}
