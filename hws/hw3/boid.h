#include "common.h"
#include "vector/gsl_vector.h"

#define SIDES 3
#define NUM_BOID_INDICES 6
GLfloat boid_vertices[][3] = {
  {0.0, 0.0 ,0.0},
  {0.0, 0.3, 0.0},
  {-0.3, 0.0, 0.0},
  {0.3, 0.0, 0.0}
};
GLfloat boid_colors[][3] = {
  {0.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
  {0.0, 0.0, 0.0},
};

GLubyte boid_indices[] = {
  0, 1, 2,
  0, 3, 1
};

typedef struct _boid {
  int id;
  gsl_vector *coords;//center
  gsl_vector *velocity;
} Boid;

GLfloat angle;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][3];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][3];
GLshort board_indices[SIDES*SIDES*4];


void init();
void draw_checkerboard();
void draw_boid();
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
