#ifndef HW1_H
#define HW1_H

#include "shapes.h"

#define POINT_SIZE 8.0
#define WIDTH 500
#define HEIGHT 500
#define SCALE_X_AXIS(x) x / WIDTH * 2 - 1
#define SCALE_Y_AXIS(y) (y / HEIGHT * 2 - 1) * -1

void toggle_color(GLfloat* target, const GLfloat* option); 
int line_available(Shape* tail);
int triangle_available(Shape* tail);
int rect_available(Shape* tail);
int circle_available(Shape* tail);
int polygon_available(Shape* tail);
int has_line(Shape* tail);
int has_triangle(Shape* tail);
int has_rect(Shape* tail);
int has_circle(Shape* tail);
void exec_cursor_line();
void exec_cursor_triangle();
void exec_cursor_rect();
void exec_cursor_circle();
void exec_cursor_polygon();
void exec_right_mouse_down();
void exec_mouse_down_polygon();
void exec_mouse_down_circle();
void exec_mouse_down_rect();
void exec_mouse_down_triangle();
void exec_mouse_down_line();
void exec_mouse_down_point();
void exec_mouse_up_line();
void exec_mouse_up_triangle();
void exec_mouse_up_rect();
void exec_mouse_up_circle();
void draw_point(Point* point);
void draw_line(Line* line);
void draw_triangle(Triangle* triangle);
void draw_rect(Rect* rect);
void draw_circle(Circle* circle);
void draw_polygon(Polygon* polygon);
void draw_shape(Shape* shape);
void draw_shapes(Shape* shapes);
void free_all_shapes(Shape* head);
int has_circle(Shape* tail);
void exec_cursor_line();
void exec_cursor_triangle();
void exec_cursor_rect();
void exec_cursor_circle();
void exec_mouse_down_circle();
void exec_mouse_down_rect();
void exec_mouse_down_triangle();
void exec_mouse_down_line();
void exec_mouse_down_point();
void exec_mouse_up_line();
void exec_mouse_up_triangle();
void exec_mouse_up_rect();
void exec_mouse_up_circle();
void draw_point(Point* point);
void draw_line(Line* line);
void draw_triangle(Triangle* triangle);
void draw_rect(Rect* rect);
void draw_circle(Circle* circle);
void draw_polygon(Polygon* polygon);
void draw_shape(Shape* shape);
void draw_shapes(Shape* shapes);
void free_all_shapes(Shape* head);
void free_one_shape(Shape* shape);
void free_point(Point* point);
void free_line(Line* line);
void free_triangle(Triangle* triangle);
void free_rect(Rect* rect);
void free_circle(Circle* circle);
void remove_prev(Shape* shape);
#endif
