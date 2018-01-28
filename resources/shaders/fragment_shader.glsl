#version 330 core

in vec3 pos;
in vec3 n;

uniform vec3 cameraPos;
uniform vec3 objDC;
uniform vec3 objSC;
uniform vec3 pointLightPos;
uniform vec3 dirLightDirection;
uniform sampler2DShadow shadowMap;

out vec4 color;

void main() {
    vec3 l = normalize(pointLightPos - pos);
    vec3 v = normalize(cameraPos - pos);
    vec3 h = normalize(l + v);
    vec3 colorDiffuse = clamp(dot(n, l), 0, 1) * objDC / pow(length(pointLightPos - pos), 2);
    vec3 colorSpecular = pow(clamp(dot(n, h), 0, 1), 30.0f) * objSC;

	color = vec4((colorDiffuse + colorSpecular) / 2, 1);
}
