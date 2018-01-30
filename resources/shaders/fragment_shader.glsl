#version 330 core

uniform vec3 objDC;
uniform vec3 objSC;
uniform vec3 cameraPos;
uniform vec3 pointLightPos;

uniform sampler2D textureDC;
uniform sampler2D textureSC;
uniform sampler2D textureNormals;
uniform sampler2D texturePositions;

layout(location = 0) out vec3 color;


void main() {
    vec2 texturePos = vec2(gl_FragCoord.x / 1024, gl_FragCoord.y / 768);
    vec3 n = 2 * texture(textureNormals, texturePos).xyz - 1;
    vec3 DC = texture(textureDC, texturePos).xyz;
    vec3 SC = texture(textureSC, texturePos).xyz;
    vec3 pos = 10 * texture(texturePositions, texturePos).xyz - 5;

    vec3 l = normalize(pointLightPos - pos);
    vec3 v = normalize(cameraPos - pos);
    vec3 h = normalize(l + v);
    vec3 colorDiffuse = vec3(0);
    vec3 colorSpecular = vec3(0);
    if (length(pointLightPos - pos) < 4 * sqrt(max(objDC.x, max(objDC.y, objDC.z))) - 0.1) {
        colorDiffuse = clamp(dot(n, l), 0, 1) * objDC * DC / pow(length(pointLightPos - pos), 2.0);
        colorSpecular = pow(clamp(dot(n, h), 0, 1), 30.0f) * objSC * SC;
    }
	color = colorDiffuse;// + colorSpecular;
}
