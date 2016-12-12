#ifndef POINT_H
#define POINT_H

#include "common.hpp"

class Point {
 public:
  float x;
  float y;
  float z;
  Point();
  Point(float px, float py, float pz);
  Point(vec3 val);
  vec3 operator-(Point a) {
    return vec3(this->x - a.x, this->y - a.y, this->z - a.z);
  }

  Point operator+(vec3 v) {
    return Point(this->x + v[0], this->y + v[1], this->z + v[2]);
  }
};
#endif
