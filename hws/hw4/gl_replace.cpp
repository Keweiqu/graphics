#include "gl_replace.hpp"
using namespace std;
void print_vec2(glm::vec2 v) {
  cout << v[0] << endl;
}

void translatef(GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res) {
  glm::vec3 trans = glm::vec3(x, y, z);
  *res = glm::translate(*res, trans);
}


void my_translatef(GLfloat x, GLfloat y, GLfloat z, mat4 &m) {
  mat4 trans_mat;
  trans_mat[12] = x;
  trans_mat[13] = y;
  trans_mat[14] = z;
  m = m * trans_mat;
}


/*
 * angles in degree
 */
void rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res) {
  angle = angle / 180 * M_PI;
  glm::vec3 axis = glm::vec3(x, y, z);
  axis = glm::normalize(axis);
  *res = glm::rotate(*res, angle, axis);
}

/*
 * angles in degree
 */
void my_rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, mat4& m) {
  vec3 rot_axis = vec3(x, y, z);
  rot_axis = vec3::normalize(rot_axis);
  x = rot_axis[0];
  y = rot_axis[1];
  z = rot_axis[2];

  GLfloat angle_in_radian = angle / 180.0 * M_PI;
  GLfloat c = cos(angle_in_radian);
  GLfloat s = sin(angle_in_radian);
  
  mat4 rot_mat;
  rot_mat[0] = x * x * (1 - c) + c;
  rot_mat[1] = y * x * (1 - c) + z * s;
  rot_mat[2] = x * z * (1 - c) - y * s;

  rot_mat[4] = x * y * (1 - c) - z * s;
  rot_mat[5] = y * y * (1 - c) + c;
  rot_mat[6] = y * z * (1 - c) + x * s;

  rot_mat[8] = x * z * (1 - c) + y * s;
  rot_mat[9] = y * z * (1 - c) - x * s;
  rot_mat[10] = z * z * (1 - c) + c;
  m = m * rot_mat;
}

void scalef(GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res) {
  glm::vec3 scale = glm::vec3(x, y, z);
  *res = glm::scale(*res, scale);
}

void my_scalef(GLfloat x, GLfloat y, GLfloat z, mat4 &m) {
  mat4 scale_mat;
  scale_mat[0] = x;
  scale_mat[5] = y;
  scale_mat[10] = z;
  m = m * scale_mat;
}

void lookat(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	    GLfloat centerX, GLfloat centerY, GLfloat centerZ,
	    GLfloat upX, GLfloat upY, GLfloat upZ,
	    glm::mat4 * view) {
  /*
  glm::vec3 forward = glm::vec3(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
  forward = glm::normalize(forward);
  glm::vec3 up = glm::vec3(upX, upY, upZ);
  glm::vec3 side = glm::cross(forward, up);
  glm::normalize(side);
  up = glm::cross(side, up);
  forward = forward * -1.0f;
  (*view)[0] = glm::vec4(side[0], up[0], forward[0], 0);
  (*view)[1] = glm::vec4(side[1], up[1], forward[1], 0);
  (*view)[2] = glm::vec4(side[2], up[2], forward[2], 0);
  (*view)[3] = glm::vec4(0, 0, 0, 1);
  *view = *project * *view;
  translatef(-eyeX, -eyeY, -eyeZ, view);
  print_mat(*view);
  */
  ///*
  glm::vec3 eye = glm::vec3(eyeX, eyeY, eyeZ);
  glm::vec3 center = glm::vec3(centerX, centerY, centerZ);
  glm::vec3 up = glm::vec3(upX, upY, upZ);
  *view = glm::lookAt(eye, center, up);
  // */
}
void print_mat(glm::mat4 mat) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      cout << mat[j][i] << " ";
    }
    cout << endl;
  }
}
