
#include <iostream>
#include "PointLight.h"

PointLight::PointLight(Object3D obj) : obj(obj) {}

void PointLight::init() {
    obj.init(4 * sqrt(fmax(obj.diffuseColor.r, fmax(obj.diffuseColor.g, obj.diffuseColor.b))));
}

void PointLight::draw(GLuint programID, bool toFullScreen) {
    glUniform3fv(glGetUniformLocation(programID, "pointLightPos"), 1, glm::value_ptr(obj.getPos()));
    obj.draw(programID, toFullScreen);
}
