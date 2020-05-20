#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 fronti, glm::vec3 ups) :
    pos(position), front(fronti), up(ups), right(glm::cross(front, up))
{}

void Camera::rotate(float angle, glm::vec3 axis)
{
    float angle2 = angle / 2;
    glm::quat q = glm::quat(glm::cos(angle2), glm::sin(angle2) * axis);
    glm::quat conjugate = glm::conjugate(q);
    front = q * front * conjugate;
    up = q * up * conjugate;
    right = q * right * conjugate;
}

void Camera::translate(glm::vec3 where)
{
    pos = where;
}
