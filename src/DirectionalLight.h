

#ifndef SCENE3D_DIRECTIONALLIGHT_H
#define SCENE3D_DIRECTIONALLIGHT_H

#include <glm/vec3.hpp>

class DirectionalLight {
public:
    DirectionalLight(float power, glm::vec3 pos, glm::vec3 direction);
    float power;
    glm::vec3 pos;
    glm::vec3 direction;
};


#endif //SCENE3D_DIRECTIONALLIGHT_H
