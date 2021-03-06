#include "common.h"
#include <gsl/gsl_vector.h>

GLfloat boid_vertices[][3] = {
  {0.0, 0.0 ,0},
  {0.0, 20, 0},
  {-20, -10, 0},
  {20, -10, 0}
};

GLfloat shadow_vertices[][3] = {
  {0.0, 0.0 ,0},
  {0.0, 20, 0},
  {-20, -10, 0},
  {20, -10, 0}
};

GLfloat boid_colors[][3] = {
  {1.0, 0.0, 0.0},
  {1.0, 1.0, 0.0},
  {1.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
};

GLubyte boid_indices[] = {
  0, 1, 2,
  3, 1, 0
};

GLubyte left_indices[] = {
  0, 1, 2
};

GLubyte right_indices[] = {
  3, 1, 0
};

GLfloat shadow_colors[][3] = {
  {0.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
};

GLfloat goal_vertices[][3] = {
  {0.0, 0.0, 0.0},
};

GLfloat goal_colors[][3] = {
  {0.0, 0.0, 1.0},
};

float world_scale[3] = {0.0001, 0.0001, 0.0005};

