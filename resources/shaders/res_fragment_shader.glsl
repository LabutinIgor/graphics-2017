#version 330

uniform sampler2D textureGR;
uniform sampler2D textureColor;

out vec3 outColor;

void main() {
    vec2 texturePos = vec2(gl_FragCoord.x / 1024, gl_FragCoord.y / 768);
    outColor = (texture(textureGR, texturePos) + texture(textureColor, texturePos)).xyz;
}
