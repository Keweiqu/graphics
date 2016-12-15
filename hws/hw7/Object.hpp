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

extern vector<Plane*> planes;

class Polyhedron {
public:
  int num_faces;
  unsigned int start_index;
  Polyhedron(int num_faces, unsigned int start_index) {
    this->num_faces = num_faces;
    this->start_index = start_index;
  }
  string toString() {
    string s = "polyhedron: \n";
    for (unsigned int i = 0; i < planes.size(); i++) {
      s = s + planes[0]->toString() + "\n";
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
  void apply_trans() {
    if (ot == SPHERE) {
      for (unsigned int i = 0; i < trans.size(); i++) {
        if (trans[i].type == SCALE) {
          sphere.radius *= trans[i].vec[0];
        } else {
          sphere.center = sphere.center + trans[i].vec;
        }
      }
    } else if (ot == PLANE) {
      for (unsigned int i = 0; i < trans.size(); i++) {
        if (trans[i].type == TRANSLATE) {
          plane.d = plane.d - plane.a * (trans[i].vec[0]) - plane.b * trans[i].vec[1] - plane.c * trans[i].vec[2];
        }
      }
    }
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
