#include "boid.h"

Boid* init_boid(int count) {
  Boid *b = (Boid*) malloc(sizeof(Boid));
  GLfloat lx, ly, lz;
  GLfloat vx = 0, vy = 0, vz = 0;
  
  b->id = count + 1;
  
  b->location = gsl_vector_alloc(VECTOR_LENGTH);
  lx = BOID_INITIAL_X + rand() % BOID_INITIAL_RANGE;
  ly = BOID_INITIAL_Y + rand() % BOID_INITIAL_RANGE;
  lz = BOID_INITIAL_Z + rand() % BOID_INITIAL_RANGE;
  gsl_vector_set(b->location, X, lx);
  gsl_vector_set(b->location, Y, ly);
  gsl_vector_set(b->location, Z, lz);
  
  b->velocity = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_set(b->velocity, X, vx);
  gsl_vector_set(b->velocity, Y, vy);
  gsl_vector_set(b->velocity, Z, vz);
  b->angle = rand() % 360;
  b->wing_angle = 0;
  b->z_angle = 0;

  b->normal = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_set_basis(b->normal, 1);

  return b;
}

Goal init_goal() {
  Goal g;
  g.speed = 10;
  g.h_move = 0;
  g.x_move = 0;
  g.trans = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_set(g.trans, X, 2000);
  gsl_vector_set(g.trans, Y, 1500);
  gsl_vector_set(g.trans, Z, 1000);
  g.direction = gsl_vector_calloc(VECTOR_LENGTH);
  gsl_vector_set_basis(g.direction, 1);
  return g;
}

void init_views() {
  init_center_view();
  init_trailing_view();
  init_side_view();
  view = &center_view;
}

void update_view() {
  switch(v_mode){
  case CENTER:
    update_center_view();
    break;
  case TRAILING:
    update_trailing_view();
    break;
  case SIDE:
    update_side_view();
    break;
  }
}

void init_center_view() {
  center_view.pos = gsl_vector_calloc(VECTOR_LENGTH);
  gsl_vector_set(center_view.pos, 2, 1300);
  world_scale_vector(center_view.pos);
  
  center_view.up = gsl_vector_calloc(VECTOR_LENGTH);
  gsl_vector_set(center_view.up, 2, 1);

  center_view.look = calc_middleway(cache, count, g);
  world_scale_vector(center_view.look);
}

void init_trailing_view() {
  trailing_view.pos = trailing_position(cache, count, g);
  world_scale_vector(trailing_view.pos);
  
  trailing_view.look = calc_middleway(cache, count, g);
  world_scale_vector(trailing_view.look);
  
  trailing_view.up = gsl_vector_calloc(VECTOR_LENGTH);
  gsl_vector_set(trailing_view.up, 2, 1);
}

void init_side_view() {
  side_view.pos = side_position(cache, count, g);
  world_scale_vector(side_view.pos);

  side_view.look = calc_middleway(cache, count, g);
  world_scale_vector(side_view.look);
  
  side_view.up = gsl_vector_calloc(3);
  gsl_vector_set(side_view.up, 2, 1);
}

void update_center_view() {
  gsl_vector_free(center_view.look);
  center_view.look = calc_middleway(cache, count, g);
  world_scale_vector(center_view.look);
}

void update_trailing_view() {
  gsl_vector_free(trailing_view.look);
  trailing_view.look = calc_middleway(cache, count, g);
  world_scale_vector(trailing_view.look);

  gsl_vector_free(trailing_view.pos);
  trailing_view.pos = trailing_position(cache, count, g);
  world_scale_vector(trailing_view.pos);
}

void update_side_view() {
  gsl_vector_free(side_view.look);
  side_view.look = calc_middleway(cache, count, g);
  world_scale_vector(side_view.look);

  gsl_vector_free(side_view.pos);
  side_view.pos = side_position(cache, count, g);
  world_scale_vector(side_view.pos);
}
void camera_look() {
  const double * pos = gsl_vector_const_ptr(view->pos, 0);
  const double * look = gsl_vector_const_ptr(view->look, 0);
  const double * up = gsl_vector_const_ptr(view->up, 0);
  lookAt(
	    *pos, *(pos+1), *(pos+2),
	    *look, *(look+1), *(look+2),
	    *up, *(up+1), *(up+2)
	    );
}

