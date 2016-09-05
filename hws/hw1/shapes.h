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


#define POINT 0
#define LINE 1
#define TRIANGLE 2
#define RECT 3
#define CIRCLE 4
#define POLY 5
#define TRUE 1
#define FALSE 0
#define NUM_SLICE 40
#define DELTA 2 * M_PI / NUM_SLICE

typedef struct _shape {
  int type; 
  void *data;
  struct _shape *prev;
  struct _shape *next;
} Shape;

typedef struct _point {
  int show;
  GLfloat x;
  GLfloat y;
  GLfloat* color;
} Point;

typedef struct _line {
  Point* v1;
  Point* v2;
  GLfloat* color;
  int dashed;
  int fill;
} Line;

typedef struct _triangle {
  Point* v1;
  Point* v2;
  Point* v3;
  GLfloat* color;
  int dashed;
  int fill;
} Triangle;

typedef struct _rect {
  Point* v1;
  Point* v2;
  Point* v3;
  Point* v4;
  GLfloat* color;
  int dashed;
  int fill;
} Rect;

typedef struct _circle {
  Point* v1;
  Point* v2;
  Point* center;
  GLfloat radius;
  GLfloat* color;
  int dashed;
  int fill;
} Circle;

GLfloat calc_radius(Point* center, Point* edge);
Point *make_point(GLfloat x, GLfloat y, GLfloat color[]);
Line *make_line(Point* x, Point* y, GLfloat color[]);
Triangle * make_triangle(Point* v1, Point* v2, Point* v3, GLfloat color[]);
Rect * make_rect(Point* diag1, Point* diag2, GLfloat color[]);
Circle * make_circle(Point* diag1, Point* diag2, GLfloat color[]);
Shape *make_shape(int type, void *data);
void insert_shape(Shape* tail, Shape* new_shape);
void print_list(Shape* head);

#endif
