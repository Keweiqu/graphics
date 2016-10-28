#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include "common.hpp"
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef struct _metadata {
  GLuint num_of_vertices;
  GLuint num_of_indices;
  GLuint offset; // offset for vertices and normals
  GLuint flat_offset;
  GLuint indices_offset;
} metadata;

class meshManager {
public:
  vector<GLfloat> *vertices_normals;
  vector<GLuint> *indices;
  map< GLuint, vector<GLuint> > *index_faces;
  map< string, metadata > *filename_metadata;
  vector<GLfloat> *flat_vertices_normals;
  vector<string> draw_sequence;
  meshManager();
};



#endif
