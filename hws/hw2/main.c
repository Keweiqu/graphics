#include "main.h"


GLfloat x_trans = 0;
GLfloat y_trans = 0;
GLfloat left_bound = (1 - (5 * SPACING + HALF_WIDTH)) * -1 + 0.1;
GLfloat right_bound = 1 - (5 * SPACING + HALF_WIDTH) - 0.1;
int direction = LEFT;
double alien_timestamp = TIMESTAMP;
double now, last = 0, delay = 0;
int advanced = FALSE;
Self self;
Alien alien_array[N_ROW][N_COL];
int duration;

int shot_alien_row = -1, shot_alien_col = -1;

void init() {
  // Set the clear color to white
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(key){
  case GLFW_KEY_SPACE:
    if(!self.shooting){
      self_shoot_bullet();
    }
    break;
  case GLFW_KEY_LEFT:
    duration = DURATION;
    self.direction = LEFT;
    break;
  case GLFW_KEY_RIGHT:
    duration = DURATION;
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
  init_alien_array(alien_array);
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
    glPushMatrix();
    glTranslatef(x_trans, y_trans, 0);
    now = glfwGetTime();
    if(now - last > alien_timestamp) {
      update_trans(&x_trans, &y_trans);
      check_collision_self_bullet(self.b, alien_array, &shot_alien_row, &shot_alien_col, &delay);
      last = now;
      printf("translate x %f, y %f\n", x_trans, y_trans);
    }
    draw_alien_array(alien_array);
    glPopMatrix();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init_alien_array(Alien (*alien_array)[N_COL]) {
  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      alien_array[i][j] = create_alien(i, j);
    }
  }
} 

void draw_alien_array(Alien (*alien_array)[N_COL]) {
  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      draw_alien(alien_array[i][j]);
    }
  }
}

void draw_alien(Alien a) {
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  glVertex2f(a.x_coord - HALF_WIDTH, a.y_coord - HALF_WIDTH);
  glVertex2f(a.x_coord - HALF_WIDTH, a.y_coord + HALF_WIDTH);
  glVertex2f(a.x_coord + HALF_WIDTH, a.y_coord + HALF_WIDTH);
  glVertex2f(a.x_coord + HALF_WIDTH, a.y_coord - HALF_WIDTH);
  glEnd();
}
/*
Alien is represented as a square, the coordinates that locate this square
is the center of the square.
 */
Alien create_alien(int row, int col) {
  Alien a;
  a.alive = TRUE;
  a.x_coord = (col - 5) * SPACING;
  a.y_coord = 0.9 - row * SPACING;
  return a;
}

/*
 Self is represented as a triangle, the coordinates that locate this triangle,
is the bottom center of the line.
 */
Self create_self() {
  Self s;
  s.lives = 3;
  s.x_coord = 0.7;
  s.y_coord = -0.98;
  s.shooting = FALSE;
  s.b = create_bullet(UP);
  return s;
}

void draw_self(Self s) {
  glColor3f(0.0, 1.0, 0.0);
  move_self();
  if(self.shooting) {
    draw_bullet(&self.b);
    if(self.b.y_coord >= 1 || self.b.y_coord <= -1){
      self.shooting = FALSE;
    }
  }
  glBegin(GL_TRIANGLES);
  glVertex2f(s.x_coord - 0.07, s.y_coord);
  glVertex2f(s.x_coord + 0.07, s.y_coord);
  glVertex2f(s.x_coord, s.y_coord + 0.06);
  glEnd();
}

void update_trans(GLfloat* x_trans, GLfloat* y_trans) {
  if((*x_trans <= left_bound || *x_trans >= right_bound) && !advanced) {
    direction = (direction == LEFT) ? RIGHT : LEFT;
    *y_trans -= DELTA * 0.8;
    alien_timestamp *= 0.9;
    advanced = TRUE;
  } else {
    *x_trans += DELTA * 0.8 * direction;
    advanced = FALSE;
  }
}

void move_self() {
  if(duration > 0) {
    self.x_coord += direction * 0.01;
    duration--;
  }
}

Bullet create_bullet(int direction) {
  Bullet b;
  b.direction = direction;
  return b;
}

void self_shoot_bullet() {
    self.b.x_coord = self.x_coord;
    self.b.y_coord = self.y_coord;
    self.shooting = TRUE;
}

void draw_bullet(Bullet *b) {
    glBegin(GL_QUADS);
    glVertex2f(b->x_coord - 0.005, b->y_coord - 0.025 * b->direction);
    glVertex2f(b->x_coord + 0.005, b->y_coord - 0.025 * b->direction);
    glVertex2f(b->x_coord + 0.005, b->y_coord + 0.025 * b->direction);
    glVertex2f(b->x_coord - 0.005, b->y_coord + 0.025 * b->direction);
    glEnd();
    b->y_coord += BULLET_SPEED * b->direction;
}

//pass as param -- row col of the alien about to go die, secs in which this is about to happen.
void check_collision_self_bullet(
				 Bullet b,
				 Alien (*alien_array)[N_COL],
				 int* candidate_row,
				 int* candidate_col,
				 double* delay
				 ) {
  printf("check collision\n");
  int col = 0, row = N_ROW;
  GLfloat b_x_trans = b.x_coord - x_trans;
  GLfloat b_y_trans = b.y_coord - y_trans;
  while(col < N_COL) {
    if(fabs(b_x_trans - alien_array[0][col].x_coord) < 0.005 + HALF_WIDTH)
      break;
    col++;
  }

  if (col == N_COL){
    *candidate_row = -1;
    *candidate_col = -1;
    return;
  }

  while(row >= 0) {
    if(alien_array[row][0].y_coord - b_y_trans < 0.025 + HALF_WIDTH)
      break;
    row--;
  }

  //if at the start of alien_timestamp, self.b shoot something.
  if(row >= 0) {
    if(alien_array[row][col].alive == TRUE) {
      *candidate_row = row;
      *candidate_col = col;
      *delay = 0;
      printf("shot alien row %d col %d, with delay %f\n", *candidate_row, *candidate_col, *delay);
      return;
    }
  }

  //calculate self.b's trajectory, and see if will shoot something during the timestamp duration
  row = N_ROW;
  GLfloat traject_half_height = alien_timestamp * BULLET_SPEED / 2;
  GLfloat traject_center_y = b_y_trans + 0.025 + traject_half_height;
  while(row >= 0) {
    if(alien_array[row][0].y_coord - traject_center_y < traject_half_height + HALF_WIDTH &&
       alien_array[row][col].alive) {
      break;
    }
    row--;
  }
  if(row < 0) {
    *candidate_row = -1;
    *candidate_col = -1;
    return;
  } else {
    *candidate_row = row;
    *candidate_col = col;
    *delay = (alien_array[row][0].y_coord - HALF_WIDTH - (b_y_trans + 0.025)) / BULLET_SPEED;
     printf("shot alien row %d col %d, with delay %f\n", *candidate_row, *candidate_col, *delay);
    return;
  }
}
