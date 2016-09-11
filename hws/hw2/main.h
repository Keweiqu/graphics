#ifndef SHAPES_H
#define SHAPES_H
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

#endif

#define TRUE 1
#define FALSE 0
#define N_ROW 5
#define N_COL 11
#define WIDTH 500
#define HEIGHT 500

typedef struct _alien {
  int alive;
  GLfloat x_coord;
  GLfloat y_coord;
} Alien;

Alien create_alien(int row, int col);
void init_alien_array(Alien (*alien_array)[N_COL]);
void draw_alien_array(Alien (*alien_array)[N_COL]);
void draw_alien(Alien a);
