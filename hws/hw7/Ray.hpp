#ifndef RAY_H
#define RAY_H

#include <cfloat>
#include "common.hpp"

class Ray {
public:
  vec3 o;
  vec3 d;
  float t;
  Ray();
  Ray(vec3 origin, vec3 direction);
  Ray(vec3 origin, vec3 direction, float scalar);
};
#endif
