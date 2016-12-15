#include "Object.hpp"

Polyhedron::Polyhedron() {}

void Polyhedron::addPlane(Plane p) {
  planes.push_back(p);
}

string Polyhedron::toString() {
  string s = "polyhedron: \n";
  for (unsigned int i = 0; i < planes.size(); i++) {
    s = s + planes[0].toString() + "\n";
  }
  return s;
}

Object::Object() {}

Object::Object(Pigment pigment, SurfaceFinish surface): p(pigment), sf(surface) {
}

Object::~Object() {}

void Object::addTrans(Trans t) {
  trans.push_back(t);
}

string Object::toString() {
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
