#include "Camera.hpp"

Camera::Camera(){}

Camera::Camera(vec3 eye, vec3 look, vec3 up) {
  o = eye;
  z = vec3::normalize(look - eye) * -1;
  x = vec3::normalize(vec3::cross(up, z));
  y = vec3::normalize(vec3::cross(z, x));
}
