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
enum Status {DEAD, DYING, ALIVE};

typedef struct _bullet {
  int direction;
  GLfloat x_coord;
  GLfloat y_coord;
  GLfloat angle;
  GLfloat x_trans;
  GLfloat y_trans;
} Bullet;

typedef struct _alien {
  enum Status status;
  GLfloat x_coord;
  GLfloat y_coord;
} Alien;

typedef struct _legion {
  Alien army[N_ROW][N_COL];
  int advanced;
  int direction;
  double march_interval;
  GLfloat left_bound;
  GLfloat right_bound;
  GLfloat x_trans;
  GLfloat y_trans;
} Legion;

typedef struct _self {
  int lives;
  GLfloat x_coord;
  GLfloat y_coord;
  GLfloat x_trans;
  GLfloat y_trans;
  int shooting;
  Bullet b;
  int direction;
  int duration;
} Self;



void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
Alien create_alien(int row, int col);
void create_legion(Legion* legion);
void draw_legion(Legion* legion);
void draw_alien(Alien a);
void update_trans(Legion* legion);
Self create_self();
void draw_self(Self s);
void move_self();
void self_shoot_bullet();
void alien_shoot_bullet();
void draw_bullet(Bullet* b);
Bullet create_bullet(int direction);
void check_collision_self_bullet(Bullet b, Legion *legion);

#endif
