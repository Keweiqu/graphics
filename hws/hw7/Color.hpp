#ifndef COLOR_H
#define COLOR_H
#include "common.hpp"

class Color {
public:
  glm::vec3 rgb;
  Color(float x, float y, float z) {
    glm::vec3 color = glm::vec3(x, y, z);
    this->rgb = color;
  }
  Color(glm::vec3 color): rgb(color) {}
};
#endif
