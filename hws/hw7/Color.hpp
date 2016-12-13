#ifndef COLOR_H
#define COLOR_H
#include "common.hpp"

using namespace std;

class Color {
public:
  vec3 rgb;
  Color(float x, float y, float z) {
    vec3 color = vec3(x, y, z);
    this->rgb = color;
  }
  Color(vec3 color): rgb(color) {}
  Color(){
    rgb = vec3(0.0);
  }
  string toString() {
    return to_string(rgb[0]) + " " + to_string(rgb[1]) + " " + to_string(rgb[2]);
  }
  Color operator+(Color c) {
    return Color(this->rgb + c.rgb);
  }

  Color operator*(Color c) {
    return Color(this->rgb[0] * c.rgb[0], this->rgb[1] * c.rgb[1], this->rgb[2] * c.rgb[2]);
  }
};
#endif
