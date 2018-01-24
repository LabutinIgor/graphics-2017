

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
    Object3D(const char* fileName, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3(* trajectory)(double));
    glm::mat4 getModelMatrix(double time);
    void init(GLuint programGBufferID);
    void drawToGBuffer();

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    std::vector<glm::vec3> pos;
    std::vector<glm::uvec3> ids;
    std::vector<glm::vec3> normals;
    glm::vec3 (* trajectory)(double);


    void calcNormals();
    GLuint verticesID;
    GLuint posID;
    GLuint idsID;
    GLuint normalsID;
    GLuint programGBufferID;

    glm::mat4 modelMatrix;

    long long startTime;
};

#endif //SCENE3D_OBJECT3D_H
