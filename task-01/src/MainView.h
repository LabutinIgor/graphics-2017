
#ifndef HW01_MANDELBROT_MAINVIEW_H
#define HW01_MANDELBROT_MAINVIEW_H


#include <OpenGL/OpenGL.h>

class MainView {
public:
    MainView(GLuint programID);
    void show();

    GLuint programID;
};


#endif //HW01_MANDELBROT_MAINVIEW_H