void draw_goal(Goal g) {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, goal_vertices);
  glColorPointer(3, GL_FLOAT, 0, goal_colors);
  glPushMatrix();
  glScalef(world_scale[X], world_scale[Y], world_scale[Z]);
  glTranslatef(
	       gsl_vector_get(g.trans, X),
	       gsl_vector_get(g.trans, Y),
	       gsl_vector_get(g.trans, Z)
	       );
  glDrawArrays(GL_POINTS, 0, 1);
  glPopMatrix();
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void update_goal(Goal *g) {
  double y_trans = gsl_vector_get(g->trans, 1) + gsl_vector_get(g->direction, 1) * g->speed;
  gsl_vector_set(g->trans, 1, y_trans);
  if(fabs(y_trans) > WORLD_HALF_WIDTH) {
    gsl_vector_set(g->direction, 1, gsl_vector_get(g->direction, 1) * -1);
  }
  update_goal_height(g);
  update_goal_horizontal(g);
}

void update_goal_height(Goal *g) {
  if(g->h_move > 0){
    double v_direction = gsl_vector_get(g->direction, 2);
    double height = gsl_vector_get(g->trans, 2) + GOAL_VERTICAL_DELTA * v_direction;
    if(fabs(WORLD_HALF_WIDTH - height) < WORLD_HALF_WIDTH) {
      gsl_vector_set(g->trans, 2, height);
    }
    g->h_move--;
  }
}

void update_goal_horizontal(Goal* g) {
  if(g->x_move > 0) {
    double h_direction = gsl_vector_get(g->direction, 0);
    double x_trans = gsl_vector_get(g->trans, 0) + GOAL_HORIZONTAL_DELTA * h_direction;
    if(fabs(x_trans) < WORLD_HALF_WIDTH) {
      gsl_vector_set(g->trans, 0, x_trans);
    }
    g->x_move--;
  }
}

Boid** n_neighbours(Boid *target, Boid** list, int size,  int n) {
  int i;
  for( i = 0; i < size; i++) {
    if(list[i]->id == target->id){
      list[i]->dist = FLT_MAX;
    }
    list[i]->dist = get_dist(target, list[i]);
  }
  qsort(list, size, sizeof(Boid*), cmp);
  Boid **output = (Boid**) calloc(n, sizeof(Boid*));
  
  memcpy(output, list+1, sizeof(Boid*) * n);
  return output;
}

Boid** cache_linkedlist(Node *head){
  int size = get_ll_size(head);
  Boid** output = (Boid**) malloc(sizeof(Boid*) * size);
  Node *current = head->next;
  int i = 0;
  while(current->type == VAL) {
    memcpy(output + i, &current->data, sizeof(Boid*));
    current = current->next;
    i++;
  }
  return output;
}

int cmp(const void *aa, const void *bb) {
  Boid * a = *(Boid * const *)aa;
  Boid * b = *(Boid * const *)bb;
  if(a->dist > b->dist) {
    return 1;
  }else if(a->dist < b->dist) {
    return -1;
  }
  return 0;
}

GLfloat get_dist(Boid* a, Boid* b) {
  return point_dist(a->location, b->location);
}

void print_boid(Boid *b) {
  printf("printing boid No.%d\n", b->id);
  printf("location x %f, y %f, z %f\n", gsl_vector_get(b->location,X), gsl_vector_get(b->location, Y), gsl_vector_get(b->location, Z));
}

void print_boids(Node* head) {
  Node* current = head->next;
  while(current->type == VAL) {
    print_boid((Boid*) current->data);
    current = current->next;
  }
}

void print_view(View v){
  print_vector(v.pos);
  print_vector(v.look);
  print_vector(v.up);
}

void print_vector(gsl_vector * v){
  printf("x %f, y %f, z %f\n", gsl_vector_get(v, X),gsl_vector_get(v, Y),gsl_vector_get(v, Z));
}

void print_boids_array(Boid** bs, int size) {
  int i;
  for(i = 0; i < size; i++) {
    Boid* b = bs[i];
    printf("Array Boid No.%d\n", b->id);
    print_vector(b->location);
  }
}

