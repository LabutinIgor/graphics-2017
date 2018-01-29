

#ifndef SCENE3D_MAINVIEW_H
#define SCENE3D_MAINVIEW_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <FrameBuffer.h>

#include "load_shaders.h"
#include "MainView.h"
#include "Object3D.h"
#include "PointLight.h"
#include "controls.hpp"

class MainView {
public:
    MainView();
    void show();

private:
    void initGlfwWindow();
    void initGlew();
    void draw();
    void drawToGBuffer();
    void drawGBufferToScreen();
    void drawToScreen();
    void initGBuffer();

    GLFWwindow* window;
    GLuint programID;
    GLuint programGBufferID;
    GLuint resProgramID;

    FrameBuffer buffer = FrameBuffer(1024, 768, FrameBuffer::depth32);

    Object3D staticObject = Object3D("../resources/stanford_bunny.obj", glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),
                                     glm::vec3(0, 0, 0),
                                     [](double time) -> glm::vec3 {
                                         return glm::vec3(0, 1, 0);
                                     });
    Object3D dynamicObject = Object3D("../resources/stanford_bunny.obj", glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
                                      glm::vec3(0, 0, 0),
                                      [](double time) -> glm::vec3 {
                                          return glm::vec3(0.3 * sin(time), 1 + 0.3 * cos(time), 0);
                                      });
    Object3D dynamicObject2 = Object3D("../resources/stanford_bunny.obj", glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
                                      glm::vec3(0, 0, 0),
                                      [](double time) -> glm::vec3 {
                                          return glm::vec3(10 + 0.5 * sin(time), 15 + 0.5 * cos(time), 1);
                                      });
    Object3D sun = Object3D("../resources/sphere.obj", glm::vec3(1, 1, 1), glm::vec3(0.3, 0.3, 0.3),
                            glm::vec3(1, 1, 0),
                            [](double time) -> glm::vec3 {
                                return glm::vec3(17, 23, 1);
                            });
    Object3D sceneObject = Object3D("../resources/sponza.obj", glm::vec3(0, 1, 0), glm::vec3(0, 0.5, 0),
                                    glm::vec3(0, 0, 0),
                                    [](double time) -> glm::vec3 {
                                        return glm::vec3(0, -1, 0);
                                    });
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint drawGBufferProgramID;
};


#endif //SCENE3D_MAINVIEW_H
