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

typedef struct _metadata {
  GLuint num_of_vertices;
  GLuint num_of_indices;
  GLuint num_of_edge_indices;

  GLuint vn_offset;
  GLuint indices_offset;
  GLuint edge_indices_offset;

  GLuint flat_offset;
  GLfloat scale;
  glm::vec3 trans;
} metadata;

class meshManager {
public:
  GLuint vbo_pos;
  GLuint vbo_pos0;
  GLuint vbo_pos1;
  GLuint vbo_normal;
  GLuint ebo;
  GLuint edge_ebo;

  GLuint vao;
  GLuint flat_vao;

  GLuint flat_vbo_pos;
  GLuint flat_vbo_normal;

  GLuint vn_offset;
  GLuint idx_offset;
  GLuint edge_idx_offset;
  GLuint flat_offset;
  vector<GLfloat> *vertices;
  vector<GLfloat> *normals;
  vector<GLuint> *indices;
  vector<GLuint> *edge_indices;

  vector<GLfloat> *flat_normals;
  vector<GLfloat> *flat_vertices;

  map< GLuint, vector<GLuint> > *index_faces; // per mesh
  map< string, metadata > *filename_metadata;

  vector<string> draw_sequence;
  meshManager();
  ~meshManager();
  void readFiles(int num_files, char* argv[]);
  void init();
  void draw_vertex_mode();
  void draw_edge_mode();
  void draw_flat_mode();
  void draw_default();
  void draw();
  void update_angle();
private:
  vector<glm::vec3> *face_normals; // should be for per mesh
  vector<glm::vec3> grid_trans;
  void readFile(char* filename);// after readFile, all data in metadata should be available
  void calc_normal(string filename);// next round of meshManager offsets should be available
  glm::vec3 calc_face_normal(GLuint v0, GLuint v1, GLuint v2);// v_offset should be added
  void calc_grid_trans_and_scale();

};



#endif
