#include "common.hpp"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
void print_vec2(glm::vec2);
void translatef(GLfloat x, GLfloat y, GLfloat z, glm::mat4* mat);
void rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res);
void scalef(GLfloat x, GLfloat y, GLfloat z, glm::mat4 *res);
void lookat(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	    GLfloat centerX, GLfloat centerY, GLfloat centerZ,
	    GLfloat upX, GLfloat upY, GLfloat upZ,
	    glm::mat4 * view);
void print_mat(glm::mat4 mat);
