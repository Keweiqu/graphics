#ifndef BOID_H
#define BOID_H

#include <float.h>
#include "common.h"
#include "linkedlist.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#define SIDES 50
#define NUM_BOID_INDICES 6
#define BOID_COUNT 10
#define NUM_NEIGHBORS 5

enum VIEW_MODE {CENTER, TRAILING, SIDE};
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
  gsl_vector * trans;
} Goal;

typedef struct _view {
  gsl_vector * pos;
  gsl_vector * look;
  gsl_vector * up;
} View;

extern GLfloat boid_vertices[][3];
extern GLfloat boid_colors[][3];
extern GLubyte boid_indices[];
extern GLfloat goal_vertices[3];
extern GLfloat goal_colors[3];
extern float world_scale[3];
double ave_multiplier;
GLfloat angle;

int count;
GLshort isPaused;
double x_pos, y_pos;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][3];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][3];
GLshort board_indices[SIDES*SIDES*4];
Node* head, *tail;
Goal g;
Boid** cache;
View center_view, trailing_view, side_view;
View view;
void init();
void init_views();
void update_view();
void init_center_view();
void init_trailing_view();
void update_center_view();
void update_trailing_view();
void camera_look();
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
void print_view(View v);
void print_vector(gsl_vector *v);
void print_boids_array(Boid** bs, int size);

gsl_vector* separation(Boid* b, Boid** neighbors);
gsl_vector* cohesion(Boid* b, Boid** neighbors);
gsl_vector* alignment(Boid* b, Boid** neighbors);
gsl_vector* goal_seeking(Goal g, Boid* b);

gsl_vector* center_goal_direction(Boid** bs, int size, Goal g);
double center_goal_dist(Boid** bs, int size, Goal);
gsl_vector* get_flock_center(Boid** bs, int size);
gsl_vector* ave(gsl_vector* v, gsl_vector* w);
gsl_vector* calc_middleway(Boid** bs, int size, Goal g);

void normalize_vector(gsl_vector* v, int size);
void world_scale_vector(gsl_vector *v);
void keyboard(GLFWwindow *w, int key, int scancode,  int action, int mods);
void cursor(GLFWwindow* w, double xpos, double ypos);

double max_boid_goal_dist(Boid** bs, int size, Goal g);
double projection_cos(gsl_vector *v, gsl_vector *);
double sum_vector(gsl_vector *v, int size);
double point_dist(gsl_vector *v, gsl_vector *w);
double get_angle(Boid *b);

void lookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void crossProduct(gsl_vector* u, gsl_vector* v, gsl_vector* r);
void testCP();
void testLookAt();
#endif
