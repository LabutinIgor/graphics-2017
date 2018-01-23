

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

#include "load_shaders.h"
#include "MainView.h"
#include "Object3D.h"
//#include "PointLight.h"
//#include "DirectionalLight.h"

class MainView {
public:
    MainView();
    void show();

private:
    void initGlfwWindow();
    void initGlew();
    void initMVPMatrices();
    void setMouseCallbacks();
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void draw();
    void drawToShadowMap();
    void drawToScreen();
    void updateRotation(double currentMousePositionX, double currentMousePositionY);

    GLFWwindow* window;
    GLuint programID;
    GLuint programShadowMapID;

    glm::mat4 projectionMatrix;
    glm::mat4 cameraMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 matrixVPShadowMap;
    double scaleCoefficient = 1;
    bool mousePressed = false;
    double previousMousePositionX;
    double previousMousePositionY;

    long long startTime;

    Object3D staticObject = Object3D("../resources/stanford_bunny.obj", glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),
                                     [](double time) -> glm::vec3 {
                                         return glm::vec3(0, 0, 0);
                                     });
    Object3D dynamicObject = Object3D("../resources/stanford_bunny.obj", glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
                                      [](double time) -> glm::vec3 {
                                          return glm::vec3(0.3 * sin(time), 0.3 * cos(time), 0);
                                      });
    Object3D planeObject = Object3D("../resources/plane.obj", glm::vec3(0, 1, 0), glm::vec3(0, 0.5, 0),
                                    [](double time) -> glm::vec3 {
                                        return glm::vec3(0, 0, 0);
                                    });
//    PointLight pointLight = PointLight(1.0,
//                                       [](double time) -> glm::vec3 {
//                                           return glm::vec3(sin(time), 1, cos(time));
//                                       });
    GLint depthMatrixID;
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint framebufferID = 0;
    GLuint depthTexture;
    GLint matrixVPID;
    GLint matrixVPShadowMapID;
    GLint ShadowMapID;
};


#endif //SCENE3D_MAINVIEW_H
