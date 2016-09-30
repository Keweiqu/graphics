#ifndef BOID_H
#define BOID_H

#include <float.h>
#include "common.h"
#include "linkedlist.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#define SIDES 50
#define NUM_BOID_INDICES 6
#define BOID_COUNT 30
#define NUM_NEIGHBORS 5
#define WORLD_HALF_WIDTH 10000
#define GOAL_V_STEP 10
#define GOAL_H_STEP 100
#define GOAL_VERTICAL_DELTA 1
#define GOAL_HORIZONTAL_DELTA 20
#define VECTOR_LENGTH 3
#define NUM_SQUARE_VERTICES 4
#define BOID_INITIAL_X 2375
#define BOID_INITIAL_Y 125
#define BOID_INITIAL_Z 1175
#define BOID_INITIAL_RANGE 51
#define GOAL_INITIAL_ANGLE 0
#define GOAL_RADIUS 1500
#define GOAL_UPDATE_ANGLE 0.005
#define X 0
#define Y 1
#define Z 2
#define SEPARATION_SCALE -1
#define COHESION_SCALE 0.0001
#define ALIGNMENT_SCALE 0.001
#define GOAL_SEEKING_SCALE 0.00003
#define MATRIX_SIZE 16
#define MAX_WING_ANGLE 30
#define WING_DELTA 2

enum VIEW_MODE {CENTER, TRAILING, SIDE};
typedef struct _boid {
  int id;
  gsl_vector *normal;
  gsl_vector *location;
  gsl_vector *velocity;
  GLfloat z_angle;
  GLfloat angle;
  GLfloat wing_angle;
  GLfloat wing_direction;
  GLfloat dist; //neighbour dist
} Boid;

typedef struct _goal {
  GLfloat speed;
  int h_move;
  int x_move;
  gsl_vector* direction;
  gsl_vector * trans;
} Goal;

typedef struct _view {
  gsl_vector * pos;
  gsl_vector * look;
  gsl_vector * up;
} View;

extern GLfloat boid_vertices[][VECTOR_LENGTH];
extern GLfloat boid_colors[][VECTOR_LENGTH];
extern GLubyte boid_indices[];
extern GLubyte left_indices[];
extern GLubyte right_indices[];
extern GLubyte shadow_vertices[][VECTOR_LENGTH];
extern GLubyte shadow_colors[][VECTOR_LENGTH];
extern GLfloat goal_vertices[VECTOR_LENGTH];
extern GLfloat goal_colors[VECTOR_LENGTH];
extern float world_scale[VECTOR_LENGTH];

enum VIEW_MODE v_mode;
double ave_multiplier;
GLfloat angle;

int count;
GLshort isPaused;
double x_pos, y_pos;
GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
GLshort board_indices[SIDES*SIDES*NUM_SQUARE_VERTICES];
Node* head, *tail;
Goal g;
Boid** cache;
View center_view, trailing_view, side_view;
View* view;
void init();
void init_views();
void update_view();
void init_center_view();
void init_trailing_view();
void init_side_view();
void update_center_view();
void update_trailing_view();
void update_side_view();
void camera_look();
void draw_checkerboard();
void draw_boid(Boid* b);
void draw_boids();
void draw_left_wing(Boid* b);
void draw_right_wing(Boid* b);
void draw_shadow(Boid* b);
void draw_shadows();
void draw_speed(Boid* b);
void calc_checkerboard_vertices(int n, GLfloat len);
void calc_checkerboard_indices(int n);
void calc_checkerboard_colors(int n);
Goal init_goal();
void draw_goal();
void update_goal(Goal *g);
void update_goal_height(Goal *g);
void update_goal_horizontal(Goal *g);
Boid** n_neighbours(Boid *target, Boid** list, int size, int n);
Boid* init_boid(int count);
void init_boids();
Boid** cache_linkedlist(Node* head);
int cmp(const void* a, const void* b);
GLfloat get_dist(Boid* a, Boid *b);
void add_boid();
void delete_boid();
void update_boids();
void update_boid(Boid* b, Boid** neighbors, Goal g);
void update_boid_wing(Boid* b);

void print_boid(Boid* b);
void print_boids(Node* head);
void print_view(View v);
void print_vector(gsl_vector *v);
void print_boids_array(Boid** bs, int size);

gsl_vector* separation(Boid* b, Boid** neighbors);
gsl_vector* cohesion(Boid* b, Boid** neighbors);
gsl_vector* alignment(Boid* b, Boid** neighbors);
gsl_vector* goal_seeking(Goal g, Boid* b);

gsl_vector* trailing_position(Boid** bs, int size, Goal g);
gsl_vector* side_position(Boid** bs, int size, Goal g);
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
void project_point(GLfloat* o, GLfloat* p, GLshort size);
double sum_vector(gsl_vector *v, int size);
double point_dist(gsl_vector *v, gsl_vector *w);
double get_xy_angle(Boid *b);
double get_z_angle(Boid *b);
double get_vector_length(gsl_vector* v);

void lookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void crossProduct(gsl_vector* u, gsl_vector* v, gsl_vector* r);
void testCP();
void testLookAt();

void free_view(View v);
void free_views();
void free_boid(Boid* b);
void free_boids();
#endif
