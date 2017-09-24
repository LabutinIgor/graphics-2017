
#ifndef HW01_MANDELBROT_SHADERSLOADER_H
#define HW01_MANDELBROT_SHADERSLOADER_H

#include <OpenGL/gl3.h>

class ShadersLoader {
public:
    ShadersLoader();

    GLuint loadShaders(const char * vertexShaderPath,const char * fragmentShaderPath);

};


#endif //HW01_MANDELBROT_SHADERSLOADER_H
