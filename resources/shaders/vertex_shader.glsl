#version 330 core

layout(location = 0) in vec3 vertexPosition;
out vec4 shadowCoord;

uniform mat4 matrixM;
uniform mat4 matrixVP;
uniform mat4 dirLightVP;

void main() {
	gl_Position = matrixVP * matrixM * vec4(vertexPosition, 1);

	shadowCoord = dirLightVP * matrixM * vec4(vertexPosition, 1);
}
