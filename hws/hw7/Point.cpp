#include "Point.hpp"

Point::Point(){}

Point::Point(float px, float py, float pz): x(px), y(py), z(pz) {}

Point::Point(vec3 val) {
  x = val[0];
  y = val[1];
  z = val[2];
}

