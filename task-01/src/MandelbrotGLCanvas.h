
#ifndef HW01_MANDELBROT_MANDELBROTGLCANVAS_H
#define HW01_MANDELBROT_MANDELBROTGLCANVAS_H


#include <nanogui/nanogui.h>

class MandelbrotGLCanvas : public nanogui::GLCanvas {
public:
    MandelbrotGLCanvas(nanogui::Widget* parent);

    ~MandelbrotGLCanvas();

    virtual void drawGL() override;

private:
    nanogui::GLShader mShader;

};


#endif //HW01_MANDELBROT_MANDELBROTGLCANVAS_H