gsl_vector* separation(Boid* b, Boid** neighbors) {
  gsl_vector* res = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_set_zero(res);
  int i;
  for (i = 0; i < NUM_NEIGHBORS; i++) {
    gsl_vector* diff1 = gsl_vector_alloc(VECTOR_LENGTH);
    gsl_vector* diff2 = gsl_vector_alloc(VECTOR_LENGTH);
    gsl_vector_set_zero(diff1);
    gsl_vector_add(diff1, (const gsl_vector*)neighbors[i]->location);
    gsl_vector_sub(diff1, (const gsl_vector*)b->location);
    gsl_vector_memcpy(diff2, diff1);
    gsl_vector_mul(diff1, diff1);
    double sum = sum_vector(diff1, VECTOR_LENGTH);
    gsl_vector_scale(diff2, 1.0 / sum);
    gsl_vector_add(res, diff2);
    gsl_vector_free(diff1);
    gsl_vector_free(diff2);
  }
  double* ptr = gsl_vector_ptr(res, 0);
  *ptr = *ptr * 1.5;
  *(ptr + 1) = *(ptr + 1) * 0.8;
  *(ptr + 2) = *(ptr + 2) * 0.3;
  gsl_vector_scale(res, SEPARATION_SCALE);
  return res;
}

gsl_vector* cohesion(Boid* b, Boid** neighbors) {
  gsl_vector* res = get_flock_center(neighbors, NUM_NEIGHBORS);
  gsl_vector_sub(res, (const gsl_vector*)b->location);
  gsl_vector_scale(res, COHESION_SCALE);
  return res;
}

gsl_vector* alignment(Boid*b, Boid** neighbors) {
  gsl_vector* res = gsl_vector_calloc(VECTOR_LENGTH);
  for (int i = 0; i < NUM_NEIGHBORS; i++) {
    gsl_vector_add(res, (const gsl_vector*)neighbors[i]->velocity);
  }

  gsl_vector_scale(res, ave_multiplier * ALIGNMENT_SCALE);
  return res;
}

gsl_vector* goal_seeking(Goal g, Boid* b) {
  gsl_vector* res = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_memcpy(res, g.trans);
  gsl_vector_sub(res, b->location);
  gsl_vector_scale(res, GOAL_SEEKING_SCALE);
  return res;
}

gsl_vector* get_flock_center(Boid** bs, int size) {
 gsl_vector* res = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_set_zero(res);
  for (int i = 0; i < size; i++) {
    gsl_vector_add(res, (const gsl_vector*)bs[i]->location);
  }
  gsl_vector_scale(res, 1.0 / size);
  return res;
}

gsl_vector* center_goal_direction(Boid** bs, int size, Goal g) {
  gsl_vector* res = get_flock_center(bs, size);
  gsl_vector_sub(res, g.trans);
  normalize_vector(res, VECTOR_LENGTH);
  return res;
}

gsl_vector* trailing_position(Boid** bs, int size, Goal g) {
  gsl_vector* center = get_flock_center(bs, size);
  gsl_vector* u = center_goal_direction(bs, size, g);

  double d = center_goal_dist(bs, size, g);
  double r = max_boid_goal_dist(bs, size, g);
  gsl_vector_scale(u, (d + 5 * r) * 0.3);
  
  gsl_vector_add(u, center);
  gsl_vector_set(u, Z, gsl_vector_get(u , 2) + (d + r)* 0.3 );
  gsl_vector_free(center);
  return u;
}

gsl_vector* side_position(Boid** bs, int size, Goal g) {
  gsl_vector* m = calc_middleway(bs, size, g);
  gsl_vector* u = center_goal_direction(bs, size, g);
  gsl_vector_scale(u, -1);
  gsl_vector* z = gsl_vector_calloc(VECTOR_LENGTH);
  gsl_vector_set_basis(z, 2);
  gsl_vector* p = gsl_vector_alloc(VECTOR_LENGTH);
  crossProduct(u, z, p);
  double d = center_goal_dist(bs, size, g);
  double r = max_boid_goal_dist(bs, size, g);
  normalize_vector(p, VECTOR_LENGTH);
  gsl_vector_scale(p, (r + 2 * d) * 0.5);
  gsl_vector_add(p, m);
  gsl_vector_set(p, 2, gsl_vector_get(p, 2) + (d + r) * 0.3);
  gsl_vector_free(m);
  gsl_vector_free(u);
  gsl_vector_free(z);
  return p;
}

double center_goal_dist(Boid** bs, int size, Goal g) {
  gsl_vector* center = get_flock_center(bs, size);
  double dist = point_dist(center, g.trans);
  gsl_vector_free(center);
  return dist;
}

gsl_vector* calc_middleway(Boid** bs, int size, Goal g) {
  gsl_vector* flock_center = get_flock_center(bs, size);
  gsl_vector* res = ave(flock_center, g.trans);
  gsl_vector_free(flock_center);
  return res;
}

