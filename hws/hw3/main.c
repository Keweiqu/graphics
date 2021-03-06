#include "boid.h"
#include "linkedlist.h"

void init() {
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPointSize(10);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  perspective(40, 1, 0.000001, 10);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);

  isPaused = 0;
  calc_checkerboard_vertices(SIDES, 2 * WORLD_HALF_WIDTH);
  calc_checkerboard_indices(SIDES);
  calc_checkerboard_colors(SIDES);
}

void init_boids() {
  count = BOID_COUNT;
  head = create_linkedlist();
  tail = head->next;
  for (int i = 0; i < BOID_COUNT; i++) {
    Boid *b = init_boid(i);
    Node *new_node = create_node(b, VAL);
    append(new_node, tail);    
  }
}


void draw_boid(Boid* b) {
  gsl_vector *location = b->location;
  glPushMatrix();
  glScalef(world_scale[0], world_scale[1], world_scale[2]);
  glTranslatef(gsl_vector_get(location, 0), gsl_vector_get(location, 1), gsl_vector_get(location, 2));
  glRotatef(b->angle, 0, 0, 1);
  glRotatef(b->z_angle, 0, 1, 0);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
  glPopMatrix();
}

void draw_left_wing(Boid* b) {
  gsl_vector* location = b->location;
  glPushMatrix();
  glScalef(world_scale[0], world_scale[1], world_scale[2]);
  glTranslatef(gsl_vector_get(location, 0), gsl_vector_get(location, 1), gsl_vector_get(location, 2));
  glRotatef(b->angle, 0, 0, 1);
  glRotatef(b->z_angle, 1, 0, 0);
  glRotatef(-b->wing_angle, 0, 1, 0);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, left_indices);
  glPopMatrix();
}

void draw_right_wing(Boid* b) {
  gsl_vector* location = b->location;
  glPushMatrix();
  glScalef(world_scale[0], world_scale[1], world_scale[2]);
  glTranslatef(gsl_vector_get(location, 0), gsl_vector_get(location, 1), gsl_vector_get(location, 2));
  glRotatef(b->angle, 0, 0, 1);
  glRotatef(b->z_angle, 1, 0, 0);
  glRotatef(b->wing_angle, 0, 1, 0);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, right_indices);
  glPopMatrix();
}

