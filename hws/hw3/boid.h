#include <float.h>
#include "common.h"
#include "linkedlist.h"
#include <gsl/gsl_vector.h>

#define SIDES 4
#define NUM_BOID_INDICES 6
#define BOID_COUNT 10
#define NUM_NEIGHBORS 5

typedef struct _boid {
  int id;
  gsl_vector *normal;
  gsl_vector *location;//center
  gsl_vector *velocity;
  GLfloat angle;
  GLfloat dist; //neighbour dist
} Boid;

typedef struct _goal {
  GLfloat angle;
  GLfloat radius;
  GLfloat x_trans;
  GLfloat y_trans;
  GLfloat z_trans;
} Goal;

extern GLfloat boid_vertices[][3];
extern GLfloat boid_colors[][3];
extern GLubyte boid_indices[];
extern GLfloat goal_vertices[3];
extern GLfloat goal_colors[3];
double ave_multiplier;
GLfloat angle;
GLshort count;
GLshort isPaused;
double x_pos, y_pos;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][3];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][3];
GLshort board_indices[SIDES*SIDES*4];
Node* head, *tail;
Goal g;
Boid** cache;
void init();
void draw_checkerboard();
void draw_boid(Boid* b);
void draw_boids();
void draw_speed(Boid* b);
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
Goal init_goal();
void draw_goal();
void update_goal(Goal *g);
Boid** n_neighbours(Boid *target, Boid** list, int size, int n);
Boid* init_boid(int count);
void init_boids();
Boid** cache_linkedlist(Node* head);
int cmp(const void* a, const void* b);
GLfloat get_dist(Boid* a, Boid *b);
void add_boid();
void update_boids();
void update_boid(Boid* b, Boid** neighbors, Goal g);

void print_boid(Boid* b);
void print_boids(Node* head);
void print_vector(gsl_vector *v);
void print_boids_array(Boid** bs, int size);

gsl_vector* separation(Boid* b, Boid** neighbors);
gsl_vector* cohesion(Boid* b, Boid** neighbors);
gsl_vector* alignment(Boid* b, Boid** neighbors);
gsl_vector* goal_seeking(Goal g, Boid* b);

void keyboard(GLFWwindow *w, int key, int scancode,  int action, int mods);
void cursor(GLFWwindow* w, double xpos, double ypos);

double projection_cos(gsl_vector *v, gsl_vector *);
double sum_vector(gsl_vector *v, int size);
double get_angle(Boid *b);

void lookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void crossProduct(gsl_vector* u, gsl_vector* v, gsl_vector* r);
void testCP();
