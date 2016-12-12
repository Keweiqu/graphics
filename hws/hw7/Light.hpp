#ifndef LIGHT_H
#define LIGHT_H
#include "common.hpp"
#include "Color.hpp"

class Light {
public:
  vec3 coord;
  Color intensity;
  vec3 attenu;
  Light(vec3 c, Color rgb, vec3 a): coord(c), intensity(rgb), attenu(a) {} 
};
#endif
