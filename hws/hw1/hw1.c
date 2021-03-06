#include "hw1.h"

double mouseX, mouseY;
int shape_mode = LINE;
GLfloat color[] = {0.0, 0.0, 0.0};
int pressed;

int fill = TRUE;
Point *pressCoordinates;
Point *releaseCoordinates;
GLenum drawMode;
Shape *head, *tail;
int global_width = WIDTH, global_height = HEIGHT;
const GLfloat RED[] = {1.0, 0.0, 0.0};
const GLfloat GREEN[] = {0.0, 1.0, 0.0};
const GLfloat BLUE[] = {0.0, 0.0, 1.0};
const GLfloat BLACK[] = {0.0, 0.0, 0.0};

void remove_prev(Shape* shape) {
  Shape* current = shape->prev;
  shape->prev = current->prev;
  free_one_shape(current);
}
void free_one_shape(Shape* shape) {
  shape->prev = NULL;
  shape->next = NULL;
  switch(shape->type){
  case POINT:
    free_point((Point *) shape->data);
    break;
  case LINE:
    free_line((Line *) shape->data);
    break;
  case TRIANGLE:
    free_triangle((Triangle *) shape->data);
    break;
  case RECT:
    free_rect((Rect *) shape->data);
    break;
  case CIRCLE:
    free_circle((Circle *) shape->data);
    break;
  case POLY:
    free_polygon((Polygon *) shape->data);
    break;
  default:
    break;
  }
  free(shape);
}

void free_all_shapes(Shape* head) {
  while(head->next->type != -1) {
    Shape* current = head->next;
    head->next = head->next->next;
    free_one_shape(current);
  }
  head->next->prev = head;
}

void free_point(Point* point) {
  free(point);
}

void free_line(Line * line) {
  if(line->color != color) {
    free(line->color);
  }
  free(line);
}

void free_triangle(Triangle * triangle) {
  if(triangle->color != color) {
    free(triangle->color);
  }
  free(triangle);
}

void free_rect(Rect* rect) {
  free(rect->v2);
  free(rect->v4);
  if(rect->color != color) {
    free(rect->color);
  }
  free(rect);
}

void free_circle(Circle* circle) {
  free(circle->x_cache);
  free(circle->y_cache);
  free(circle->center);
  if(circle->color != color) {
    free(circle->color);
  }
  free(circle);
}

void free_polygon(Polygon* polygon) {
  free_all_shapes(polygon->head);
  free_one_shape(polygon->head);
  free_one_shape(polygon->tail);
  free(polygon->cache);
  free(polygon);
}

void toggle_color(GLfloat* target, const GLfloat* option){
  if(target[0] == option[0] && target[1] == option[1] && target[2] == option[2]){
    memcpy(target, BLACK, sizeof(GLfloat) * 3);
  } else {
    memcpy(target, option, sizeof(GLfloat) *3);
  }
}
int line_available(Shape *tail) {
  return tail->prev->type == POINT &&
    ((Point *)(tail->prev->data))->show &&
    tail->prev->prev->type == POINT &&
    ((Point *)(tail->prev->prev->data))->show;
}

int triangle_available(Shape *tail) {
  return tail->prev->type == LINE && ((Line *)(tail->prev->data))->dashed == TRUE;
}

int rect_available(Shape* tail) {
  return tail->prev->type == POINT &&
    ((Point *)(tail->prev->data))->show &&
    tail->prev->prev->type == POINT &&
    ((Point *)(tail->prev->prev->data))->show;
}

int circle_available(Shape* tail) {
  return tail->prev->type == POINT &&
    ((Point *)(tail->prev->data))->show &&
    tail->prev->prev->type == POINT &&
    ((Point *)(tail->prev->prev->data))->show;
}

int polygon_available(Shape* tail) {
  return tail->prev->type == POLY &&
    ((Polygon *)(tail->prev->data))->finished == FALSE;
}

int has_circle(Shape* tail) {
  return tail->prev->type == CIRCLE;
}

int has_rect(Shape* tail) {
  return tail->prev->type == RECT;
}

int has_triangle(Shape* tail) {
  return tail->prev->type == TRIANGLE;
}

int has_line(Shape* tail) {
  return tail->prev->type == LINE;
}

