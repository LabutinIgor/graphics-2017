
#ifndef SCENE3D_POINTLIGHT_H
#define SCENE3D_POINTLIGHT_H

#include <glm/vec3.hpp>

class PointLight {
public:
    PointLight(float power, glm::vec3(* trajectory)(double));
    float power;
    glm::vec3 (* trajectory)(double);
};


#endif //SCENE3D_POINTLIGHT_H
