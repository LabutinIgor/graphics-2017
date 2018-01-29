#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 matrixVP;
uniform vec3 lPos;

out vec4 lightPos;

void main(){
	gl_Position = vec4(vertexPosition, 1);
	lightPos = (matrixVP * vec4(lPos, 1));
}