void exec_cursor_line() {
  if(pressed){
    if(tail->prev->type != LINE) {
      Point * start_point = (Point *)tail->prev->data;
      Point * end_point = make_point(mouseX, mouseY, color);
      Line * line = make_line(start_point, end_point, color);
      insert_shape(tail, make_shape(POINT, end_point));
      insert_shape(tail, make_shape(LINE, line));
    }else{
      ((Point *)(tail->prev->prev->data))->x = mouseX;
      ((Point *)(tail->prev->prev->data))->y = mouseY;
    }
  }
}

void exec_cursor_triangle(){
  if(!has_triangle(tail)){
    exec_cursor_line();
  }else{
    if(pressed){
      ((Point *)(tail->prev->prev->data))->x = mouseX;
      ((Point *)(tail->prev->prev->data))->y = mouseY;
    }
  }
}

void exec_cursor_rect() {
  if(pressed) {
    if(!has_rect(tail)) {
      Point * start_point = (Point *)tail->prev->data;
      Point * end_point = make_point(mouseX, mouseY, color);
      Rect * rect = make_rect(start_point, end_point, color);
      insert_shape(tail, make_shape(POINT, end_point));
      insert_shape(tail, make_shape(RECT, rect));
    } else {
      ((Point *)(tail->prev->prev->data))->x = mouseX;
      ((Point *)(tail->prev->prev->data))->y = mouseY;
      ((Rect *)(tail->prev->data))->v2->x = mouseX;
      ((Rect *)(tail->prev->data))->v4->y = mouseY;
    }
  }
}

void exec_cursor_circle() {
  if(pressed) {
    if(!has_circle(tail)) {
      Point * start_point = (Point *)tail->prev->data;
      Point * end_point = make_point(mouseX, mouseY, color);
      Circle * circle = make_circle(start_point, end_point, color);
      insert_shape(tail, make_shape(POINT, end_point));
      insert_shape(tail, make_shape(CIRCLE, circle));
    } else {
      ((Point *)(tail->prev->prev->data))->x = mouseX;
      ((Point *)(tail->prev->prev->data))->y = mouseY;
      Circle* circle = (Circle *) tail->prev->data;
      circle->center->x = (circle->v1->x + mouseX) / 2;
      circle->center->y = (circle->v1->y + mouseY) / 2;
      circle->radius = calc_radius(circle->center, circle->v1);
      calc_circle_indices(circle);
    }
  }
}

void exec_cursor_polygon() {
  if(pressed) {
    Polygon * polygon = (Polygon *) tail->prev->data;
    if(count_polygon_sides(polygon) == 1){
      Point* new_point = make_point(mouseX, mouseY, color);
      insert_shape(polygon->tail, make_shape(POINT, new_point));
      cache_polygon_vertices(polygon);
    } else {
      Point * last_point = (Point *)polygon->tail->prev->data;
      last_point->x = mouseX;
      last_point->y = mouseY;
    }
  }
}

void exec_right_mouse_down(){
  if(polygon_available(tail)){
    Polygon * polygon = (Polygon *)tail->prev->data;
    polygon->finished = TRUE;
    polygon->dashed = FALSE;
    polygon->fill = fill;
  }
}

void exec_mouse_down_polygon() {
  if(!polygon_available(tail)) {
    printf("create a new polygon\n");
    Point* init_point = make_point(mouseX, mouseY, color);
    Polygon * polygon = make_polygon(init_point, color);
    insert_shape(tail, make_shape(POLY, polygon));
    cache_polygon_vertices(polygon);
  }else{
    Polygon * polygon = (Polygon *)tail->prev->data;
    Point* new_point = make_point(mouseX, mouseY, color);
    insert_shape(polygon->tail, make_shape(POINT, new_point));
    cache_polygon_vertices(polygon);
  }
}

void exec_mouse_down_circle() {
  exec_mouse_down_point();
  if(circle_available(tail)){
    Circle * circle = make_circle(
				  (Point *) tail->prev->prev->data,
				  (Point *) tail->prev->data,
				  color
				  );
    insert_shape(tail, make_shape(CIRCLE, circle));
  }
}

void exec_mouse_down_rect() {
  exec_mouse_down_point();
  if(rect_available(tail)) {
    Rect * rect = make_rect(
			    (Point *) tail->prev->prev->data,
			    (Point *) tail->prev->data,
			    color
			    );
    insert_shape(tail, make_shape(RECT, rect));
  }
}

