#include "gl_replace.hpp"
using namespace std;
void print_vec2(glm::vec2 v) {
  cout << v[0] << endl;
}

void translatef(GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res) {
  glm::vec3 trans = glm::vec3(x, y, z);
  *res = glm::translate(*res, trans);
}

/*
 * angles in degree
 */
void rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res) {
  angle = angle / 180 * 3.14159262728;
  glm::vec3 axis = glm::vec3(x, y, z);
  axis = glm::normalize(axis);
  *res = glm::rotate(*res, angle, axis);
}

void scalef(GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res) {
  glm::vec3 scale = glm::vec3(x, y, z);
  *res = glm::scale(*res, scale);
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
  */
  glm::vec3 eye = glm::vec3(eyeX, eyeY, eyeZ);
  glm::vec3 center = glm::vec3(centerX, centerY, centerZ);
  glm::vec3 up = glm::vec3(upX, upY, upZ);
  *view = glm::lookAt(eye, center, up);
}
void print_mat(glm::mat4 mat) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      cout << mat[j][i] << " ";
    }
    cout << endl;
  }
}
