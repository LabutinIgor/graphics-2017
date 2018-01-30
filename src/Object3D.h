

#ifndef SCENE3D_OBJECT3D_H
#define SCENE3D_OBJECT3D_H


#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object3D {
public:
    Object3D(const char* fileName, glm::vec3 diffuseColor, glm::vec3 specularColor,
             std::function<glm::vec3(double)> trajectory);
    glm::mat4 getModelMatrix(double time);
    glm::vec3 getPos();
    void init(float scale = 1);
    void draw(GLuint programID, bool toFullScreen = false);

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

private:
    std::vector<glm::vec3> pos;
    std::vector<glm::uvec3> ids;
    std::vector<glm::vec3> normals;
    std::function<glm::vec3(double)> trajectory;

    void calcNormals();
    GLuint verticesID;
    GLuint posID;
    GLuint idsID;
    GLuint normalsID;
    GLuint vertexArrayID;
    GLuint vertexBufferID;

    glm::mat4 modelMatrix;
    long long startTime;
    float scale;
};

#endif //SCENE3D_OBJECT3D_H
