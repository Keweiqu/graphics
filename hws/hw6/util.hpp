#ifndef UTIL_H
#define UTIL_H
#include "common.hpp"
#include "Flock.hpp"
#include "gl_replace.hpp"
#include "hw4.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include "meshManager.hpp"

#define VECTOR_LENGTH 3
#define SIDES 50
#define NUM_SQUARE_VERTICES 4

#define EYE_DIST_NEAR 14
#define EYE_DIST_FAR 46
#define SCALE_FACTOR_MAX 2.2
#define SCALE_FACTOR_MIN 0.68

enum VIEW_TYPE {CENTER, SIDE, TRAILING, FIRST_PERSON};

class View {
public:
  vec3 pos;
  vec3 look;
  vec3 up;
  View() {
    pos = vec3();
    look = vec3();
    up = vec3();
  }
};

void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_ocean_vertices(GLfloat len);

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_shadows(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_ocean(GLuint vao);
void draw_checkerboard(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void center_view(mat4 &v, Flock& f);
void side_view(mat4 &v, Flock& f);
void update_view(mat4 &view, Flock& f);
void trailing_view(mat4 &view, Flock& f);
void first_person_view(mat4 &view, Flock& f);
vec3 ave_flock_center(Flock& f);
vec3 calc_middleway(Flock& f);
vec3 get_side_pos(Flock& f);
vec3 get_trailing_pos(Flock &f);
vec3 get_center_pos(Flock &f);
GLfloat center_goal_dist(Flock& f);
GLfloat max_boid_goal_dist(Flock& f);
void print_step_msg(Flock* f);
void init_time();
void update_time(int index);
void zoom_in();
void zoom_out();
#endif
