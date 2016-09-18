#include "universe.h"
#include "global.h"

extern Self self;
extern Legion legion;

/*
 * Alien is represented as a square, the coordinates that locate this square
 * is the center of the square.
 */
Alien create_alien(int row, int col) {
  Alien a;
  a.status = ALIVE;
  a.x_coord = (col - 5) * SPACING;
  a.y_coord = 0.9 - row * SPACING;
  return a;
}

void create_legion(Legion* legion) {
  legion->advanced = FALSE;
  legion->direction = LEFT;
  legion->march_interval = TIMESTAMP;
  legion->left_bound = (1 - (5 * SPACING + HALF_WIDTH)) * -1 + 0.1;
  legion->right_bound = 1 - (5 * SPACING + HALF_WIDTH) - 0.1;
  legion->x_trans = 0;
  legion->y_trans = 0;
  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      legion->army[i][j] = create_alien(i, j);
    }
  }

} 

/*
 * Self is represented as a triangle, the coordinates that locate this triangle,
 * is the bottom center of the line.
 */
Self create_self() {
  Self s;
  s.lives = 3;
  s.x_coord = 0.7;
  s.y_coord = -0.98;
  s.x_trans = 0;
  s.y_trans = 0;
  s.shooting = FALSE;
  s.b = create_bullet(UP);
  s.duration = 0;
  return s;
}

void update_trans(Legion * legion) {
  if(
     (legion->x_trans <= legion->left_bound ||
      legion->x_trans >= legion->right_bound) &&
     !legion->advanced) {
    legion->direction = ~(legion->direction) + 1;
    legion->y_trans -= DELTA * 0.8;
    legion->march_interval *= 0.9;
    legion->advanced = TRUE;
  } else {
    legion->x_trans += DELTA * 0.8 * legion->direction;
    legion->advanced = FALSE;
  }
}



void move_self() {
  if(self.duration > 0) {
    GLfloat temp = self.x_trans + self.direction * 0.02;
    if( self.x_coord + temp > -1 + 0.07 && self.x_coord + temp < 1 - 0.07) {
      self.x_trans = temp;
    }
    self.duration--;
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
    self.b.x_trans = self.x_trans;
    self.b.y_trans = self.y_trans;
    self.shooting = TRUE;
}

void check_collision_self_bullet(Bullet b, Legion* legion) {
  int col = 0, row = N_ROW;
  GLfloat b_x = b.x_coord + b.x_trans;
  GLfloat b_y = b.y_coord + b.y_trans;
  GLfloat top = legion->army[0][0].y_coord + HALF_WIDTH + legion->y_trans;
  GLfloat bottom = legion->army[N_ROW -1][0].y_coord - HALF_WIDTH + legion->y_trans;
  GLfloat left = legion->army[0][0].x_coord - HALF_WIDTH + legion->x_trans;
  GLfloat right = legion->army[0][N_COL - 1].x_coord + HALF_WIDTH + legion->x_trans;
  if(b_x < left || b_x > right ||
     b_y > top || b_y < bottom){
    return;
  }

  while(col < N_COL) {
    GLfloat x = legion->army[0][col].x_coord + legion->x_trans;
    if(fabs(b_x - x) < HALF_WIDTH + 0.005)
      break;
    col++;
  }

  if (col == N_COL){
    return;
  }

  while(row >= 0) {
    GLfloat y = legion->army[row][0].y_coord + legion->y_trans;
    if(fabs(b_y - y) < HALF_WIDTH + 0.005)
      break;
    row--;
  }

  if(row >= 0) {
    if(legion->army[row][col].status == ALIVE) {
      legion->army[row][col].status = DYING;
      self.shooting = FALSE;
      printf("shot alien row %d col %d\n", row, col);
      return;
    }
  }
}