void draw_shadows() {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, shadow_vertices);
  glColorPointer(3, GL_FLOAT, 0, shadow_colors);
  Node* current;
  current = head->next;
  while (current->type != HEAD_TAIL) {
    draw_shadow(current->data);
    current = current->next;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void draw_shadow(Boid* b) {
  gsl_vector *location = b->location;
  glPushMatrix();
  glScalef(world_scale[0], world_scale[1], world_scale[2]);
  glTranslatef(gsl_vector_get(location, 0), gsl_vector_get(location, 1), gsl_vector_get(location, 2)*0.60);
  glRotatef(b->angle, 0,0,1);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
  glPopMatrix();
}


void draw_boids() {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
  glColorPointer(3, GL_FLOAT, 0, boid_colors);
  Node* current;
  current = head->next;
  while (current->type != HEAD_TAIL) {
    draw_left_wing(current->data);
    draw_right_wing(current->data);
    current = current->next;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void update_boids() {
  Node* current = head->next;
  while (current->type != HEAD_TAIL) {
    Boid* this_b = (Boid *)current->data;
    Boid ** knn = n_neighbours(this_b, cache, count, 5);
    update_boid(this_b, knn, g);
    free(knn);
    current = current->next;
  }
}

void update_boid(Boid* b, Boid** neighbors, Goal g) {
  gsl_vector_add(b->location, b->velocity);
  b->angle = get_xy_angle(b);
  b->z_angle = get_z_angle(b);
  gsl_vector* s = separation(b, neighbors);
  gsl_vector* c = cohesion(b, neighbors);
  gsl_vector* a = alignment(b, neighbors);
  gsl_vector* g_s = goal_seeking(g, b);
  gsl_vector_add(g_s, s);
  gsl_vector_add(g_s, c);
  gsl_vector_add(g_s, a);
  gsl_vector_scale(b->velocity, 0.99);
  gsl_vector_scale(g_s, 4);
  gsl_vector_add(b->velocity, g_s);

  double speed = get_vector_length(b->velocity);
  if(speed > 30) {
    normalize_vector(b->velocity, VECTOR_LENGTH);
    gsl_vector_scale(b->velocity, 30);
  }
  gsl_vector_free(s);
  gsl_vector_free(c);
  gsl_vector_free(a);
  gsl_vector_free(g_s);
  update_boid_wing(b);
}

void update_boid_wing(Boid* b) {
  b->wing_angle += WING_DELTA * b->wing_direction;
  if(fabs(b->wing_angle) > MAX_WING_ANGLE) {
    b->wing_direction *= -1;
  }
}

void draw_checkerboard() {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glPushMatrix();
  glScalef(world_scale[0], world_scale[1], world_scale[2]);
  glVertexPointer(3, GL_FLOAT, 0, board_vertices);
  glColorPointer(3, GL_FLOAT, 0, board_colors);
  glDrawElements(GL_QUADS, SIDES*SIDES*4, GL_UNSIGNED_SHORT, board_indices);
  glPopMatrix();
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

/*
 * calculate all vertices coordinates for checkerboard.
 * @param n n * n checkerboard
 * @param len side-length of the checkboard
 */
void calc_checkerboard_vertices(int n, GLfloat len) {
  GLfloat lx = -len / 2, ly = len / 2;
  GLfloat num_of_points = pow(n + 1, 2); 
  for (int i = 0; i < num_of_points; i++) {
    GLfloat xpos = lx + (i % (n + 1)) * (len / n);
    GLfloat ypos = ly - (i / (n + 1)) * (len / n);
    board_vertices[i][0] = xpos;
    board_vertices[i][1] = ypos;
    board_vertices[i][2] = 0; //z
  }
}

/*
 * Calculate the indices for each square.
 */
void calc_checkerboard_indices(int n) {
  int i;
  for (i = 0; i < pow(n, 2); i++) {
    board_indices[4 * i] = (i / n) * (n + 1) + i % n + 1;    
    board_indices[4 * i + 1] = (i / n) * (n + 1) + i % n;
    board_indices[4 * i + 2] = (i / n) * (n + 1) + i % n + n + 1;
    board_indices[4 * i + 3] = (i / n) * (n + 1) + i % n + n + 2;
  }
}

void calc_checkerboard_colors(int n) {
  if (n % 2 == 0) {
    for (int i = 0; i < (n+1)*(n+1); i++) {
      if (i % 2) {
	board_colors[i][0] = 1.0;
	board_colors[i][1] = 1.0;
	board_colors[i][2] = 1.0;
      } else {
	board_colors[i][0] = 0.0;
	board_colors[i][1] = 0.0;
	board_colors[i][2] = 0.0;
      }
    }
  } else {
    printf("yay");
    for (int i = 0; i < (n+1)*(n+1); i++) {
      if ((i % 2 == 1 && (i / (n+1)) % 2 == 1) || (i % 2 == 0 && (i / (n+1)) % 2 == 0)) {
	printf("white vertex: %d\n", i);
	board_colors[i][0] = 1.0;
	board_colors[i][1] = 1.0;
	board_colors[i][2] = 1.0;
      } else {
	board_colors[i][0] = 0.0;
	board_colors[i][1] = 0.0;
	board_colors[i][2] = 0.0;
      }
    }
  }
}

void add_boid() {
  Boid *b = init_boid(count+1);
  Node *new_node = create_node(b, VAL);
  append(new_node, tail);
  count++;
}

void keyboard(GLFWwindow *w, int key, int scancode,  int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
    case GLFW_KEY_EQUAL:
      add_boid();
      free(cache);
      cache = cache_linkedlist(head);
      break;
    case GLFW_KEY_BACKSPACE:
      if (count > BOID_COUNT) {
	delete_boid();
	free(cache);
	cache = cache_linkedlist(head);
	count--;
      }       
      break;
    case GLFW_KEY_C:
      v_mode = CENTER;
      view = &center_view;
      break;
    case GLFW_KEY_T:
      v_mode = TRAILING;
      view = &trailing_view;
      break;
    case GLFW_KEY_S:
      v_mode = SIDE;
      view = &side_view;
      break;
    case GLFW_KEY_P:
      isPaused = isPaused ? 0 : 1;
      break;
    case GLFW_KEY_D:
      isPaused = 1;
      update_goal(&g);
      update_boids();
      printf("=====================DEBUGING INFO====================\n");
      gsl_vector* boid_center = calc_middleway(cache, count, g);
      printf("Center of the boids is: \n");
      print_vector(boid_center);
      gsl_vector_free(boid_center);
      printf("Location of goal: \n");
      print_vector(g.trans);
      break;
    case GLFW_KEY_Q:
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(w, TRUE);
      break;
    }
  }
  if(action == GLFW_REPEAT || action == GLFW_PRESS) {
    switch(key) {
    case GLFW_KEY_UP:
      g.h_move = GOAL_V_STEP;
      gsl_vector_set(g.direction, 2, UP);
      break;
    case GLFW_KEY_DOWN:
      g.h_move = GOAL_V_STEP;
      gsl_vector_set(g.direction, 2, DOWN);
      break;
    case GLFW_KEY_LEFT:
      g.x_move = GOAL_H_STEP;
      gsl_vector_set(g.direction, 0, LEFT);
      break;
    case GLFW_KEY_RIGHT:
      g.x_move = GOAL_H_STEP;
      gsl_vector_set(g.direction, 0, RIGHT);
      break;
    case GLFW_KEY_V:
      if(g.speed < 100) {
	g.speed += 5;
      }
      break;
    case GLFW_KEY_B:
      if(g.speed > 20) {
	g.speed -= 5;
      }
      break;
    }
  }
  
}

void cursor(GLFWwindow* w, double xpos, double ypos) {
  x_pos = xpos;
  y_pos = ypos;
}

void lookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ) {
  GLdouble m[16];
  gsl_vector* forward = gsl_vector_alloc(4);
  gsl_vector* up = gsl_vector_alloc(4);
  gsl_vector* side = gsl_vector_alloc(4);
  
  gsl_vector_set_zero(forward);
  gsl_vector_set_zero(up);
  gsl_vector_set_zero(side);
  gsl_vector_set(forward, 0, centerX - eyeX);
  gsl_vector_set(forward, 1, centerY - eyeY);
  gsl_vector_set(forward, 2, centerZ - eyeZ);
  normalize_vector(forward, 4);
  gsl_vector_set(up, 0, upX);
  gsl_vector_set(up, 1, upY);
  gsl_vector_set(up, 2, upZ); 
  crossProduct(forward, up, side);
  normalize_vector(side, 4);
  crossProduct(side, forward, up);
  gsl_vector_scale(forward, -1);
  const double* f = gsl_vector_const_ptr(forward, 0);
  const double* u = gsl_vector_const_ptr(up, 0);
  const double* s = gsl_vector_const_ptr(side, 0);

  m[0] = s[0];
  m[4] = s[1];
  m[8] = s[2];
  m[12] = 0.0;
  
  m[1] = u[0];
  m[5] = u[1];
  m[9] = u[2];
  m[13] = 0.0;

  m[2] = f[0];
  m[6] = f[1];
  m[10] = f[2];
  m[14] = 0.0;

  m[3] = m[7] = m[11] = 0.0;
  m[15] = 1.0;

  glMultMatrixd((const GLdouble *)m);
  glTranslated(-eyeX, -eyeY, -eyeZ);

  gsl_vector_free(forward);
  gsl_vector_free(up);
  gsl_vector_free(side);
}

void testLookAt() {
  lookAt(-3, -1, -2, 0, 3, 1, 2, 1, 0);  
}

void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
  GLdouble m[16] = {0};
  GLdouble fovy_in_radians = fovy * M_PI / 180;
  m[0] = (1 / tan(fovy_in_radians / 2)) / aspect;
  m[5] = 1 / tan(fovy_in_radians / 2);
  m[10] = (zFar + zNear) / (zNear - zFar);
  m[11] = -1;
  m[14] = 2 * (zNear * zFar) / (zNear - zFar);
  glLoadIdentity();
  glMultMatrixd((const GLdouble *)m);
}

