#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include "common.hpp"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct _metadata {
  GLuint num_of_vertices;
  GLuint num_of_indices;

  GLuint vn_offset;
  GLuint indices_offset;

  GLuint flat_vn_offset;
} metadata;

class meshManager {
public:
  GLuint vn_offset;
  GLuint idx_offset;//in readF
  GLuint flat_offset;
  vector<GLfloat> *vertices_normals;
  vector<GLuint> *indices;
  map< GLuint, vector<GLuint> > *index_faces;
  map< string, metadata > *filename_metadata;
  vector<GLfloat> *flat_vertices_normals;
  vector<string> draw_sequence;
  meshManager();
  void readFiles(int num_files, char* argv[]);
private:
  vector<GLfloat> face_normals; // should be for per mesh
  void readFile(char* filename);// after readFile, all data in metadata should be available
  void calc_normal(GLuint idx_offset, GLuint vn_offset, string filename);
};



#endif
