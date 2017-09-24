
#include "MandelbrotGLCanvas.h"

MandelbrotGLCanvas::MandelbrotGLCanvas(nanogui::Widget* parent) : nanogui::GLCanvas(parent) {
    mShader.initFromFiles("a_simple_shader", "../shaders/vertex-shader.glsl", "../shaders/fragment-shader.glsl");

    nanogui::MatrixXu indices(3, 2);
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 0, 2, 3;

    nanogui::MatrixXf positions(3, 4);
    positions.col(0) << -1, -1, 1;
    positions.col(1) << -1, 1, 1;
    positions.col(2) << 1, 1, 1;
    positions.col(3) << 1, -1, 1;

    mShader.bind();
    mShader.uploadIndices(indices);
    mShader.uploadAttrib("position", positions);
}

void MandelbrotGLCanvas::drawGL() {
    mShader.bind();
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);
}

MandelbrotGLCanvas::~MandelbrotGLCanvas() {
    mShader.free();
}
