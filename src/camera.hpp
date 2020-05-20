#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>



class Camera
{
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    Camera(glm::vec3 position, glm::vec3 fronti, glm::vec3 ups);

    void rotate(float angle, glm::vec3 axis);
    void translate(glm::vec3 where);
};
#endif
