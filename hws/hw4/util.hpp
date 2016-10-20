#ifndef UTIL_H
#define UTIL_H
#include "common.hpp"
#include "Flock.hpp"
#include "gl_replace.hpp"
#include <cmath>

#define VECTOR_LENGTH 3
#define SIDES 50
#define NUM_SQUARE_VERTICES 4
#define CENTER 0
#define TRAILING 1
#define SIDE 2

struct View {
  vec3 pos;
  vec3 look;
  vec3 up;
};

void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
void calc_checkerboard_vertices(int n, GLfloat len);

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_checkerboard(Flock* f, GLuint matrix, GLuint vao, GLuint index);

void init_views();
void update_view();
void init_center_view();
void init_trailing_view();
void init_side_view();
void update_center_view();
void update_trailing_view();
void update_side_view();
void camera_look();
vec3 calc_middleway();
vec3 trailing_position();
vec3 side_position();
vec3 ave_flock_center();
void world_scale_vector(vec3 *v);
GLfloat max_boid_goal_dist();
GLfloat center_goal_dist();

#endif
