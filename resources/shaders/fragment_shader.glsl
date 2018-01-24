#version 330 core

//in vec3 pos;
//in vec3 n;

//uniform vec3 cameraPos;
//uniform vec3 objDC;
//uniform vec3 objSC;
//uniform vec3 pointLightPos;
//uniform float pointLightPow;

uniform sampler2D textureDC;
uniform sampler2D textureSC;
uniform sampler2D textureNormals;
uniform sampler2D texturePositins;

layout(location = 0) out vec3 color;


void main() {
//    vec3 l = normalize(pointLightPos - pos);
//    vec3 v = normalize(cameraPos - pos);
//    vec3 h = normalize(l + v);
//    vec3 colorDiffuse = clamp(dot(n, l), 0, 1) * objDC * pointLightPow / pow(length(pointLightPos - pos), 2.0);
//    vec3 colorSpecular = pow(clamp(dot(n, h), 0, 1), 30.0f) * objSC;

    color = vec3(0.4, 0.4, 0.4);
//	color = colorDiffuse + colorSpecular;
}
