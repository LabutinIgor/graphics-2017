

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

//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);

    programShadowMapID = loadShaders("../resources/shaders/shadow_map_vertex_shader.glsl",
                                     "../resources/shaders/shadow_map_fragment_shader.glsl");

    depthMatrixID = glGetUniformLocation(programShadowMapID, "matrixVP");



//    // Load it into a VBO
//
//
//    glGenBuffers(1, &vertexbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBufferData(GL_ARRAY_BUFFER, staticObject.pos.size() * sizeof(glm::vec3), &staticObject.pos[0], GL_STATIC_DRAW);
//
//
//    // Generate a buffer for the indices as well
//
//    glGenBuffers(1, &elementbuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, staticObject.ids.size() * sizeof(glm::uvec3), &staticObject.ids[0],
//                 GL_STATIC_DRAW);


    // ---------------------------------------------
    // Render to Texture - specific code begins here
    // ---------------------------------------------

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.

    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    // No color output in the bound framebuffer, only depth.
    glDrawBuffer(GL_NONE);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error in Framebuffer\n";
        exit(1);
    }


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
                        glm::lookAt(glm::vec3(2, 2, 0),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(0, 1, 0));
    drawToShadowMap();
    drawToScreen();


    glfwSwapBuffers(window);
    glfwPollEvents();
}

void MainView::drawToShadowMap() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, 1024, 1024);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programShadowMapID);


    glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &matrixVPShadowMap[0][0]);

    staticObject.drawToShadowMap();
    dynamicObject.drawToShadowMap();
    planeObject.drawToShadowMap();
}

void MainView::drawToScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 1024);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    float c = exp(scaleCoefficient);
    scaleMatrix = glm::scale(glm::mat4(1.f), glm::tvec3<float>(c, c, c));
    glm::mat4 matrixVP = projectionMatrix * cameraMatrix * rotationMatrix * scaleMatrix;

    glUniformMatrix4fv(matrixVPID, 1, GL_FALSE, &matrixVP[0][0]);
    glUniformMatrix4fv(matrixVPShadowMapID, 1, GL_FALSE, &matrixVPShadowMap[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(ShadowMapID, 0);

    staticObject.draw();
    dynamicObject.draw();
    planeObject.draw();
}




