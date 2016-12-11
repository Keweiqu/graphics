#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "common.hpp"
#include "SurfaceFinish.hpp"
#include "Pigment.hpp"
#include "Trans.hpp"

using namespace std;

enum ObjectType {SPHERE, PLANE, POLYHEDRON, MESH};

class Sphere {
public:
  float radius;
  glm::vec3 center_pos;
  Sphere(float cx, float cy, float cz, float r) {
    center_pos = glm::vec3(cx, cy, cz);
    radius = r;
  }
  Sphere(){
    this->radius = 0;
    this->center_pos = glm::vec3(0.0);
  }
  string toString() {
    return "sphere: " + to_string(center_pos[0]) + " " + to_string(center_pos[1]) + " " + to_string(center_pos[2]) + " " + to_string(radius);
  }
};

class Plane {
public:
  glm::vec4 cof;
  Plane(float a, float b, float c, float d) {
    cof = glm::vec4(a, b, c, d);
  }
  Plane(){
    this->cof = glm::vec4(0.0);
  }
  string toString() {
    return "plane: " + to_string(cof[0]) + " " + to_string(cof[1]) + " " + to_string(cof[2]) + " " + to_string(cof[3]);
  }
};

class Polyhedron {
public:
  vector<Plane> planes;
  Polyhedron() {}
  void addPlane(Plane p) {
    planes.push_back(p);
  }
  string toString() {
    string s = "polyhedron: \n";
    for (unsigned int i = 0; i < planes.size(); i++) {
      s = s + planes[0].toString() + "\n";
    }
    return s;
  }
};

class Object {
public:
  ObjectType ot;
  Pigment p;
  SurfaceFinish sf;
  vector<Trans> trans;
  union {
    Sphere sphere;
    Plane plane;
    Polyhedron polyhedron;
  };
  Object(){}
  ~Object(){}
  Object(Pigment pigment, SurfaceFinish surface): p(pigment), sf(surface) {
  }
  void addTrans(Trans t) {
    trans.push_back(t);
  }
  string toString() {
    string s = p.toString() + "\n" + sf.toString();
    for (unsigned int i = 0; i < trans.size(); i++) {
      s += trans[i].toString() + " ";
    }
    s += "\n";
    switch (ot) {
      case SPHERE:
        return s + sphere.toString();
        break;
      case PLANE:
        return s + plane.toString();
        break;
      case POLYHEDRON:
        return s + polyhedron.toString();
        break;
      default:
        return s;
        break;
    }
  }
};

#endif