void exec_mouse_down_triangle() {
  if(!triangle_available(tail)){
    exec_mouse_down_line();
  }else{
    if(tail->prev->type == LINE){
      remove_prev(tail);
    }
    exec_mouse_down_point();
    Point* v1 = (Point *)tail->prev->data;
    Point* v2 = (Point *)tail->prev->prev->data;
    Point* v3 = (Point *)tail->prev->prev->prev->data;
    Triangle* triangle = make_triangle(v1, v2, v3, color);
    insert_shape(tail, make_shape(TRIANGLE, triangle));
  }
}


void exec_mouse_down_line() {
  exec_mouse_down_point();
  if(line_available(tail)) {
    Line * line = make_line(
			    (Point *) tail->prev->data,
			    (Point *) tail->prev->prev->data,
			    color
			    );
    insert_shape(tail, make_shape(LINE, line));
  }
}

void exec_mouse_down_point(){
  Point * pressCoordinates = make_point(mouseX, mouseY, color);
  insert_shape(tail, make_shape(POINT,pressCoordinates));
}

void exec_mouse_up_line() {
  if(has_line(tail)){				    
    Line * line = (Line *)tail->prev->data;
    line->dashed = FALSE;
    line->fill = fill;
    line->color = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    memcpy(line->color, color, sizeof(GLfloat) * 3);
    ((Point *)(tail->prev->prev->data))->show = FALSE;
    ((Point *)(tail->prev->prev->prev->data))->show = FALSE;
  }
}

void exec_mouse_up_triangle() {
  if(has_triangle(tail)){
    ((Point *) tail->prev->prev->data)->show = FALSE;
    ((Point *) tail->prev->prev->prev->data)->show = FALSE;
    ((Point *) tail->prev->prev->prev->prev->data)->show = FALSE;
    Triangle * triangle = (Triangle *)tail->prev->data;
    triangle->dashed = FALSE;
    triangle->fill = fill;
    triangle->color = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    memcpy(triangle->color, color, sizeof(GLfloat) * 3);
  }
}

void exec_mouse_up_rect() {
  if(has_rect(tail)) {
    ((Point *) tail->prev->prev->prev->data)->show = FALSE;
    ((Point *) tail->prev->prev->data)->show = FALSE;
    Rect * rect = (Rect *) tail->prev->data;
    rect->dashed = FALSE;
    rect->fill = fill;
    rect->color = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    memcpy(rect->color, color, sizeof(GLfloat) * 3);
  }
}

void exec_mouse_up_circle() {
  if(has_circle(tail)) {
    ((Point *) tail->prev->prev->prev->data)->show = FALSE;
    ((Point *) tail->prev->prev->data)->show = FALSE;	
    Circle* circle = (Circle *) tail->prev->data;
    circle->dashed = FALSE;
    circle->fill = fill;
    circle->color = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    memcpy(circle->color, color, sizeof(GLfloat) * 3);
  }
}

void draw_point(Point* point){
  if(point->show){
    glColor3fv(point->color);
    glBegin(GL_POINTS);
    glVertex2f(point->x, point->y);
    glEnd();
  }
}

void draw_line(Line* line){
  if(line->dashed || !line->fill){
    glEnable(GL_LINE_STIPPLE);
  }else{
    glDisable(GL_LINE_STIPPLE);
  }
  glColor3fv(line->color);
  glBegin(GL_LINES);
  glVertex2f(line->v1->x, line->v1->y);
  glVertex2f(line->v2->x, line->v2->y);
  glEnd();
}

