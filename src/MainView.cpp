

#include <sys/time.h>

#include "MainView.h"

MainView::MainView() {}

int currentTexture = 0;

void MainView::show() {
    initGlfwWindow();
    initGlew();
    initWindow(window);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_1:
                currentTexture = -1;
                break;
            case GLFW_KEY_2:
                currentTexture = 0;
                break;
            case GLFW_KEY_3:
                currentTexture = 1;
                break;
            case GLFW_KEY_4:
                currentTexture = 2;
                break;
            case GLFW_KEY_5:
                currentTexture = 3;
                break;
            default:
                break;
        }
    });

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    initGBuffer();

    programGBufferID = loadShaders("../resources/shaders/g_buffer_vertex_shader.glsl",
                                   "../resources/shaders/g_buffer_fragment_shader.glsl");

    programID = loadShaders("../resources/shaders/vertex_shader.glsl", "../resources/shaders/fragment_shader.glsl");

    staticObject.init(programGBufferID);
    dynamicObject.init(programGBufferID);
    sceneObject.init(programGBufferID);


    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);
    static const GLfloat vertexBuffer[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
    drawGBufferProgramID = loadShaders("../resources/shaders/output_g_buffer_vertex_shader.glsl",
                                       "../resources/shaders/output_g_buffer_fragment_shader.glsl");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1024, 768);

    struct timeval tp;
    gettimeofday(&tp, NULL);
    startTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    while (glfwWindowShouldClose(window) == 0) {
        draw();
    }

    glDeleteProgram(programID);
    glfwTerminate();
}

void MainView::initGBuffer() {
    unsigned diffuseColorTexture = buffer.createColorTexture(GL_RGBA, GL_RGBA8, GL_CLAMP, FrameBuffer::filterLinear);
    unsigned specularColorTexture = buffer.createColorTexture(GL_RGBA, GL_RGBA8, GL_CLAMP, FrameBuffer::filterLinear);
    unsigned normalTexture = buffer.createColorTexture(GL_RGBA, GL_RGBA8, GL_CLAMP, FrameBuffer::filterLinear);
    unsigned posTexture = buffer.createColorTexture(GL_RGBA, GL_RGBA8, GL_CLAMP, FrameBuffer::filterLinear);

    buffer.create();
    buffer.bind();

    if (!buffer.attachColorTexture(GL_TEXTURE_2D, diffuseColorTexture, 0))
        std::cerr << "buffer error with color attachment\n";

    if (!buffer.attachColorTexture(GL_TEXTURE_2D, specularColorTexture, 1))
        std::cerr << "buffer error with color attachment\n";

    if (!buffer.attachColorTexture(GL_TEXTURE_2D, normalTexture, 2))
        std::cerr << "buffer error with color attachment\n";

    if (!buffer.attachColorTexture(GL_TEXTURE_2D, posTexture, 3))
        std::cerr << "buffer error with color attachment\n";

    if (!buffer.isOk())
        std::cerr << "Error with framebuffer\n";

    buffer.unbind();
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
    if (currentTexture == -1) {
        drawToScreen();
    } else {
        drawGBufferToScreen();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void MainView::drawToGBuffer() {
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT,
                        GL_COLOR_ATTACHMENT3_EXT};
    buffer.bind();
    glDrawBuffers(4, buffers);

    glViewport(0, 0, 1024, 768);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programGBufferID);

    computeMatricesFromInputs();
    glm::mat4 projectionMatrix = getProjectionMatrix();
    glm::mat4 viewMatrix = getViewMatrix();
    glm::mat4 matrixVP = projectionMatrix * viewMatrix;

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long curTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    glm::mat4 invViewMatr = glm::inverse(viewMatrix);
    glm::vec3 cameraPos = glm::vec3(invViewMatr[3][0], invViewMatr[3][1], invViewMatr[3][2]);

    glUniformMatrix4fv(glGetUniformLocation(programGBufferID, "matrixVP"), 1, GL_FALSE, &matrixVP[0][0]);

    glUniform3fv(glGetUniformLocation(programGBufferID, "pointLightPos"), 1,
                 glm::value_ptr(pointLight.trajectory((curTime - startTime) / 2000.0)));
    glUniform3fv(glGetUniformLocation(programGBufferID, "cameraPos"), 1, glm::value_ptr(cameraPos));
    glUniform1f(glGetUniformLocation(programGBufferID, "pointLightPow"), pointLight.power);

    staticObject.drawToGBuffer();
    dynamicObject.drawToGBuffer();
    sceneObject.drawToGBuffer();
    buffer.unbind();
}

void MainView::drawGBufferToScreen() {
    glViewport(0, 0, 1024, 768);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(drawGBufferProgramID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(currentTexture));
    glUniform1i(glGetUniformLocation(drawGBufferProgramID, "renderedTexture"), 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}

void MainView::drawToScreen() {
    glViewport(0, 0, 1024, 768);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(3));
    glUniform1i(glGetUniformLocation(programID, "textureDC"), 0);
    glUniform1i(glGetUniformLocation(programID, "textureSC"), 1);
    glUniform1i(glGetUniformLocation(programID, "textureNormals"), 2);
    glUniform1i(glGetUniformLocation(programID, "texturePositions"), 3);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}
