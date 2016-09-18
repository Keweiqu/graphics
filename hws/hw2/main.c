#include "universe.h"
#include "global.h"

double now, last = 0, delay = 0;
extern Self self;
extern Legion legion;

int shot_alien_row = -1, shot_alien_col = -1;

void init() {
  // Set the clear color to white
  glPointSize(POINT_SIZE);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(key){
  case GLFW_KEY_SPACE:
    if(action == GLFW_PRESS)
      self_shoot_bullet();
    break;
  case GLFW_KEY_LEFT:
    self.duration = INIT_DURATION;
    self.direction = LEFT;
    break;
  case GLFW_KEY_RIGHT:
    self.duration = INIT_DURATION;
    self.direction = RIGHT;
    break;
  case 'q':
  case 'Q':
    glfwSetWindowShouldClose(w, TRUE);
  default:
    break; 
  }
}

int main() {
  create_legion(&legion);
  self = create_self();
  GLFWwindow* window;
  if (!glfwInit())
    exit(EXIT_FAILURE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Homework 2", NULL, NULL);
  if(!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 100, 100);
  // Make the window's context current 
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, keyboard);

  init();
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_self(self);
    draw_self_bullets();
    draw_legion(&legion);
    check_collision_self(&legion);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}



void draw_legion(Legion* legion) {
  glPushMatrix();
  glTranslatef(legion->x_trans, legion->y_trans, 0);
  now = glfwGetTime();
  if(now - last > legion->march_interval) {
    //check_collision_self_bullet(self.b, legion);
    update_trans(legion);
    last = now;
    printf("translate x %f, y %f\n", legion->x_trans, legion->y_trans);
  }

  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      draw_alien(legion->army[i][j]);
    }
  }
  glPopMatrix();
}

void draw_alien(Alien a) {
  if(a.status == ALIVE) {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(a.x_coord - HALF_WIDTH, a.y_coord - HALF_WIDTH);
    glVertex2f(a.x_coord - HALF_WIDTH, a.y_coord + HALF_WIDTH);
    glVertex2f(a.x_coord + HALF_WIDTH, a.y_coord + HALF_WIDTH);
    glVertex2f(a.x_coord + HALF_WIDTH, a.y_coord - HALF_WIDTH);
    glEnd();
  }
}


void draw_self(Self s) {
  glColor3f(0.0, 1.0, 0.0);
  move_self();
  glPushMatrix();
  glTranslatef(self.x_trans, self.y_trans, 0);
  glBegin(GL_TRIANGLES);
  glVertex2f(s.x_coord - 0.07, s.y_coord);
  glVertex2f(s.x_coord + 0.07, s.y_coord);
  glVertex2f(s.x_coord, s.y_coord + 0.06);
  glEnd();
  glPopMatrix();
}




void draw_bullet(Bullet *b) {
  if(b->fired) {
    glPushMatrix();
    glTranslatef(b->x_trans, b->y_trans, 0);
    glBegin(GL_QUADS);
    glVertex2f(b->x_coord - 0.005, b->y_coord - 0.005);
    glVertex2f(b->x_coord - 0.005, b->y_coord + 0.005);
    glVertex2f(b->x_coord + 0.005, b->y_coord + 0.005);
    glVertex2f(b->x_coord + 0.005, b->y_coord - 0.005);
    glEnd();
    glPopMatrix();
    b->y_trans += BULLET_SPEED * b->direction;
  }
  if(b->y_coord + b->y_trans >= 1) {
    b->fired = FALSE;
  }
}

void draw_self_bullets() {
  unsigned int i;
  for(i = 0; i < FIRE_LOAD; i++) {
    Bullet* this_b = &self.fire[i];
    draw_bullet(this_b);
  }
}