void draw_triangle(Triangle* triangle) {
  if(triangle->dashed) {
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  if(triangle->fill) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  glColor3fv(triangle->color);
  glBegin(GL_TRIANGLES);
  glVertex2f(triangle->v1->x, triangle->v1->y);
  glVertex2f(triangle->v2->x, triangle->v2->y);
  glVertex2f(triangle->v3->x, triangle->v3->y);
  glEnd();
}

 void draw_rect(Rect* rect) {
  if(rect->dashed) {
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  if(rect->fill) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  glColor3fv(rect->color);
  glBegin(GL_QUADS);
  glVertex2f(rect->v1->x, rect->v1->y);
  glVertex2f(rect->v2->x, rect->v2->y);
  glVertex2f(rect->v3->x, rect->v3->y);
  glVertex2f(rect->v4->x, rect->v4->y);
  glEnd();
 }

void draw_circle(Circle* circle) {
  if(circle->dashed) {
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glColor3fv(circle->color);
  if(circle->fill) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(circle->center->x, circle->center->y);
  } else {
    glBegin(GL_LINE_LOOP);
  }
  unsigned int i;
  for(i = 0; i <= NUM_SLICE; i++) {
    glVertex2f(circle->x_cache[i], circle->y_cache[i]);
  }
  glEnd();
}

void draw_polygon_not_finished(Polygon* polygon) {
  unsigned int i;
  for(i = 0; i < polygon->size; i++) {
    draw_point(polygon->cache[i]);
  }
  glBegin(GL_LINE_STRIP);
  for(i = 0; i < polygon->size; i++) {
    glVertex2f(polygon->cache[i]->x, polygon->cache[i]->y);
  }
  glEnd();
}

void draw_polygon_nofill(Polygon* polygon) {
  unsigned int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_LINE_LOOP);
  for(i = 0; i < polygon->size; i++) {
    glVertex2f(polygon->cache[i]->x, polygon->cache[i]->y);
  }
  glEnd();
}

void draw_polygon_fill(Polygon* polygon) {
  unsigned int i;
  glEnable(GL_STENCIL_TEST);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glStencilFunc(GL_ALWAYS, 0, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
  glStencilMask(1);
  glBegin(GL_POLYGON);
  for(i = 0; i < polygon->size; i++) {
    glVertex2f(polygon->cache[i]->x, polygon->cache[i]->y);
  }
  glEnd();


  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glStencilFunc(GL_EQUAL, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_POLYGON);
  for(i = 0; i < polygon->size; i++) {
    glVertex2f(polygon->cache[i]->x, polygon->cache[i]->y);
  }
  glEnd();
  glDisable(GL_STENCIL_TEST);
}

void draw_polygon(Polygon* polygon) {
  if(polygon->size == 1) {
    draw_point(polygon->cache[0]);
    return;
  }
  
  if(polygon->finished) {
    if(polygon->fill){
      draw_polygon_fill(polygon);
    } else {
      draw_polygon_nofill(polygon);
    }
  } else {
    draw_polygon_not_finished(polygon);
  }
  
}

void draw_shapes(Shape* shapes){
  Shape* current = shapes->next;
  while(current->type != -1) {
    draw_shape(current);
    current = current->next;
  }
}

void draw_shape(Shape* shape) {
  switch(shape->type){
  case POINT:
    draw_point((Point *) shape->data);
    break;
  case LINE:
    draw_line((Line *) shape->data);
    break;
  case TRIANGLE:
    draw_triangle((Triangle *) shape->data);
    break;
  case RECT:
    draw_rect((Rect *) shape->data);
    break;
  case CIRCLE:
    draw_circle((Circle *) shape->data);
    break;
  case POLY:
    draw_polygon((Polygon *) shape->data);
    break;
  default:
    break;
  }
}
void init() {
  // Set the clear color to white
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3fv(color);
  glPointSize(POINT_SIZE);
  glLineStipple(1, 0x3F07);
}




void reshape(GLFWwindow *w, int width, int height){  
  printf("Got a Reshape Event");
  printf(":\t width = %d height = %d\n", width, height);
  global_width = width;
  global_height = height;
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
  // Set the new viewport size
  global_width = width;
  global_height= height;
  glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch(key) {
    case 'f':
    case 'F':
      fill = (fill == TRUE) ? FALSE : TRUE;
    break;
    case 'r':
    case 'R':
      toggle_color(color, RED);
      break;
    case 'g':
    case 'G':
      toggle_color(color, GREEN);
      break;
    case 'b':
    case 'B':
      toggle_color(color, BLUE);
      break;
    case 'c':
    case 'C':
      free_all_shapes(head);
      break;
    case GLFW_KEY_F1:
      shape_mode = LINE;
      break;
    case GLFW_KEY_F2:
      shape_mode = TRIANGLE;
      break;
    case GLFW_KEY_F3:
      shape_mode = RECT;
      break;
    case GLFW_KEY_F4:
      shape_mode = CIRCLE;
      break;
    case GLFW_KEY_F5:
      shape_mode = POLY;
      break;
    default:
      break;
    }
  }

  switch(key) {
  case GLFW_KEY_ESCAPE :
  case 'q' :
  case 'Q':
    free_all_shapes(head);
    glfwSetWindowShouldClose(w, GL_TRUE);
    break;
  default:
    break;
  }
}

/********************************************************************
All shapes creation design:

Each shape are inserted into the linked list in the following way:

Line: insert two points in mouse_down or cursor event, depending on
how users draw the line.
As soon as there are two new points at the tail of the linked list,
i.e. a line can be draw in between the two points, draw the line.
In mouse up event, hide the two points and make dashed line solid
if in fill mode, or remain dashed if not.

Triangle: while the number of points available is less than 3, behave 
just like the line, execept always maintain in dashed mode. 
As soon as there is a dashed line and one point available, draw the
triangle.
In mouse up event, hide all three points, change dashed line to solid,
fill triangle if in fill mode, otherwise, do not fill.

Rect: Insert two points in mouse_down or cursor events. 
As soon as there are two new points available, draw the rect.
In mouse up event, hide the two diagon vertices and make dashed line
solid. Fill rect if in fill mode, otherwise, do not fill.

Circle: ditto^

Polygon: if not in fill mode, draw GL_LINE_STRIP if not finished, 
or GL_LINE_LOOP if finished.
If in fill mode, do stencil test, and only draw the areas that pass
the stencil test odd number of times, i.e. can be seen.

**********************************************************************/

void mouse(GLFWwindow* window, int button, int action, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT:
    mouseX = SCALE_X_AXIS(xpos, global_width);
    mouseY = SCALE_Y_AXIS(ypos, global_height);
    if(action == GLFW_PRESS) {
      pressed = TRUE;
      switch(shape_mode){
      case LINE:
	exec_mouse_down_line();
	break;
      case TRIANGLE:
	exec_mouse_down_triangle();
	break;
      case RECT:
	exec_mouse_down_rect();
	break;
      case CIRCLE:
	exec_mouse_down_circle();
	break;
      case POLY:
	exec_mouse_down_polygon();
      default:
	break;
      }
    } else {
      pressed = FALSE;
      switch(shape_mode) {
      case LINE:
	exec_mouse_up_line();
	break;
      case TRIANGLE:
	exec_mouse_up_triangle();
	break;
      case RECT:
	exec_mouse_up_rect();
	break;
      case CIRCLE:
	exec_mouse_up_circle();
	break;
      default:
	break;
      }
    }
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    if(shape_mode == POLY) {
      exec_right_mouse_down();
    }
    break;
  default :
    printf("none\n");
    break;
  }
}

