#include "common.h"

GLfloat boid_vertices[][3] = {
  {0.0, 0.0 ,1},
  {0.0, 100, 1},
  {-100, -50, 1},
  {100, -50, 1}
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

GLfloat goal_vertices[][3] = {
  {0.0, 0.0, 1.0},
};

GLfloat gola_colors[][3] = {
  {1.0, 1.0, 0},
};
