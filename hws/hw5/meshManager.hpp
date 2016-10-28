#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include "common.hpp"
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef struct _metadata {
  GLuint num_of_vertices;
  GLuint num_of_faces;
  GLuint vertices_offset;
  GLuint indices_offset;
  GLuint flat_offset;
} metadata;

class meshManager {
public:
  vector<GLfloat> *vertices;
  vector<GLfloat> *normals;
  vector<GLuint> *indices;
  map< GLuint, vector<GLuint> > *index_faces;
  map< string, metadata > *filename_metadata;
  vector<GLfloat> *flat_vertices;
  vector<GLfloat> *flat_normals;
  vector<string> draw_sequence;
  metadata md;
  meshManager();
};



#endif
