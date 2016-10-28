#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include "common.hpp"
#include <vector>
#include <map>

using namespace std;

class meshManager {
public:
  vector<GLfloat> *vertices;
  vector<GLuint> *indices;
  map< GLuint, vector<GLuint> > *index_faces;
  map< string, metadata > *filename_metadata;
  // vector<vec4> normals;
  metadata md;
  meshManager();
};

class metadata {
public:
	GLuint num_of_vertices;
	GLuint num_of_faces;
};

#endif
