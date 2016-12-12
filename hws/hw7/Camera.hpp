#ifndef CAMERA_H
#define CAMERA_H
#include "common.hpp"

using namespace std;

class Camera {
public:
  vec3 o;
  vec3 x;
  vec3 y;
  vec3 z;
  Camera();
  Camera(vec3 eye, vec3 look, vec3 up);
  
  
};
#endif
