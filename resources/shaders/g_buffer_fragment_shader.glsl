#version 330 core

uniform vec3 colToGR;

layout(location = 0) out vec3 colGR;

void main() {
    colGR = colToGR;
}
