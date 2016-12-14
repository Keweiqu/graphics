#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "common.hpp"
#include "SurfaceFinish.hpp"
#include "Pigment.hpp"
#include "Trans.hpp"
#include "Point.hpp"

using namespace std;

enum ObjectType {SPHERE, PLANE, POLYHEDRON, MESH};

class Sphere {
public:
  float radius;
  Point center;
  Sphere(float cx, float cy, float cz, float r) {
    center = Point(cx, cy, cz);
    radius = r;
  }
  Sphere(){
    this->radius = 0;
    this->center = Point();
  }
  string toString() {
    return "sphere: " + to_string(center.x) + " " + to_string(center.y) + " " + to_string(center.z) + " " + to_string(radius);
  }
};

class Plane {
public:
  float a;
  float b;
  float c;
  float d;
  Plane(float aa, float bb, float cc, float dd):a(aa), b(bb), c(cc), d(dd) {}
  Plane(){}
  string toString() {
    return "plane: " + to_string(a) + " " + to_string(b) + " " + to_string(c) + " " + to_string(d);
  }
};

class Polyhedron {
public:
  vector<Plane> planes;
  Polyhedron() {
  }
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
