#version 330 core

in vec4 lightPos;

uniform sampler2D textureColorGR;

layout(location = 0) out vec3 outColor;

float decay = 0.96815;
float exposure = 0.2;
float density = 0.926;
float weight = 0.58767;
int numSamples = 100;



void main() {
    vec2 texturePos = vec2(gl_FragCoord.x / 1024, gl_FragCoord.y / 768);
    vec2 lPos = (vec2(lightPos.xy) / lightPos.w) / 2 + 0.5;
    vec2 tc = texturePos;
    vec2 deltaTexCoord = (tc - lPos);
    deltaTexCoord *= 1.0 / float(numSamples) * density;
    float illuminationDecay = 1.0;
    vec4 color = texture(textureColorGR, tc.xy) * 0.4;
    for(int i = 0; i < numSamples; i++)
    {
        tc -= deltaTexCoord;
        vec4 sampl = texture(textureColorGR, tc) * 0.4;
        sampl *= illuminationDecay * weight;
        color += sampl;
        illuminationDecay *= decay;
    }

    outColor = color.xyz;
}
