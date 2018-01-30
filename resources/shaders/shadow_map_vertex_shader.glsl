#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 matrixM;
uniform mat4 matrixVP;

out vec4 pos;

void main() {
	gl_Position = matrixVP * matrixM * vec4(vertexPosition, 1);
	pos = matrixVP * matrixM * vec4(vertexPosition, 1);
}
