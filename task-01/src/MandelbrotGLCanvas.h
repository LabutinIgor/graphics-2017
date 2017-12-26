
#ifndef HW01_MANDELBROT_MANDELBROTGLCANVAS_H
#define HW01_MANDELBROT_MANDELBROTGLCANVAS_H

#include <nanogui/nanogui.h>

using handleType = std::unique_ptr<uint8_t[], void(*)(void*)>;

class MandelbrotGLCanvas : public nanogui::GLCanvas {
public:
    MandelbrotGLCanvas(nanogui::Widget* parent);

    ~MandelbrotGLCanvas();

    virtual void drawGL() override;
    virtual bool scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) override;
    virtual bool mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel,
                                int button, int modifiers) override;
    void setIterationsCnt(float x);
    void setThreshold(float x);

private:
    nanogui::GLShader mShader;
    int mMaxIter = 200;
    int mThreshold = 10;
    nanogui::Vector2f mCenterPos = nanogui::Vector2f(0, 0);
    float mScale = 1.0;
    GLuint mTextureId = 0;

    void loadTexture(const std::string& fileName);
};


#endif //HW01_MANDELBROT_MANDELBROTGLCANVAS_H
