#include "boid.h"

Boid* init_boid(int count) {
  Boid *b = (Boid*) malloc(sizeof(Boid));
  GLfloat lx, ly, lz;
  GLfloat vx = 0, vy = 0, vz = 0;
  
  b->id = count + 1;
  
  b->location = gsl_vector_alloc(3);
  lx = 2375 + rand() % 51;
  ly = 125 + rand() % 51;
  lz = 1175 + rand() % 51;
  gsl_vector_set(b->location, 0, lx);
  gsl_vector_set(b->location, 1, ly);
  gsl_vector_set(b->location, 2, lz);
  
  b->velocity = gsl_vector_alloc(3);
  gsl_vector_set(b->velocity, 0, vx);
  gsl_vector_set(b->velocity, 1, vy);
  gsl_vector_set(b->velocity, 2, vz);
  b->angle = rand() % 360;

  b->normal = gsl_vector_alloc(3);
  gsl_vector_set_basis(b->normal, 1);
  return b;
}

Goal init_goal() {
  Goal g;
  g.angle = 0;
  g.radius = 1500;
  g.trans = gsl_vector_alloc(3);
  gsl_vector_set(g.trans, 0, g.radius * sin(g.angle));
  gsl_vector_set(g.trans, 1, g.radius * cos(g.angle));
  gsl_vector_set(g.trans, 2, 1000);
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
  center_view.pos = gsl_vector_calloc(3);
  gsl_vector_set(center_view.pos, 2, 1300);
  world_scale_vector(center_view.pos);
  
  center_view.up = gsl_vector_calloc(3);
  gsl_vector_set(center_view.up, 2, 1);

  center_view.look = calc_middleway(cache, count, g);
  world_scale_vector(center_view.look);
}

