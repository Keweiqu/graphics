#ifndef SURFACE_FINISH_H
#define SURFACE_FINISH_H

#include "common.hpp"

using namespace std;

enum SurfaceFinishType {AMBIENT, DIFFUSE, SPECULAR, SHININESS, REFLECT, TRANSMIT, REFRACT};

class SurfaceFinish {
public:
  float cof[7];
  SurfaceFinish(float ka, float kd, float ks, float alpha, float kr, float kt, float et) {
    cof[AMBIENT] = ka;
    cof[DIFFUSE] = kd;
    cof[SPECULAR] = ks;
    cof[SHININESS] = alpha;
    cof[REFLECT] = kr;
    cof[TRANSMIT] = kt;
    cof[REFRACT] = et;
  }
  SurfaceFinish(){
    memset(cof, 0, sizeof(float) * 7);
  }
  string toString() {
    string s = "surface finish: ";
    for (int i = 0; i < 7; i++) {
      s = s + to_string(cof[i]);
      s = s + " ";
    }
    return s;
  }
};

#endif
