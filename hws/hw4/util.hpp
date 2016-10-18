#ifndef UTIL_H
#define UTIL_H
#include "common.hpp"
#include <cmath>

#define VECTOR_LENGTH 3
#define SIDES 2
#define NUM_SQUARE_VERTICES 4

void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
void calc_checkerboard_vertices(int n, GLfloat len);

#endif