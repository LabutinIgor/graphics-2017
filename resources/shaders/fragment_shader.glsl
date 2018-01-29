#version 330 core

in vec3 pos;
in vec3 n;
in vec4 lightPos;

uniform vec3 cameraPos;
uniform vec3 objDC;
uniform vec3 objSC;
uniform vec3 pointLightPos;
uniform sampler2D textureColorGR;

float decay=0.96815;
float exposure=0.2;
float density=0.926;
float weight=0.58767;
int numSamples = 100;

out vec4 outColor;


void main() {
    vec3 l = normalize(pointLightPos - pos);
    vec3 v = normalize(cameraPos - pos);
    vec3 h = normalize(l + v);
    vec3 colorDiffuse = clamp(dot(n, l), 0, 1) * objDC;
    vec3 colorSpecular = pow(clamp(dot(n, h), 0, 1), 30.0f) * objSC;
	vec4 sceneColor = vec4((colorDiffuse + colorSpecular) / 2, 1);

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

    outColor = sceneColor + color;
}
