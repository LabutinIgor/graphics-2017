#version 330 core

in vec4 shadowCoord;

layout(location = 0) out vec3 color;

uniform sampler2DShadow shadowMap;

void main() {
    float visibility = texture(shadowMap, 0.5 * (1 + vec3(shadowCoord.xy, (shadowCoord.z) / shadowCoord.w)));

//	float bias = 0.005;
//    float visibility = 1.0;
//    if ( texture( shadowMap, (shadowCoord.xy/shadowCoord.w) ).z  <  (shadowCoord.z-bias)/shadowCoord.w ) {
//       visibility = 0.5;
//    }
	color = vec3(visibility, visibility, visibility);
//	color = vec3(0.5, 0.5, 0.5);
}
