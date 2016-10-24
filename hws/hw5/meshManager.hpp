#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include "common.hpp"
#include <vector>

using namespace std;

class meshManager {
public:
  vector<GLfloat> *vertices;
  vector<GLuint> *indices;
  meshManager();
};

#endif
