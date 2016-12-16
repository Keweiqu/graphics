#ifndef PIGMENT_H
#define PIGMENT_H
#include "common.hpp"
#include "Color.hpp"
#include "read_ppm.hpp"

using namespace std;

enum PigmentType {SOLID, CHECKER, IMAGE};

class Solid {
public:
  Color color;
  Solid(float r, float g, float b): color(r, g, b) {}
  Solid(){}
  string toString() {
    return "solid: " + color.toString();
  }
};

class Checker {
public:
  Color c0, c1;
  float size;
  Checker(float r0, float g0, float b0, float r1, float g1, float b1, float size): c0(r0, g0, b0), c1(r1, g1, b1) {
    this->size = size;
  }
  Checker(){
    this->size = 0;
  }
  string toString() {
    return "checker: " + c0.toString() + " " + c1.toString() + " " + to_string(size);
  }
};

class Pigment {
public:
  PigmentType type;
  union {
    Solid solid;
    Checker checker;
    Image image;
  };
  Pigment() {}
  string toString() {
    if (type == SOLID) {
      return solid.toString();
    } else {
      return checker.toString();
    }
  }
};

#endif
