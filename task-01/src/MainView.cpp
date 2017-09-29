
#include <string>

#include "MainView.h"
#include "MandelbrotGLCanvas.h"


MainView::MainView() {
}

void MainView::show() {
    nanogui::init();

    nanogui::Screen screen(Eigen::Vector2i(1024, 768), "Mandelbrot");

    screen.setLayout(new nanogui::GroupLayout());
    MandelbrotGLCanvas* canvas = new MandelbrotGLCanvas(&screen);
    canvas->setBackgroundColor({100, 100, 100, 255});
    canvas->setSize({1024, 600});

    nanogui::Slider* iterationsSlider = new nanogui::Slider(&screen);
    nanogui::Slider* thresholdSlider = new nanogui::Slider(&screen);
    iterationsSlider->setCallback([canvas](float x) { canvas->setIterationsCnt(x); });
    thresholdSlider->setCallback([canvas](float x) { canvas->setThreshold(x); });
    iterationsSlider->setValue(0.5);
    thresholdSlider->setValue(0.5);

    screen.setVisible(true);
    screen.performLayout();

    nanogui::mainloop();

    nanogui::shutdown();
}
