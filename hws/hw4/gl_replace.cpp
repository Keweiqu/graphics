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
  glm::vec3 eye = glm::vec3(eyeX, eyeY, eyeZ);
  glm::vec3 center = glm::vec3(centerX, centerY, centerZ);
  glm::vec3 up = glm::vec3(upX, upY, upZ);
  *view = glm::lookAt(eye, center, up);
}

void my_lookat(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	       GLfloat centerX, GLfloat centerY, GLfloat centerZ,
	       GLfloat upX, GLfloat upY, GLfloat upZ,
	       mat4& m) {
  vec3 forward = vec3(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
  forward = vec3::normalize(forward);
  vec3 up = vec3(upX, upY, upZ);
  vec3 side = vec3::cross(forward, up);
  side = vec3::normalize(side);
  up = vec3::cross(side, forward);
  forward = forward * -1.0f;

  m[0] = side[0];
  m[1] = up[0];
  m[2] = forward[0];
  m[3] = 0;
  
  m[4] = side[1];
  m[5] = up[1];
  m[6] = forward[1];
  m[7] = 0;

  m[8] = side[2];
  m[9] = up[2];
  m[10] = forward[2];
  m[11] = 0;
  
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
  my_translatef(-eyeX, -eyeY, -eyeZ, m);
}

/*
 * fovy in degree
 * source: https://www.opengl.org/wiki/GluPerspective_code
 */
void my_perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, mat4& m) {
  GLfloat ymax = zNear * tan(fovy * M_PI / 360.0);
  GLfloat ymin = -ymax;
  GLfloat xmax = ymax * aspect;
  GLfloat xmin = ymin * aspect;
  
  my_frustum(xmin, xmax, ymin, ymax, zNear, zFar, m);
}

/*
 * source: https://www.opengl.org/wiki/GluPerspective_code
 */
void my_frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar, mat4& m) {
  GLfloat temp, temp2, temp3, temp4;
  temp = 2.0 * zNear;
  temp2 = right - left;
  temp3 = top - bottom;
  temp4 = zFar - zNear;

  m[0] = temp / temp2;
  m[1] = 0.0;
  m[2] = 0.0;
  m[3] = 0.0;

  m[4] = 0.0;
  m[5] = temp / temp3;
  m[6] = 0.0;
  m[7] = 0.0;

  m[8] = (right + left) / temp2;
  m[9] = (top + bottom) / temp3;
  m[10] = (-zFar - zNear) / temp4;
  m[11] = -1.0;
  
  m[12] = 0.0;
  m[13] = 0.0;
  m[14] = (-temp * zFar) / temp4;
  m[15] = 0.0;

}


void print_mat(glm::mat4 mat) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      cout << mat[j][i] << " ";
    }
    cout << endl;
  }
}
