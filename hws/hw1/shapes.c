#include "shapes.h"

GLfloat calc_radius(Point* center, Point * edge) {
  GLfloat delta_x = center->x - edge->x;
  GLfloat delta_y = center->y - edge->y;
  return sqrt(delta_x * delta_x + delta_y * delta_y);
}

void calc_circle_indices(Circle* circle) {
  unsigned int i;
  for(i = 0; i <= NUM_SLICE; i++) {
    circle->x_cache[i] = circle->center->x + circle->radius * cos(i * DELTA);
    circle->y_cache[i] = circle->center->y + circle->radius * sin(i * DELTA);
  }
}

Point *make_point(GLfloat x, GLfloat y, GLfloat color[]) {
  Point *p;
  if( (p = (Point *) malloc(sizeof(Point))) != NULL) {
    p->show = TRUE;
    p->x = x;
    p->y = y;
    p->color = color;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return p;
}

Line *make_line(Point* v1, Point* v2, GLfloat color[]) {
  Line * line;
  if((line = (Line *) malloc(sizeof(Line))) != NULL) {
    line->v1 = v1;
    line->v2 = v2;
    line->color = color;
    line->dashed = TRUE;
    line->fill = FALSE;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return line;
}

Triangle *make_triangle(Point* v1, Point* v2, Point* v3, GLfloat color[]) {
  Triangle* tri;
  if((tri = (Triangle *) malloc(sizeof(Triangle))) != NULL) {
    if(v1->x < v2->x){
      tri->v1 = v1;
      tri->v2 = v2;
    } else {
      tri->v1 = v2;
      tri->v2 = v1;
    }
    GLfloat slope = (tri->v1->y - tri->v2->y) / (tri->v1->x - tri->v2->x);
    if(slope * (v3->x) <= v3->y){
      tri->v3 = v3;
    } else {
      Point* temp = tri->v2;
      tri->v2 = v3;
      tri->v3 = temp;
    }
    tri->color = color;
    tri->dashed = TRUE;
    tri->fill = FALSE;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return tri;
}

Rect *make_rect(Point* diag1, Point* diag2, GLfloat color[]) {
  Rect* rect;
  if((rect = (Rect *) malloc(sizeof(Rect))) != NULL) {
    rect->v1 = diag1;
    rect->v2 = make_point(diag2->x, diag1->y, color);
    rect->v3 = diag2;
    rect->v4 = make_point(diag1->x, diag2->y, color);
    rect->color = color;
    rect->dashed = TRUE;
    rect->fill = FALSE;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return rect;
}

Circle * make_circle(Point* diag1, Point* diag2, GLfloat color[]) {
  Circle * circle;
  if((circle = (Circle *) malloc(sizeof(Circle))) != NULL) {
    circle->v1 = diag1;
    circle->v2 = diag2;
    circle->center = make_point((diag1->x + diag2->x)/2, (diag1->y + diag2->y) /2, color);
    circle->radius = calc_radius(circle->center, circle->v1);
    circle->x_cache = (GLfloat *) malloc(sizeof(GLfloat) * (NUM_SLICE + 1));
    circle->y_cache = (GLfloat *) malloc(sizeof(GLfloat) * (NUM_SLICE + 1));
    calc_circle_indices(circle);
    circle->color = color;
    circle->dashed = TRUE;
    circle->fill = FALSE;
  }else {
    printf("Out of memory\n");
    exit(0);
  }
  return circle;
}

Shape *make_shape(int type, void *data) {
  Shape* shape;
  if((shape = (Shape *) malloc(sizeof(Shape))) != NULL) {
    shape->type = type;
    shape->data = data;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return shape;
}

void insert_shape(Shape* tail, Shape* new_shape){
  new_shape->next = tail;
  new_shape->prev = tail->prev;
  tail->prev->next = new_shape;
  tail->prev = new_shape;
}

void print_list(Shape* head) {
  printf("%d\n", head->type);
  Shape* current = head->next;
  while(current->type != -1){
    printf("%d\n", current->type);
    current = current->next;
  }
  printf("%d\n", current->type);
}
