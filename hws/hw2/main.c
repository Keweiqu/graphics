#include "universe.h"
#include "global.h"

extern double now, last;
extern enum Game_status game_status;
extern Self self;
extern Legion legion;
extern Fort forts[NUM_FORT];
int breakpoint = FALSE;

void init() {
  glPointSize(POINT_SIZE);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(10);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(key){
  case GLFW_KEY_SPACE:
    if(action == GLFW_PRESS)
      self_shoot_bullet(self.sin, self.cos);
    break;
  case GLFW_KEY_LEFT:
    if(action == GLFW_PRESS) {
      self.move = TRUE;
      self.duration = INIT_DURATION;
      self.direction = LEFT;
    } else if(action == GLFW_RELEASE) {
      if(self.direction == LEFT) {
	self.move = FALSE;
      }
    }
   
    break;
  case GLFW_KEY_RIGHT:
    if(action == GLFW_PRESS) {
      self.move = TRUE;
      self.duration = INIT_DURATION;
      self.direction = RIGHT;
    } else if(action == GLFW_RELEASE){
      if(self.direction == RIGHT) {
	self.move = FALSE;
      }
    }
    break;
  case GLFW_KEY_COMMA:
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
      self.angle -= 5;
      self.sin = sin(self.angle * RADIUS_FACTOR);
      self.cos = cos(self.angle * RADIUS_FACTOR);
    }
    break;
  case GLFW_KEY_PERIOD:
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
      self.angle += 5;
      self.sin = sin(self.angle * RADIUS_FACTOR);
      self.cos = cos(self.angle * RADIUS_FACTOR);
    }
    break;
  case 'd':
  case 'D':
    if(!breakpoint) {
      breakpoint = TRUE;
    }else {
      advance_universe(&self, &legion);
    }
    
    break;
  case 'r':
  case 'R':
    breakpoint = FALSE;
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
  last = 0;
  game_status = UNDEFINED;
  create_forts(forts);

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
    if(game_status == WIN) {
      draw_win();
    }else if(game_status == LOSE) {
      draw_lose();
    }else{
      glClear(GL_COLOR_BUFFER_BIT);
      draw_self(&self);
      draw_life(&self);
      draw_score(&self);
      draw_forts(forts);
      draw_self_bullets(&self);
      draw_legion(&legion);
      check_collision_self(&legion);
      if(breakpoint) {
      }else{ 
	update_universe(&self, &legion);
      }
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}



void draw_legion(Legion* legion) {
  glPushMatrix();
  glTranslatef(legion->x_trans, legion->y_trans, 0);
 

  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      draw_alien(&legion->army[i][j]);
    }
  }
  draw_legion_bullets(legion);
  check_collision_legion(legion, &self, forts);
  glPopMatrix();
}

void draw_alien(Alien *a) {
  if(a->status == ALIVE) {
    glPushMatrix();
    glTranslatef(a->x_coord, a->y_coord, 0);
    glRotatef(a->flip_base+a->angle, 0, 1, 0);
    glTranslatef(-a->x_coord, -a->y_coord, 0);
    draw_alien_helper(a);
    a->angle = (a->angle + 2) % 360;
    glPopMatrix();
  } else if(a->status == DYING) {
    glPushMatrix();
    glTranslatef(a->x_coord, a->y_coord, 0);
    glScalef(a->scale, a->scale, a->scale);
    glRotatef(a->angle, 0, 0, 1);
    glTranslatef(-a->x_coord, -a->y_coord, 0);
    draw_alien_helper(a);
    glPopMatrix();
  }
}


void draw_self(Self *s) {
  glColor3f(0.0, 1.0, 0.0);
  glPushMatrix();
  glTranslatef(s->x_trans, s->y_trans, 0);
  if(s->dying) {
    glTranslatef(s->x_coord, s->y_coord, 0);
    glScalef(s->scale, s->scale, s->scale);
    glTranslatef(-s->x_coord, -s->y_coord, 0);
  }
  draw_elements(s->elements, 9);
  glBegin(GL_LINES);
  glVertex2f(s->x_coord, s->y_coord);
  glVertex2f(s->x_coord + s->sin * s->element_width * 2, s->y_coord + s->cos* s->element_width * 2);
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
  }
}

void draw_self_bullets(Self *self) {
  unsigned int i;
  for(i = 0; i < FIRE_LOAD; i++) {
    Bullet* this_b = &self->fire[i];
    draw_bullet(this_b);
  }
}

void draw_alien_helper(Alien* a) {
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  glVertex2f(a->x_coord - HALF_WIDTH, a->y_coord - HALF_WIDTH);
  glVertex2f(a->x_coord - HALF_WIDTH, a->y_coord + HALF_WIDTH);
  glVertex2f(a->x_coord + HALF_WIDTH, a->y_coord + HALF_WIDTH);
  glVertex2f(a->x_coord + HALF_WIDTH, a->y_coord - HALF_WIDTH);
  glEnd(); 
}

void draw_elements(Coord *elements, unsigned int size){
  unsigned int i;
  for(i = 0; i < size; i++) {
    Coord c = elements[i];
    if(c.draw) {
      glBegin(GL_QUADS);
      glVertex2f(c.x_coord - c.width, c.y_coord - c.width);
      glVertex2f(c.x_coord - c.width, c.y_coord + c.width);
      glVertex2f(c.x_coord + c.width, c.y_coord + c.width);
      glVertex2f(c.x_coord + c.width, c.y_coord - c.width);
      glEnd();
    }
  }
}
