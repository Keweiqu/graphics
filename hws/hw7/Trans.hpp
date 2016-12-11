#ifndef TRANS_H
#define TRANS_H

#include "common.hpp"

using namespace std;

enum TransType {SCALE, TRANSLATE};

class Trans {
public:
  TransType type;
  glm::vec3 vec;
  Trans(TransType t, float x, float y, float z) {
    this->type = t;
    vec = glm::vec3(x, y, z);
  }
  Trans(){}
  string toString() {
    if (type == SCALE) {
      return "scale: " + to_string(vec[0]) + " " + to_string(vec[1]) + " " + to_string(vec[2]);
    } else {
      return "translate: " + to_string(vec[0]) + " " + to_string(vec[1]) + " " + to_string(vec[2]);
    }
  }
};

#endif
