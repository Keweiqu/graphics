#ifndef CAMERA_H
#define CAMERA_H
#include "common.hpp"

using namespace std;

class Camera {
public:
  glm::vec3 o;
  glm::vec3 x;
  glm::vec3 y;
  glm::vec3 z;
  Camera();
  Camera(glm::vec3 eye, glm::vec3 look, glm::vec3 up);
  
  
};
#endif
