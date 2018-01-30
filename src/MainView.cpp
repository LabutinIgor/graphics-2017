

#include <sys/time.h>
#include "MainView.h"

MainView::MainView() {}

void MainView::show() {
    initGlfwWindow();
    initGlew();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    programShadowMapID = loadShaders("../resources/shaders/shadow_map_vertex_shader.glsl",
                                     "../resources/shaders/shadow_map_fragment_shader.glsl");

    depthMatrixID = glGetUniformLocation(programShadowMapID, "matrixVP");


    unsigned posTexture = buffer.createColorTexture(GL_RGBA, GL_RGBA8, GL_CLAMP, FrameBuffer::filterLinear);

    buffer.create();
    buffer.bind();

    if (!buffer.attachColorTexture(GL_TEXTURE_2D, posTexture, 0))
        std::cerr << "buffer error with color attachment\n";

    if (!buffer.isOk())
        std::cerr << "Error with framebuffer\n";

    buffer.unbind();


    programID = loadShaders("../resources/shaders/vertex_shader.glsl", "../resources/shaders/fragment_shader.glsl");

    matrixVPID = glGetUniformLocation(programID, "matrixVP");
    matrixVPShadowMapID = glGetUniformLocation(programID, "dirLightVP");
    ShadowMapID = glGetUniformLocation(programID, "shadowMap");

    initMVPMatrices();
    setMouseCallbacks();

    staticObject.init(programID, programShadowMapID);
    dynamicObject.init(programID, programShadowMapID);
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

    window = glfwCreateWindow(1024, 768, "Scene3D", NULL, NULL);
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

void MainView::initMVPMatrices() {
    projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    cameraMatrix = glm::lookAt(
            glm::vec3(0, 0, 10),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );
    rotationMatrix = glm::mat4(1.f);
    scaleMatrix = glm::mat4(1.f);
}

void MainView::setMouseCallbacks() {
    glfwSetWindowUserPointer(window, this);
    auto buttonCallback = [](GLFWwindow* window, int button, int action, int mods) {
        static_cast<MainView*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(window, buttonCallback);

    auto scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
        static_cast<MainView*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
    };
    glfwSetScrollCallback(window, scrollCallback);

}

void MainView::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &previousMousePositionX, &previousMousePositionY);
            mousePressed = true;
        } else {
            if (action == GLFW_RELEASE) {
                mousePressed = false;
            }
        }
    }
}

void MainView::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    scaleCoefficient += yoffset * 0.001;
}

void MainView::updateRotation(double currentMousePositionX, double currentMousePositionY) {
    double dx = previousMousePositionX - currentMousePositionX;
    double dy = previousMousePositionY - currentMousePositionY;

    double r = sqrt(dx * dx + dy * dy);
    rotationMatrix = glm::eulerAngleXYZ((float) (dy * r * 0.001f), (float) (dx * r * 0.001f), 0.0f) * rotationMatrix;

    previousMousePositionX = currentMousePositionX;
    previousMousePositionY = currentMousePositionY;
}

void MainView::draw() {
    if (mousePressed) {
        double currentMousePositionX, currentMousePositionY;
        glfwGetCursorPos(window, &currentMousePositionX, &currentMousePositionY);
        updateRotation(currentMousePositionX, currentMousePositionY);
    }

    matrixVPShadowMap = glm::ortho<float>(-10, 10, -10, 10, -10, 20) *
                        glm::lookAt(directionalLight.pos,
                                    directionalLight.pos + directionalLight.direction,
                                    glm::vec3(0, 1, 0));
    drawToShadowMap();
    drawToScreen();


    glfwSwapBuffers(window);
    glfwPollEvents();
}

void MainView::drawToShadowMap() {
    glDisable(GL_BLEND);
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT};
    buffer.bind();
    glDrawBuffers(1, buffers);

    glViewport(0, 0, 1024, 768);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programShadowMapID);


    glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &matrixVPShadowMap[0][0]);

    staticObject.drawToShadowMap();
    dynamicObject.drawToShadowMap();
    planeObject.drawToShadowMap();
    buffer.unbind();
}

void MainView::drawToScreen() {
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 768);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    float c = exp(scaleCoefficient);
    scaleMatrix = glm::scale(glm::mat4(1.f), glm::tvec3<float>(c, c, c));
    glm::mat4 matrixVP = projectionMatrix * cameraMatrix * rotationMatrix * scaleMatrix;

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long curTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    glm::mat4 invViewMatr = glm::inverse(cameraMatrix * rotationMatrix * scaleMatrix);
    glm::vec3 cameraPos = glm::vec3(invViewMatr[3][0], invViewMatr[3][1], invViewMatr[3][2]);

    glUniformMatrix4fv(matrixVPID, 1, GL_FALSE, &matrixVP[0][0]);
    glUniformMatrix4fv(matrixVPShadowMapID, 1, GL_FALSE, &matrixVPShadowMap[0][0]);

    glUniform3fv(glGetUniformLocation(programID, "pointLightPos"), 1,
                 glm::value_ptr(pointLight.trajectory((curTime - startTime) / 2000.0)));
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, glm::value_ptr(cameraPos));
    glUniform1f(glGetUniformLocation(programID, "pointLightPow"), pointLight.power);
    glUniform3fv(glGetUniformLocation(programID, "dirLightDirection"), 1, glm::value_ptr(directionalLight.direction));
    glUniform1f(glGetUniformLocation(programID, "dirLightPow"), directionalLight.power);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(0));
    glUniform1i(ShadowMapID, 0);

    staticObject.draw();
    dynamicObject.draw();
    planeObject.draw();
}
