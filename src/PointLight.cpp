
#include "PointLight.h"

PointLight::PointLight(float power, glm::vec3 (* trajectory)(double)) :
        power(power), trajectory(trajectory) {}