void crossProduct(gsl_vector* u, gsl_vector* v, gsl_vector* r) {
  double p1 = gsl_vector_get(u, 1) * gsl_vector_get(v, 2) - gsl_vector_get(u, 2) * gsl_vector_get(v, 1);
  double p2 = gsl_vector_get(u, 2) * gsl_vector_get(v, 0) - gsl_vector_get(u, 0) * gsl_vector_get(v, 2);
  double p3 = gsl_vector_get(u, 0) * gsl_vector_get(v, 1) - gsl_vector_get(u, 1) * gsl_vector_get(v, 0);
  gsl_vector_set(r, 0, p1);
  gsl_vector_set(r, 1, p2);
  gsl_vector_set(r, 2, p3);
}

void testCP() {
  gsl_vector* u = gsl_vector_alloc(3);
  gsl_vector* v = gsl_vector_alloc(3);
  gsl_vector* r = gsl_vector_alloc(3);
  gsl_vector_set(u, 0, 1);
  gsl_vector_set(u, 1, 1);
  gsl_vector_set(u, 2, 1);
  gsl_vector_set(v, 0, 4);
  gsl_vector_set(v, 1, 5);
  gsl_vector_set(v, 2, 6);
  crossProduct(u, v, r);
  printf("x: %f, y: %f, z: %f\n", gsl_vector_get(r, 0), gsl_vector_get(r, 1), gsl_vector_get(r, 2));
}

void reshape(GLFWwindow *w, int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  perspective(40, 1, 0.000001, 10);
  glMatrixMode(GL_MODELVIEW); 
}

void framebuffer_resize(GLFWwindow *w, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char **argv) {
  v_mode = CENTER;
  init_boids();
  cache = cache_linkedlist(head);
  g = init_goal();
  ave_multiplier = 0.2;
  init_views();

  GLFWwindow *window;
  
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  
  window = glfwCreateWindow(1000, 1000, "Flocking Boids", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);
  glfwSetKeyCallback(window, keyboard);
  glfwSetCursorPosCallback(window, cursor);

  init();
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    draw_boids();
    draw_shadows();
    draw_checkerboard();
    draw_goal(g);
    if (!isPaused) {
      update_boids();
      update_goal(&g);
    }
    
    glLoadIdentity();
    camera_look();

    update_view();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  free_views();
  free(cache);
  free_boids();
  free(head);
  free(tail);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
