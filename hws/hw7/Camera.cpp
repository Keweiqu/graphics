#include "Camera.hpp"

Camera::Camera(){}

Camera::Camera(glm::vec3 eye, glm::vec3 look, glm::vec3 up) {
  o = eye;
  z = -glm::normalize(look - eye);
  x = glm::normalize(glm::cross(up, z));
  y = glm::normalize(glm::cross(z, x));
}
