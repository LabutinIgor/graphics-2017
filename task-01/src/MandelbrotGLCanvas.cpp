
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
    mShader.setUniform("max_iter", mMaxIter);
    mShader.setUniform("center_pos", mCenterPos);
    mShader.setUniform("scale", mScale);
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);
}

MandelbrotGLCanvas::~MandelbrotGLCanvas() {
    mShader.free();
}

bool MandelbrotGLCanvas::scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) {
    float newScale = mScale + rel.y() / 100;

    mCenterPos = nanogui::Vector2f(mCenterPos.x() - (((float) p.x()) / width() - 0.5) *
                                                            (exp(newScale) - exp(mScale)) * 2,
                                   mCenterPos.y() + (((float) p.y()) / height() - 0.5) *
                                                            (exp(newScale) - exp(mScale)) * 2);
    mScale = newScale;
    return nanogui::Widget::scrollEvent(p, rel);
}

bool MandelbrotGLCanvas::mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel,
                                        int button, int modifiers) {
    mCenterPos = nanogui::Vector2f(mCenterPos.x() - ((float) rel.x()) / width() * exp(mScale) * 2,
                                   mCenterPos.y() + ((float) rel.y()) / height() * exp(mScale) * 2);
    return nanogui::Widget::mouseDragEvent(p, rel, button, modifiers);
}
