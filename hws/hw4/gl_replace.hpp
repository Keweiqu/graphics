#include "common.hpp"
#include "mat4.hpp"
#include "vec3.hpp"

void my_translatef(GLfloat x, GLfloat y, GLfloat z, mat4& m);
void my_rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, mat4& m);
void my_scalef(GLfloat x, GLfloat y, GLfloat z, mat4& m);
void my_lookat(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	       GLfloat centerX, GLfloat centerY, GLfloat centerZ,
	       GLfloat upX, GLfloat upY, GLfloat upZ,
	       mat4& m);
void my_perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, mat4& m);
void my_frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar, mat4& m);
