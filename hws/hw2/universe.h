#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define LEFT -1
#define RIGHT 1
#define UP 1
#define DOWN -1
#define N_ROW 5
#define N_COL 11
#define WIDTH 700
#define HEIGHT 700
#define DELTA 0.1
#define TIMESTAMP 1
#define INIT_DURATION 5
#define SPACING 0.12
#define HALF_WIDTH 0.04
#define BULLET_SPEED 0.01
#define POINT_SIZE 15
#define FIRE_LOAD 40
#define LEFT_BOUND -0.90
#define RIGHT_BOUND 0.90
enum Status {DEAD, DYING, ALIVE};

typedef struct _bullet {
  int fired;
  int direction;
  GLfloat x_coord;
  GLfloat y_coord;
  GLfloat angle;
  GLfloat x_trans;
  GLfloat y_trans;
} Bullet;

typedef struct _coord {
  GLfloat x_coord;
  GLfloat y_coord;
  GLfloat x_trans;
  GLfloat y_trans;
  GLfloat width;
  int draw;
} Coord;

typedef struct _alien {
  enum Status status;
  int angle;
  GLfloat scale;
  Bullet b;
  GLfloat x_coord;
  GLfloat y_coord;
} Alien;

typedef struct _legion {
  Alien army[N_ROW][N_COL];
  int advanced;
  int direction;
  double march_interval;
  int left_bound;
  int right_bound;
  GLfloat x_trans;
  GLfloat y_trans;
} Legion;

typedef struct _self {
  int lives;
  int move;
  GLfloat x_coord;
  GLfloat y_coord;
  GLfloat x_trans;
  GLfloat y_trans;
  GLfloat element_width;
  Coord elements[9];
  int shooting;
  Bullet fire[FIRE_LOAD];
  int fire_pointer;
  int direction;
  int duration;
} Self;




void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
Alien create_alien(int row, int col);
void create_legion(Legion* legion);
void draw_legion(Legion* legion);
void draw_alien(Alien *a, Legion *legion);
void draw_alien_helper(Alien *a);
void update_trans(Legion* legion);
Self create_self();
void draw_self(Self s);
void move_self();
void self_shoot_bullet();
void alien_shoot_bullet();
void draw_bullet(Bullet* b);
void draw_self_bullets();
Bullet create_bullet(int direction);
void check_collision_self_bullet(Bullet *b, Legion *legion);
void check_collision_legion_bullet(Bullet *b, Self *self);
void check_collision_self(Legion *legion);
void check_collision_legion(Legion *legion, Self *self);
void update_bound(Legion *legion);
int all_dead(Legion *legion, int col);
void legion_fire(Legion *legion);
void draw_legion_bullets(Legion *legion);
Coord create_coord(GLfloat x_coord, GLfloat y_coord, GLfloat x_trans, GLfloat y_trans, GLfloat width);
void draw_elements(Coord *elements, unsigned int size);
#endif
