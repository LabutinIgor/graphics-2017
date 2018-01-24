#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;

void main() {
    //color = vec3(0.5, 0.5, 0.5);
    color = texture(renderedTexture, UV).xyz;
}
