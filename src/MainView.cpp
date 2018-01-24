

#include <sys/time.h>

#include "MainView.h"
#include "controls.hpp"

MainView::MainView() {}

void MainView::show() {
    initGlfwWindow();
    initGlew();

    initWindow(window);

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    programShadowMapID = loadShaders("../resources/shaders/g_buffer_vertex_shader.glsl",
                                     "../resources/shaders/g_buffer_fragment_shader.glsl");

//    depthMatrixID = glGetUniformLocation(programShadowMapID, "matrixVP");
//
//
//    glGenFramebuffers(1, &framebufferID);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
//
//    glGenTextures(1, &depthTexture);
//    glBindTexture(GL_TEXTURE_2D, depthTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//
//    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
//
//    glDrawBuffer(GL_NONE);
//
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//        std::cerr << "Error in Framebuffer\n";
//        exit(1);
//    }


    programID = loadShaders("../resources/shaders/vertex_shader.glsl", "../resources/shaders/fragment_shader.glsl");

    matrixVPID = glGetUniformLocation(programID, "matrixVP");

//    matrixVPShadowMapID = glGetUniformLocation(programID, "dirLightVP");
//    ShadowMapID = glGetUniformLocation(programID, "shadowMap");

//    staticObject.init(programID, programShadowMapID);
//    dynamicObject.init(programID, programShadowMapID);
    planeObject.init(programID, programShadowMapID);

    struct timeval tp;
    gettimeofday(&tp, NULL);
    startTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;


    while (glfwWindowShouldClose(window) == 0) {
        draw();
    }

    glDeleteProgram(programID);
    glfwTerminate();
}

void MainView::initGlfwWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Deferred", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
}

void MainView::initGlew() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        exit(1);
    }
}

void MainView::draw() {
    drawToGBuffer();
    drawToScreen();


    glfwSwapBuffers(window);
    glfwPollEvents();
}

void MainView::drawToGBuffer() {
//    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
//    glViewport(0, 0, 1024, 1024);
//
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glUseProgram(programShadowMapID);
//
//
//    staticObject.drawToGBuffer();
//    dynamicObject.drawToGBuffer();
//    planeObject.drawToGBuffer();
}

void MainView::drawGBufferToScreen() {

}

void MainView::drawToScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 1024);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    computeMatricesFromInputs();
    glm::mat4 projectionMatrix = getProjectionMatrix();
    glm::mat4 viewMatrix = getViewMatrix();
    glm::mat4 matrixVP = projectionMatrix * viewMatrix;

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long curTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    glm::mat4 invViewMatr = glm::inverse(viewMatrix);
    glm::vec3 cameraPos = glm::vec3(invViewMatr[3][0], invViewMatr[3][1], invViewMatr[3][2]);

    glUniformMatrix4fv(matrixVPID, 1, GL_FALSE, &matrixVP[0][0]);

    glUniform3fv(glGetUniformLocation(programID, "pointLightPos"), 1,
                 glm::value_ptr(pointLight.trajectory((curTime - startTime) / 2000.0)));
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, glm::value_ptr(cameraPos));
    glUniform1f(glGetUniformLocation(programID, "pointLightPow"), pointLight.power);

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, depthTexture);
//    glUniform1i(ShadowMapID, 0);

//    staticObject.draw();
//    dynamicObject.draw();
    planeObject.draw();
}