void cursor(GLFWwindow* window, double xpos, double ypos) {
  mouseX = SCALE_X_AXIS(xpos, global_width);
  mouseY = SCALE_Y_AXIS(ypos, global_height);
  switch(shape_mode){
  case LINE:
    exec_cursor_line();
    break;
  case TRIANGLE:
    exec_cursor_triangle();
    break;
  case RECT:
    exec_cursor_rect();
    break;
  case CIRCLE:
    exec_cursor_circle();
    break;
  case POLY:
    exec_cursor_polygon();
    break;
  default:
    break;
  }
}

int main(int argc, char **argv) {

  head = init_linked_list();
  tail = head->next;

  GLFWwindow* window;
  
  // Initialize the library 
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Create a window and its OpenGL context 
  window = glfwCreateWindow(WIDTH, HEIGHT, "Homework 1", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 100, 100);
  // Make the window's context current 
  glfwMakeContextCurrent(window);

 // Assign reshape() to be the function called whenever 
 // a reshape event occurs
  glfwSetWindowSizeCallback(window, reshape);

  // Assign keyboard() to be the function called whenever
  // a key is pressed or released
  glfwSetKeyCallback(window, keyboard);

  // Assign mouse() to be the function called whenever
  // a mouse button is pressed or released
  glfwSetMouseButtonCallback(window, mouse);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  // Assign cursor() to be the function called whenever
  // a cursor moves
  glfwSetCursorPosCallback(window, cursor);

  init();
  // Loop until the user closes the window 
  while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    draw_shapes(head);
    
    // Swap front and back buffers 
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  free_one_shape(head);
  free_one_shape(tail);
  exit(EXIT_SUCCESS);
}
