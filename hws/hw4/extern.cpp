#include "common.hpp"
//Front face clockwise 0, 1, 2, 3
//Back face clockwise, 4, 5, 6, 7
GLfloat goal_vertices[] = {
  -10, -10, 10,
  10, -10, 10,
  -10, -10, -10,
  10, -10, -10,
  -10, 10, 10,
  10, 10, 10,
  -10, 10, -10,
  10, 10, -10
};

GLfloat goal_colors[8][4] = {
  {0.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0, 1.0},
  {1.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 1.0, 1.0},
  {0.0, 1.0, 1.0, 1.0},
  {0.0, 1.0, 0.0, 1.0},
  {0.0, 1.0, 1.0, 1.0}
};

GLubyte goal_indices[] = {
  0, 2, 3,
  1, 0, 3,

  1, 3, 7,
  5, 1, 7,

  5, 7, 4,
  7, 6, 4,

  0, 4, 6,
  2, 0, 6,

  4, 0, 1,
  5, 4, 1,

  7, 3, 2,
  6, 7, 2
};
