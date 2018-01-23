#version 330 core

in vec3 pos;
in vec3 n;
in vec4 shadowCoord;

uniform vec3 cameraPos;
uniform vec3 objDC;
uniform vec3 objSC;
uniform vec3 pointLightPos;
uniform float pointLightPow;
uniform float dirLightPow;
uniform vec3 dirLightDirection;

uniform sampler2DShadow shadowMap;

layout(location = 0) out vec3 color;


void main() {
    vec3 l = normalize(pointLightPos - pos);
    vec3 v = normalize(cameraPos - pos);
    vec3 h = normalize(l + v);
    vec3 colorDiffuse = clamp(dot(n, l), 0, 1) * objDC * pointLightPow / pow(length(pointLightPos - pos), 2);
    vec3 colorSpecular = pow(clamp(dot(n, h), 0, 1), 30.0f) * objSC;

    vec3 l1 = normalize(-dirLightDirection);
    vec3 h1 = normalize(l1 + v);
    vec3 colorDiffuse1 = clamp(dot(n, l1), 0, 1) * objDC * dirLightPow;
    vec3 colorSpecular1 = pow(clamp(dot(n, h1), 0, 1), 30.0f) * objSC;


    float visibility = texture(shadowMap, 0.5 * (1 + vec3(shadowCoord.xy, (shadowCoord.z) / shadowCoord.w)));
//	float bias = 0.005;
//    float visibility = 1.0;
//    if ( texture( shadowMap, (shadowCoord.xy/shadowCoord.w) ).z  <  (shadowCoord.z-bias)/shadowCoord.w ) {
//       visibility = 0.5;
//    }
	color = colorDiffuse + colorSpecular + visibility * (colorDiffuse1 + colorSpecular1);
}
