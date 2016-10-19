#ifndef UTIL_H
#define UTIL_H
#include "gl_replace.hpp"
#include "common.hpp"
#include "Flock.hpp"
#include <cmath>

#define VECTOR_LENGTH 3
#define SIDES 50
#define NUM_SQUARE_VERTICES 4

void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
void calc_checkerboard_vertices(int n, GLfloat len);

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_checkerboard(Flock* f, GLuint matrix, GLuint vao, GLuint index);
#endif
