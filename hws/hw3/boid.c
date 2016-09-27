#include "boid.h"

Boid* init_boid(int count) {
  Boid *b = (Boid*) malloc(sizeof(Boid));
  GLfloat lx, ly, lz;
  GLfloat vx, vy, vz;
  
  b->id = count + 1;
  
  b->location = gsl_vector_alloc(3);
  lx = 2375 + rand() % 51;
  ly = 125 + rand() % 51;
  lz = 1175 + rand() % 51;
  gsl_vector_set(b->location, 0, lx);
  gsl_vector_set(b->location, 1, ly);
  gsl_vector_set(b->location, 2, lz);
  
  b->velocity = gsl_vector_alloc(3);
  vx = vy = vz = rand() % 41;
  gsl_vector_set(b->velocity, 0, vx);
  gsl_vector_set(b->velocity, 1, vy);
  gsl_vector_set(b->velocity, 2, vz);
  b->angle = rand() % 360;
  return b;
}

Goal init_goal() {
  Goal g;
  g.angle = 0;
  g.radius = 2000;
  g.x_trans = g.radius * sin(g.angle);
  g.y_trans = g.radius * cos(g.angle);
  return g;
}


void draw_goal(Goal g) {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, goal_vertices);
  glColorPointer(3, GL_FLOAT, 0, goal_colors);
  glPushMatrix();
  glScalef(0.0001, 0.0001, 0.0005);
  glTranslatef(sin(g.angle) * g.radius, cos(g.angle) * g.radius, 1000);
  glDrawArrays(GL_POINTS, 0, 1);
  glPopMatrix();
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void update_goal(Goal *g) {
  g->angle += 0.005;
  g->x_trans = g->radius * sin(g->angle);
  g->y_trans = g->radius * cos(g->angle);
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
  gsl_vector* copy = gsl_vector_alloc(3);
  gsl_vector_memcpy(copy, a->location);
  gsl_vector_sub(copy, b->location);
  gsl_vector_mul(copy, copy);
  GLfloat dist = gsl_vector_get(copy, 0) + gsl_vector_get(copy, 1) + gsl_vector_get(copy, 2);
  gsl_vector_free(copy);
  return dist;
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
    gsl_vector_add(res, (const gsl_vector*)neighbors[i]->location);
    gsl_vector_sub(res, (const gsl_vector*)b->location);
  }
  gsl_vector_scale(res, -1);
  return res;
}

gsl_vector* cohesion(Boid* b, Boid** neighbors) {
  gsl_vector* res = gsl_vector_alloc(3);
  gsl_vector_set_zero(res);
  for (int i = 0; i < NUM_NEIGHBORS; i++) {
    gsl_vector_add(res, (const gsl_vector*)neighbors[i]->location);
  }
  gsl_vector_scale(res, NUM_NEIGHBORS);
  gsl_vector_sub(res, (const gsl_vector*)b->location);
  return res;
}

gsl_vector* alignment(Boid*b, Boid** neighbors) {
  gsl_vector* res = gsl_vector_alloc(3);
  gsl_vector_set_zero(res);
  for (int i = 0; i < NUM_NEIGHBORS; i++) {
    gsl_vector_add(res, (const gsl_vector*)neighbors[i]->velocity);
  }
  gsl_vector_scale(res, NUM_NEIGHBORS);
  return res;
}

gsl_vector* goal_seeking(Goal g, Boid* b) {
  gsl_vector* res = gsl_vector_alloc(3);
  gsl_vector_set_zero(res);
  return res;
}
