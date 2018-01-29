#version 330 core

in vec3 pos;
in vec3 n;

uniform vec3 cameraPos;
uniform vec3 objDC;
uniform vec3 objSC;
uniform vec3 pointLightPos;

float decay = 0.96815;
float exposure = 0.2;
float density = 0.926;
float weight = 0.58767;
int numSamples = 100;

layout(location = 0) out vec3 outColor;


void main() {
    vec3 l = normalize(pointLightPos - pos);
    vec3 v = normalize(cameraPos - pos);
    vec3 h = normalize(l + v);
    vec3 colorDiffuse = clamp(dot(n, l), 0, 1) * objDC;
    vec3 colorSpecular = pow(clamp(dot(n, h), 0, 1), 30.0f) * objSC;
	vec4 sceneColor = vec4((colorDiffuse + colorSpecular) / 2, 1);

    outColor = sceneColor.xyz;
}
