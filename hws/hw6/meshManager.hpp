#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include "common.hpp"
#include "util.hpp"
// #include "main.hpp"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cfloat>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#define WIDTH 10.0f
#define ROTATE_X_SPEED 0.01
#define ROTATE_Y_SPEED 0.01
#define ROTATE_Z_SPEED 0.01

using namespace std;

class meshManager {
public:
  GLuint vbo_pos;
  GLuint vbo_tex;
  GLuint vbo_normal;
  GLuint ebo;
  GLuint vao;
  
  GLfloat scale;
  glm::vec3 trans_vec;

  vector<GLfloat> *vertices;
  vector<GLfloat> *normals;
  vector<GLfloat> *tex_coords;
  vector<GLuint> *indices;

  GLuint num_of_vertices;
  GLuint num_of_indices;
  
  map< GLuint, vector<GLuint> > *index_faces; // per mesh

  meshManager();
  ~meshManager();
  void readFile(string filename);
  void init();
  void draw();
private:
  vector<glm::vec3> *face_normals; // should be for per mesh
  void calc_normal();// next round of meshManager offsets should be available
  glm::vec3 calc_face_normal(GLuint v0, GLuint v1, GLuint v2);// v_offset should be added

};

#endif
