#ifndef LIGHT_H
#define LIGHT_H
#include "common.hpp"
#include "Color.hpp"

class Light {
public:
  glm::vec3 coord;
  Color intensity;
  glm::vec3 attenu;
  Light(glm::vec3 c, Color rgb, glm::vec3 a): coord(c), intensity(rgb), attenu(a) {} 
};
#endif
