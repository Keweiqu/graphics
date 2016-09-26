#include "common.h"
#include "vector/gsl_vector.h"

#define SIDES 4
#define NUM_BOID_INDICES 6
#define BOID_COUNT 10
extern GLfloat boid_vertices[][3];
extern GLfloat boid_colors[][3];
extern GLubyte boid_indices[];
extern GLfloat goal_vertices[3];
typedef struct _boid {
  int id;
  gsl_vector *location;//center
  gsl_vector *velocity;
  GLfloat angle;
  GLfloat dist; //neighbour dist
} Boid;

GLfloat angle;
GLshort count;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][3];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][3];
GLshort board_indices[SIDES*SIDES*4];

typedef struct _goal {
  GLfloat angle;
  GLfloat radius;
} Goal;

void init();
void draw_checkerboard();
void draw_boid();
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
Goal init_goal();
void draw_goal();
void update_goal();
Node* n_neighbours(Node *target, Node *list, int n);
void insert(Node *head, Node *item);
void attemp(Node *head, Node *item);

GLfloat** cache_vertices(Node *head);
Boid* init_boid();
void init_boids();
