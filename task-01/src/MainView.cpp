
#include <string>
#include "MainView.h"
#include "MandelbrotGLCanvas.h"


MainView::MainView() {
}

void MainView::show() {
    nanogui::init();

    nanogui::Screen screen(Eigen::Vector2i(1024, 768), "Mandelbrot");
    MandelbrotGLCanvas* canvas = new MandelbrotGLCanvas(&screen);
    canvas->setBackgroundColor({100, 100, 100, 255});
    canvas->setSize({1024, 600});

    screen.setVisible(true);
    screen.performLayout();

    nanogui::mainloop();

    nanogui::shutdown();
}
