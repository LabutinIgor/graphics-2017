#include <iostream>
#include "MainView.h"
#include "ShadersLoader.h"

int main() {
    ShadersLoader shadersLoader;
    GLuint programID = shadersLoader.loadShaders("../shaders/vertex-shader.glsl", "../shaders/fragment-shader.glsl");

    MainView mainView(programID);
    mainView.show();

    return 0;
}
