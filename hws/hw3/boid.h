#include "common.h"
#include "vector/gsl_vector.h"

#define SIDES 2

typedef struct _boid {
  gsl_vector *coords;
  gsl_vector *velocity;
} Boid;

GLfloat angle;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][3];
GLubyte board_indices[SIDES*SIDES*4];
GLubyte board_colors[(SIDES+1)*(SIDES+1)][3];

void init();
void draw_checkerboard();
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
