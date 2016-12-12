#ifndef LIGHT_H
#define LIGHT_H
#include "common.hpp"
#include "Color.hpp"
#include "Point.hpp"

class Light {
public:
  Point coord;
  Color intensity;
  vec3 attenu;
  Light(Point c, Color rgb, vec3 a): coord(c), intensity(rgb), attenu(a) {} 
};
#endif
