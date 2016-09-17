#include "universe.h"
#include "global.h"

extern Self self;
extern Legion legion;

/*
Alien is represented as a square, the coordinates that locate this square
is the center of the square.
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
 Self is represented as a triangle, the coordinates that locate this triangle,
is the bottom center of the line.
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

//pass as param -- row col of the alien about to go die, secs in which this is about to happen.
void check_collision_self_bullet(Bullet b, Legion* legion) {
  printf("check collision\n");
  int col = 0, row = N_ROW;
  GLfloat b_x_trans = b.x_coord - legion->x_trans;
  GLfloat b_y_trans = b.y_coord - legion->y_trans;
  while(col < N_COL) {
    if(fabs(b_x_trans - legion->army[0][col].x_coord) < 0.005 + HALF_WIDTH)
      break;
    col++;
  }

  if (col == N_COL){
    return;
  }

  while(row >= 0) {
    if(legion->army[row][0].y_coord - b_y_trans < 0.005 + HALF_WIDTH)
      break;
    row--;
  }

  //if at the start of alien_timestamp, self.b shoot something.
  if(row >= 0) {
    if(legion->army[row][col].status == ALIVE) {
      legion->army[row][col].status = DYING;
      printf("shot alien row %d col %d\n", row, col);
      return;
    }
  }
}