void init_trailing_view() {
  trailing_view.pos = trailing_position(cache, count, g);
  world_scale_vector(trailing_view.pos);
  
  trailing_view.look = calc_middleway(cache, count, g);
  world_scale_vector(trailing_view.look);
  
  trailing_view.up = gsl_vector_calloc(3);
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
  glScalef(world_scale[0], world_scale[1], world_scale[2]);
  glTranslatef(
	       gsl_vector_get(g.trans, 0),
	       gsl_vector_get(g.trans, 1),
	       gsl_vector_get(g.trans, 2)
	       );
  glDrawArrays(GL_POINTS, 0, 1);
  glPopMatrix();
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void update_goal(Goal *g) {
  // gsl_vector_set(g->trans, 0, gsl_vector_get(g->trans, 0) + 5);
  g->angle += 0.005;
  gsl_vector_set(g->trans, 0, g->radius * sin(g->angle));
  gsl_vector_set(g->trans, 1, g->radius * cos(g->angle));
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
  printf("location x %f, y %f, z %f\n", gsl_vector_get(b->location,0), gsl_vector_get(b->location, 1), gsl_vector_get(b->location, 2));
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
  printf("x %f, y %f, z %f\n", gsl_vector_get(v, 0),gsl_vector_get(v, 1),gsl_vector_get(v, 2));
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
  gsl_vector* res = gsl_vector_alloc(3);
  gsl_vector_set_zero(res);
  for (int i = 0; i < NUM_NEIGHBORS; i++) {
    gsl_vector* diff1 = gsl_vector_alloc(3);
    gsl_vector* diff2 = gsl_vector_alloc(3);
    gsl_vector_set_zero(diff1);
    gsl_vector_add(diff1, (const gsl_vector*)neighbors[i]->location);
    gsl_vector_sub(diff1, (const gsl_vector*)b->location);
    gsl_vector_memcpy(diff2, diff1);
    gsl_vector_mul(diff1, diff1);
    double sum = sum_vector(diff1, 3);
    gsl_vector_scale(diff2, 1.0 / sum);
    gsl_vector_add(res, diff2);
    gsl_vector_free(diff1);
    gsl_vector_free(diff2);
  }
  
  gsl_vector_scale(res, -1);
  return res;
}

gsl_vector* cohesion(Boid* b, Boid** neighbors) {
  gsl_vector* res = get_flock_center(neighbors, NUM_NEIGHBORS);
  gsl_vector_sub(res, (const gsl_vector*)b->location);
  gsl_vector_scale(res, 0.0001);
  return res;
}

gsl_vector* alignment(Boid*b, Boid** neighbors) {
  gsl_vector* res = gsl_vector_calloc(3);
  for (int i = 0; i < NUM_NEIGHBORS; i++) {
    gsl_vector_add(res, (const gsl_vector*)neighbors[i]->velocity);
  }

  gsl_vector_scale(res, ave_multiplier * 0.001);
  return res;
}

gsl_vector* goal_seeking(Goal g, Boid* b) {
  gsl_vector* res = gsl_vector_alloc(3);
  gsl_vector_memcpy(res, g.trans);
  gsl_vector_sub(res, b->location);
  gsl_vector_scale(res, 0.00003);
  return res;
}

gsl_vector* get_flock_center(Boid** bs, int size) {
 gsl_vector* res = gsl_vector_calloc(3);
 for (int i = 0; i < size; i++) {
   gsl_vector_add(res, (const gsl_vector*)bs[i]->location);
 }
 gsl_vector_scale(res, 1.0 / size);
 return res;
}

gsl_vector* center_goal_direction(Boid** bs, int size, Goal g) {
  gsl_vector* res = get_flock_center(bs, size);
  gsl_vector_sub(res, g.trans);
  normalize_vector(res, 3);
  return res;
}

gsl_vector* trailing_position(Boid** bs, int size, Goal g) {
  gsl_vector* center = get_flock_center(bs, size);
  gsl_vector* u = center_goal_direction(bs, size, g);

  double d = center_goal_dist(bs, size, g);
  double r = max_boid_goal_dist(bs, size, g);
  gsl_vector_scale(u, (d + 5 * r) * 0.3);
  
  gsl_vector_add(u, center);
  gsl_vector_set(u, 2, gsl_vector_get(u , 2) + (d + r)* 0.3 );
  gsl_vector_free(center);
  return u;
}

gsl_vector* side_position(Boid** bs, int size, Goal g) {
  gsl_vector* m = calc_middleway(bs, size, g);
  gsl_vector* u = center_goal_direction(bs, size, g);
  gsl_vector_scale(u, -1);
  gsl_vector* z = gsl_vector_calloc(3);
  gsl_vector_set_basis(z, 2);
  gsl_vector* p = gsl_vector_alloc(3);
  crossProduct(u, z, p);
  double d = center_goal_dist(bs, size, g);
  double r = max_boid_goal_dist(bs, size, g);
  normalize_vector(p, 3);
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
  gsl_vector * v_copy = gsl_vector_alloc(3);
  gsl_vector * w_copy = gsl_vector_alloc(3);
  gsl_vector * temp = gsl_vector_alloc(3);

  gsl_vector_memcpy(temp, v);
  gsl_vector_memcpy(v_copy, v);
  gsl_vector_memcpy(w_copy, w);

  gsl_vector_mul(temp, w_copy);
  gsl_vector_mul(v_copy, v_copy);
  gsl_vector_mul(w_copy, w_copy);
  double denominator = sum_vector(temp, 3);
  double nominator = sqrt(sum_vector(v_copy, 3)) * sqrt(sum_vector(w_copy, 3));
  double cos = denominator / nominator;
  gsl_vector_free(temp);
  gsl_vector_free(v_copy);
  gsl_vector_free(w_copy);
  return cos;
}

gsl_vector* ave(gsl_vector* v, gsl_vector* w) {
  gsl_vector* res = gsl_vector_calloc(3);
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
  gsl_vector* copy = gsl_vector_alloc(3);
  gsl_vector_memcpy(copy, v);
  gsl_vector_sub(copy, w);
  gsl_vector_mul(copy, copy);
  GLfloat dist = gsl_vector_get(copy, 0) + gsl_vector_get(copy, 1) + gsl_vector_get(copy, 2);
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

double get_angle(Boid* b) {
  double cos = projection_cos(b->normal, b->velocity);
  double angle = acos(cos) * ANGLE_CONVERTER;
  if(gsl_vector_get(b->velocity, 0) > 0) {
    angle = 360 - angle;
  }
  return angle;
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
  while(head->next != NULL) {
    Node* current = head->next;
    head->next = head->next->next;
    if (current->type != HEAD_TAIL) {
      free_boid((Boid*)current);
    }
    free_node(current);
  }  
  free(head);
}
