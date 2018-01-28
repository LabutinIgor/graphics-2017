#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normal;

uniform mat4 matrixVP;
uniform mat4 matrixM;

out vec3 pos;
out vec3 n;

void main() {
	gl_Position = matrixVP * matrixM * vec4(vertexPosition, 1);
	pos = (matrixM * vec4(vertexPosition, 1)).xyz;

	n = normal;
}
