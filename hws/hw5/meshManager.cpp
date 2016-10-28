#include "meshManager.hpp"

meshManager::meshManager() {
  vertices = new vector<GLfloat>();
  indices = new vector<GLuint>();
  index_faces = new map< GLuint, vector<GLuint> >();
  filename_metadata = new map< string, metadata>();
  flat_vertices = new vector<GLfloat>();
  flat_normals = new vector<GLfloat>();
}

