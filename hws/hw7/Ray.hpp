#ifndef RAY_H
#define RAY_H

#include <cfloat>
#include "common.hpp"
#include "Point.hpp"

enum Status {INTERSECT, NO_INTERSECT};

class Ray {
public:
  Point o;
  vec3 d;
  float t;
  vec3 intersect_normal;
  int intersect_obj_index;
  Point intersect_point;
  Ray();
  Ray(vec3 origin, vec3 direction);
  Ray(vec3 origin, vec3 direction, float scalar);
  Ray(Point source, Point dest);
};
#endif
