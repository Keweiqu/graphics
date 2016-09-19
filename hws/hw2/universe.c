#include "universe.h"
#include "global.h"

extern Self self;
extern Legion legion;
extern time_t t;

/*
 * Alien is represented as a square, the coordinates that locate this square
 * is the center of the square.
 */
Alien create_alien(int row, int col) {
  Alien a;
  a.status = ALIVE;
  a.b = create_bullet(DOWN);
  a.angle = 0;
  a.scale = 1;
  a.x_coord = (col - 5) * SPACING;
  a.y_coord = 0.9 - row * SPACING;
  return a;
}

void create_legion(Legion* legion) {
  legion->advanced = FALSE;
  legion->direction = LEFT;
  legion->march_interval = TIMESTAMP;
  legion->left_bound = 0;
  legion->right_bound = N_COL - 1;
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
  s.move = FALSE;
  s.x_coord = 0.7;
  s.y_coord = -0.98;
  s.x_trans = 0;
  s.y_trans = 0;
  s.shooting = FALSE;
  s.element_width = 0.04;
  unsigned int i;
  for(i = 0; i < FIRE_LOAD; i++) {
    s.fire[i] = create_bullet(UP);
  }
  for(i = 0; i < 9; i++) {
    int row = i / 3;
    int col = i % 3;
    GLfloat x_coord = s.x_coord + (col - 1) * s.element_width;
    GLfloat y_coord = s.y_coord + row * s.element_width + s.element_width / 2;
    s.elements[i] = create_coord(x_coord, y_coord, 0, 0, s.element_width / 2);
  }
  int no_draw[] = {6, 7, 8, 3, 5};
  for(i = 0; i < 5; i++) {
    int index = no_draw[i];
    s.elements[index].draw = FALSE;
  }
  s.fire_pointer = 0;
  s.duration = 0;
  return s;
}

void update_trans(Legion * legion) {
  if(
     (legion->x_trans + legion->army[0][legion->left_bound].x_coord <= LEFT_BOUND ||
      legion->x_trans + legion->army[0][legion->right_bound].x_coord >= RIGHT_BOUND) &&
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
    if(self.move == FALSE) {
      self.duration--;
    }
  }
}

Bullet create_bullet(int direction) {
  Bullet b;
  b.direction = direction;
  b.fired = FALSE;
  return b;
}


void self_shoot_bullet() {
  Bullet* this_b = &self.fire[self.fire_pointer];
  this_b->fired = TRUE;
  this_b->x_coord = self.x_coord;
  this_b->y_coord = self.y_coord;
  this_b->x_trans = self.x_trans;
  this_b->y_trans = self.y_trans;
  self.fire_pointer = (self.fire_pointer + 1) % FIRE_LOAD;
}

void check_collision_self_bullet(Bullet* b, Legion* legion) {
  int col = 0, row = N_ROW;
  GLfloat b_x = b->x_coord + b->x_trans;
  GLfloat b_y = b->y_coord + b->y_trans;
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
      b->fired = FALSE;
      printf("shot alien row %d col %d\n", row, col);
      return;
    }
  }
}

void check_collision_legion(Legion *legion, Self *self) {
  unsigned int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      Bullet *b = &legion->army[i][j].b;
      if(b->fired) {
	check_collision_legion_bullet(b, self);
      }
    }
  }
}

void check_collision_legion_bullet(Bullet *b, Self *self) {
  int n_col = 3, n_row = 3;
  int col = 0, row = n_row;
  GLfloat b_x = b->x_coord + b->x_trans;
  GLfloat b_y = b->y_coord + b->y_trans;
  GLfloat half_width = self->elements[0].width;
  GLfloat top = self->elements[8].y_coord + half_width + self->y_trans;
  GLfloat bottom = self->elements[0].y_coord - half_width +self->y_trans;
  GLfloat left = self->elements[0].x_coord - half_width + self->x_trans; 
  GLfloat right = self->elements[8].x_coord + half_width + self->x_trans;
  if(b_x < left || b_x > right ||
     b_y > top || b_y < bottom){
     return;
  }
  
  while(col < n_col) {
    GLfloat x = self->elements[col].x_coord + self->x_trans;
    if(fabs(b_x - x) < half_width + 0.005)
      break;
    col++;
  }
  
  if (col == n_col){
    return;
  }
  
  while(row >= 0) {
    GLfloat y = self->elements[row * 3].y_coord + self->y_trans;
    if(fabs(b_y - y) < half_width + 0.005)
      break;
    row--;
  }
  
  if(row >= 0) {
    int index = row * 3 + col;
    if(self->elements[index].draw) {
      self->lives--;
      self->x_trans = 0;
      self->y_trans = 0;
      return;
    }
  }
}


void check_collision_self(Legion* legion) {
  unsigned int i;
  for(i = 0; i < FIRE_LOAD; i++) {
    Bullet* this_b = &self.fire[i];
    if(this_b->fired) {
      check_collision_self_bullet(this_b, legion);
    }
  }
}

void update_bound(Legion* legion) {
  int left_bound = legion->left_bound;
  while(all_dead(legion, left_bound)) {
    left_bound++;
  }
  legion->left_bound = left_bound;
  int right_bound = legion->right_bound;
  while(all_dead(legion, right_bound)) {
    right_bound--;
  }
  legion->right_bound = right_bound;
  printf("left bound %d, right bound %d\n", legion->left_bound, legion->right_bound);
}

int all_dead(Legion* legion, int col) {
  unsigned int i;
  for(i = 0; i < N_ROW; i++) {
    if(legion->army[i][col].status == ALIVE) {
      return FALSE;
    }
  }
  return TRUE;
}

void legion_fire(Legion *legion) {
  unsigned int i, j;
  srand((unsigned) time(&t));
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      Alien *this_a = &legion->army[i][j];
      if(this_a->status == ALIVE && this_a->b.fired == FALSE) {
	int lottery = rand() % 100;
	if(lottery > 97) {
	  this_a->b.fired = TRUE;
	  this_a->b.x_coord = this_a->x_coord;
	  this_a->b.y_coord = this_a->y_coord;
	  this_a->b.x_trans = legion->x_trans;
	  this_a->b.y_trans = legion->y_trans;
	}
      }
    }
  }
}

void draw_legion_bullets(Legion *legion) {
  unsigned int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      Bullet *this_b = &legion->army[i][j].b;
      glPushMatrix();
      glLoadIdentity();
      if(this_b->fired == TRUE) {
	draw_bullet(this_b);
      }
      glPopMatrix();
    }
  }
}


Coord create_coord(GLfloat x_coord, GLfloat y_coord, GLfloat x_trans, GLfloat y_trans, GLfloat width) {
  Coord c;
  c.x_coord = x_coord;
  c.y_coord = y_coord;
  c.x_trans = x_trans;
  c.y_trans = y_trans;
  c.width = width;
  c.draw = TRUE;
  return c;
}