double projection_cos(gsl_vector* v, gsl_vector* w) {
  gsl_vector * v_copy = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector * w_copy = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector * temp = gsl_vector_alloc(VECTOR_LENGTH);

  gsl_vector_memcpy(temp, v);
  gsl_vector_memcpy(v_copy, v);
  gsl_vector_memcpy(w_copy, w);

  gsl_vector_mul(temp, w_copy);
  gsl_vector_mul(v_copy, v_copy);
  gsl_vector_mul(w_copy, w_copy);
  double denominator = sum_vector(temp, VECTOR_LENGTH);
  double nominator = sqrt(sum_vector(v_copy, VECTOR_LENGTH)) * sqrt(sum_vector(w_copy, VECTOR_LENGTH));
  double cos = denominator / nominator;
  gsl_vector_free(temp);
  gsl_vector_free(v_copy);
  gsl_vector_free(w_copy);
  return cos;
}

gsl_vector* ave(gsl_vector* v, gsl_vector* w) {
  gsl_vector* res = gsl_vector_calloc(VECTOR_LENGTH);
  gsl_vector_add(res, v);
  gsl_vector_add(res, w);
  gsl_vector_scale(res, 0.5);
  return res;
}

double sum_vector(gsl_vector* v, int size) {
  int i;
  double sum = 0;
  for(i = 0; i < size; i++){
    sum += gsl_vector_get(v, i);
  }
  return sum;
}

void normalize_vector(gsl_vector* v, int size) {
  gsl_vector* temp = gsl_vector_alloc(size);
  gsl_vector_memcpy(temp, v);
  gsl_vector_mul(temp, temp);
  double sum = sum_vector(temp, size);
  sum = sqrt(sum);
  gsl_vector_scale(v, 1.0 / sum);
  gsl_vector_free(temp);
}

double point_dist(gsl_vector *v, gsl_vector *w) {
  gsl_vector* copy = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_memcpy(copy, v);
  gsl_vector_sub(copy, w);
  gsl_vector_mul(copy, copy);
  GLfloat dist = gsl_vector_get(copy, X) + gsl_vector_get(copy, Y) + gsl_vector_get(copy, Z);
  dist = sqrt(dist);
  gsl_vector_free(copy);
  return dist;
}

double max_boid_goal_dist(Boid**bs, int size, Goal g) {
  double max = 0;
  int i;
  for(i = 0; i < size; i++) {
    double dist = point_dist(bs[i]->location, g.trans);
    max = dist > max? dist : max;
  }
  return max;
}

void world_scale_vector(gsl_vector *v){
  double * ptr = gsl_vector_ptr(v, 0);
  *ptr = *ptr * world_scale[0];
  *(ptr + 1) = *(ptr + 1) * world_scale[1];
  *(ptr + 2) = *(ptr + 2) * world_scale[2];
}

double get_xy_angle(Boid* b) {
  double cos = projection_cos(b->normal, b->velocity);
  double angle = acos(cos) * ANGLE_CONVERTER;
  if(gsl_vector_get(b->velocity, 0) > 0) {
    angle = 360 - angle;
  }
  return angle;
}

double get_z_angle(Boid* b) {
  gsl_vector* xy_normal = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector_memcpy(xy_normal, b->velocity);
  gsl_vector_set(xy_normal, 2, 0);
  double cos = projection_cos(xy_normal, b->velocity);
  double angle = acos(cos) * ANGLE_CONVERTER;
  if(gsl_vector_get(b->velocity, 0) < 0) {
    angle = 360 - angle;
  }
  return angle;

void project_point(GLfloat* o, GLfloat* p, GLshort size) {
  gsl_vector* v = gsl_vector_alloc(VECTOR_LENGTH);
  gsl_vector* n = gsl_vector_alloc(VECTOR_LENGTH);
  double* v_ptr = gsl_vector_ptr(v, 0);
  gsl_vector_set_zero(n);
  gsl_vector_set(n, 2, 1);
  v_ptr[0] = o[0];
  v_ptr[1] = o[1];
  v_ptr[2] = o[2];
}

void free_view(View v) {
  gsl_vector_free(v.pos);
  gsl_vector_free(v.look);
  gsl_vector_free(v.up);
}

void free_views() {
  free_view(center_view);
  free_view(trailing_view);
  free_view(side_view);
}

void free_boid(Boid* b) {
  gsl_vector_free(b->location);
  gsl_vector_free(b->velocity);
  free(b);
}

void free_boids() {
  Node* current = head->next;
  while(current->type == VAL) {
    Node* temp = current;
    current = current->next;
    if (current->type == VAL) {
      Boid* b = (Boid*)temp->data;
      free_boid(b);
    }
    free_node(temp);
  } 
}
