

#include <sys/time.h>

#include "MainView.h"

MainView::MainView() {}

void MainView::show() {
    initGlfwWindow();
    initGlew();
    initWindow(window);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    initGBuffer();

    programGBufferID = loadShaders("../resources/shaders/g_buffer_vertex_shader.glsl",
                                   "../resources/shaders/g_buffer_fragment_shader.glsl");

    programID = loadShaders("../resources/shaders/vertex_shader.glsl", "../resources/shaders/fragment_shader.glsl");

    staticObject.init();
    dynamicObject.init();
    dynamicObject2.init(20);
    sun.init(3);
    sceneObject.init();

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
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

    resProgramID = loadShaders("../resources/shaders/res_vertex_shader.glsl",
                               "../resources/shaders/res_fragment_shader.glsl");

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

    window = glfwCreateWindow(1024, 768, "GodRays", NULL, NULL);
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
    drawGBufferToScreen();
    drawToScreen();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void MainView::drawToGBuffer() {
    glDisable(GL_BLEND);
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT};
    buffer.bind();
    glDrawBuffers(1, buffers);

    glViewport(0, 0, 1024, 768);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programGBufferID);

    computeMatricesFromInputs();
    glm::mat4 projectionMatrix = getProjectionMatrix();
    glm::mat4 viewMatrix = getViewMatrix();
    glm::mat4 matrixVP = projectionMatrix * viewMatrix;

    glm::mat4 invViewMatr = glm::inverse(viewMatrix);
    glm::vec3 cameraPos = glm::vec3(invViewMatr[3][0], invViewMatr[3][1], invViewMatr[3][2]);

    glUniformMatrix4fv(glGetUniformLocation(programGBufferID, "matrixVP"), 1, GL_FALSE, glm::value_ptr(matrixVP));
    glUniform3fv(glGetUniformLocation(programGBufferID, "cameraPos"), 1, glm::value_ptr(cameraPos));

    staticObject.draw(programGBufferID);
    dynamicObject.draw(programGBufferID);
    sun.draw(programGBufferID);
    dynamicObject2.draw(programGBufferID);
    sceneObject.draw(programGBufferID);
    buffer.unbind();
}

void MainView::drawGBufferToScreen() {
    glDisable(GL_BLEND);
    GLenum buffers[] = {GL_COLOR_ATTACHMENT1_EXT};
    buffer.bind();
    glDrawBuffers(1, buffers);
    glViewport(0, 0, 1024, 768);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(drawGBufferProgramID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(0));
    glUniform1i(glGetUniformLocation(drawGBufferProgramID, "renderedTexture"), 0);

    computeMatricesFromInputs();
    glm::mat4 projectionMatrix = getProjectionMatrix();
    glm::mat4 viewMatrix = getViewMatrix();
    glm::mat4 matrixVP = projectionMatrix * viewMatrix;

    glm::mat4 invViewMatr = glm::inverse(viewMatrix);
    glm::vec3 cameraPos = glm::vec3(invViewMatr[3][0], invViewMatr[3][1], invViewMatr[3][2]);

    glUniform3fv(glGetUniformLocation(drawGBufferProgramID, "lPos"), 1, glm::value_ptr(sun.getPos()));
    glUniformMatrix4fv(glGetUniformLocation(drawGBufferProgramID, "matrixVP"), 1, GL_FALSE, glm::value_ptr(matrixVP));


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    buffer.unbind();


    GLenum buffers2[] = {GL_COLOR_ATTACHMENT2_EXT};
    buffer.bind();
    glDrawBuffers(1, buffers2);
    glViewport(0, 0, 1024, 768);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glUniform3fv(glGetUniformLocation(programID, "lPos"), 1, glm::value_ptr(sun.getPos()));
    glUniformMatrix4fv(glGetUniformLocation(programID, "matrixVP"), 1, GL_FALSE, glm::value_ptr(matrixVP));
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, glm::value_ptr(cameraPos));

    staticObject.draw(programID);
    dynamicObject.draw(programID);
    sun.draw(programID);
    dynamicObject2.draw(programID);
    sceneObject.draw(programID);
    buffer.unbind();
}

void MainView::drawToScreen() {
    glDisable(GL_BLEND);
    glViewport(0, 0, 1024, 768);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(resProgramID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(1));
    glUniform1i(glGetUniformLocation(resProgramID, "textureGR"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, buffer.getColorBuffer(2));
    glUniform1i(glGetUniformLocation(resProgramID, "textureColor"), 1);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}
