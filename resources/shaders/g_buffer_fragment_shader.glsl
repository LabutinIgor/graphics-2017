#version 330 core

in vec3 pos;
in vec3 n;

uniform vec3 cameraPos;
uniform vec3 objDC;
uniform vec3 objSC;
uniform vec3 pointLightPos;
uniform float pointLightPow;

layout(location = 0) out vec3 diffuseColor;
layout(location = 1) out vec3 specularColor;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 position;

void main() {
    diffuseColor = objDC;
    specularColor = objSC;
    normal = n;
    position = pos;
}
