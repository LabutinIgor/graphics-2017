
#ifndef SCENE3D_POINTLIGHT_H
#define SCENE3D_POINTLIGHT_H

#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <sys/time.h>
#include <vector>
#include "Object3D.h"
#include <algorithm>

class PointLight {
public:
    PointLight(Object3D obj);
    void init();
    void draw(GLuint programID);

    Object3D obj;
};


#endif //SCENE3D_POINTLIGHT_H
