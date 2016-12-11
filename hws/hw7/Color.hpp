#ifndef COLOR_H
#define COLOR_H
#include "common.hpp"

using namespace std;

class Color {
public:
  glm::vec3 rgb;
  Color(float x, float y, float z) {
    glm::vec3 color = glm::vec3(x, y, z);
    this->rgb = color;
  }
  Color(glm::vec3 color): rgb(color) {}
  Color(){
    rgb = glm::vec3(0.0);
  }
  string toString() {
    return to_string(rgb[0]) + " " + to_string(rgb[1]) + " " + to_string(rgb[2]);
  }
};
#endif
