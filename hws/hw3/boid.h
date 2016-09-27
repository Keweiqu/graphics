#include "common.h"
#include "vector/gsl_vector.h"

#define SIDES 4
#define NUM_BOID_INDICES 6
extern GLfloat boid_vertices[][3];
extern GLfloat boid_colors[][3];
extern GLubyte boid_indices[];

typedef struct _boid {
  int id;
  gsl_vector *location;//center
  gsl_vector *velocity;
  GLfloat angle;
} Boid;

GLfloat angle;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][3];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][3];
GLshort board_indices[SIDES*SIDES*4];

typedef struct _goal {
  GLint angle;
  GLfloat radius;
} Goal;

void init();
void draw_checkerboard();
void draw_boid();
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);

Boid init_boid();
