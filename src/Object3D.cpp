
#include <sys/time.h>
#include <iostream>
#include "Object3D.h"

Object3D::Object3D(const char* fileName, glm::vec3 diffuseColor, glm::vec3 specularColor,
                   glm::vec3 (* trajectory)(double)) : diffuseColor(diffuseColor),
                                                       specularColor(specularColor),
                                                       trajectory(trajectory) {
    std::ifstream in(fileName);
    std::string l;
    while (getline(in, l)) {
        if (l[0] == '#') {
            continue;
        }
        std::istringstream line(l);
        char t;
        if (line >> t) {
            if (t == 'v') {
                float x, y, z;
                line >> x >> y >> z;
                pos.push_back(glm::vec3(x, y, z));
            } else if (t == 'f') {
                int id1, id2, id3;
                line >> id1 >> id2;
                while (line >> id3) {
                    ids.push_back(glm::uvec3(id1 - 1, id2 - 1, id3 - 1));
                    id2 = id3;
                }
            }
        }
    }
}

glm::mat4 Object3D::getModelMatrix(double time) {
    return glm::translate(glm::scale(glm::mat4(1), glm::vec3(scale, scale, scale)), trajectory(time));
}


glm::vec3 Object3D::getPos() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long curTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return trajectory((curTime - startTime) / 2000.0);
}

void Object3D::init(float scale) {
    this->scale = scale;
    glGenVertexArrays(1, &verticesID);
    glBindVertexArray(verticesID);

    glGenBuffers(1, &posID);
    glBindBuffer(GL_ARRAY_BUFFER, posID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pos.size(), &pos[0], GL_STATIC_DRAW);

    glGenBuffers(1, &idsID);
    glBindBuffer(GL_ARRAY_BUFFER, idsID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::uvec3) * ids.size(), &ids[0], GL_STATIC_DRAW);

    calcNormals();
    glGenBuffers(1, &normalsID);
    glBindBuffer(GL_ARRAY_BUFFER, normalsID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);

    struct timeval tp;
    gettimeofday(&tp, NULL);
    startTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void Object3D::draw(GLuint programID) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long curTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    glBindVertexArray(verticesID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, posID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalsID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    modelMatrix = getModelMatrix((curTime - startTime) / 2000.0);
    glUniformMatrix4fv(glGetUniformLocation(programID, "matrixM"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform3fv(glGetUniformLocation(programID, "objDC"), 1, glm::value_ptr(diffuseColor));
    glUniform3fv(glGetUniformLocation(programID, "objSC"), 1, glm::value_ptr(specularColor));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idsID);
    glDrawElements(GL_TRIANGLES, 3 * ids.size(), GL_UNSIGNED_INT, nullptr);
}

void Object3D::calcNormals() {
    normals.resize(pos.size());

    for (int i = 0; i < ids.size(); i++) {
        glm::vec3 p1(pos[ids[i].x]);
        glm::vec3 p2(pos[ids[i].y]);
        glm::vec3 p3(pos[ids[i].z]);
        glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
        normals[ids[i].x] += n;
        normals[ids[i].y] += n;
        normals[ids[i].z] += n;
    }

    for (int i = 0; i < pos.size(); i++) {
        normals[i] = glm::normalize(normals[i]);
    }
}
