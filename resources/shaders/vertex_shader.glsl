#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normal;

uniform mat4 matrixM;
uniform mat4 matrixVP;

void main() {
	gl_Position = matrixVP * matrixM * vec4(vertexPosition, 1);
}
