
#include <stb_image.h>
#include <cstdint>
#include <memory>

#include "MandelbrotGLCanvas.h"


MandelbrotGLCanvas::MandelbrotGLCanvas(nanogui::Widget* parent) : nanogui::GLCanvas(parent) {
    mShader.initFromFiles("a_simple_shader", "../resources/shaders/vertex-shader.glsl",
                          "../resources/shaders/fragment-shader.glsl");

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

    loadTexture("../resources/palette.png");
}

void MandelbrotGLCanvas::loadTexture(const std::string& fileName) {
    if (mTextureId) {
        glDeleteTextures(1, &mTextureId);
        mTextureId = 0;
    }
    int force_channels = 0;
    int w, h, n;
    handleType textureData(stbi_load(fileName.c_str(), &w, &h, &n, force_channels), stbi_image_free);
    if (!textureData)
        throw std::invalid_argument("Could not load texture data from file " + fileName);
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_1D, mTextureId);
    GLint internalFormat;
    GLint format;
    switch (n) {
        case 1:
            internalFormat = GL_R8;
            format = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG8;
            format = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            break;
        default:
            internalFormat = 0;
            format = 0;
            break;
    }
    glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, w, 0, format, GL_UNSIGNED_BYTE, textureData.get());
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void MandelbrotGLCanvas::drawGL() {
    mShader.bind();
    glBindTexture(GL_TEXTURE_1D, mTextureId);
    mShader.setUniform("max_iter", mMaxIter);
    mShader.setUniform("threshold", mThreshold);
    mShader.setUniform("center_pos", mCenterPos);
    mShader.setUniform("scale", mScale);
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);
}

MandelbrotGLCanvas::~MandelbrotGLCanvas() {
    mShader.free();
}

bool MandelbrotGLCanvas::scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) {
    float newScale = mScale + rel.y() / 100;
    nanogui::Vector2f p0(((float) p.x() - this->mPos.x()) / width(), ((float) p.y() - this->mPos.y()) / height());

    mCenterPos = nanogui::Vector2f(mCenterPos.x() + (p0.x() - 0.5) * (exp(mScale) - exp(newScale)) * 2,
                                   mCenterPos.y() - (p0.y() - 0.5) * (exp(mScale) - exp(newScale)) * 2);
    mScale = newScale;
    return nanogui::Widget::scrollEvent(p, rel);
}

bool MandelbrotGLCanvas::mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel,
                                        int button, int modifiers) {
    mCenterPos = nanogui::Vector2f(mCenterPos.x() - ((float) rel.x()) / width() * exp(mScale) * 2,
                                   mCenterPos.y() + ((float) rel.y()) / height() * exp(mScale) * 2);
    return nanogui::Widget::mouseDragEvent(p, rel, button, modifiers);
}

void MandelbrotGLCanvas::setIterationsCnt(float x) {
    mMaxIter = (int) (400 * x);
}

void MandelbrotGLCanvas::setThreshold(float x) {
    mThreshold = (int) (20 * x);
}
