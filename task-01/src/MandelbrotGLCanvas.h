
#ifndef HW01_MANDELBROT_MANDELBROTGLCANVAS_H
#define HW01_MANDELBROT_MANDELBROTGLCANVAS_H

#include <nanogui/nanogui.h>

class MandelbrotGLCanvas : public nanogui::GLCanvas {
public:
    MandelbrotGLCanvas(nanogui::Widget* parent);

    ~MandelbrotGLCanvas();

    virtual void drawGL() override;
    virtual bool scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) override;
    virtual bool mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel,
                                int button, int modifiers) override;

private:
    nanogui::GLShader mShader;
    int mMaxIter = 100;
    nanogui::Vector2f mCenterPos = nanogui::Vector2f(0, 0);
    float mScale = 1.0;
};


#endif //HW01_MANDELBROT_MANDELBROTGLCANVAS_H
