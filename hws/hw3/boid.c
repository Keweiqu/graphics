#include "boid.h"
Boid* init_boid() {
  Boid* b;
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
  g.radius = 8000;
  return g;
}


void draw_goal(Goal g) {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, goal_vertices);
  glColorPointer(3, GL_FLOAT, 0, goal_vertices);
  glPushMatrix();
  glScalef(0.0001, 0.0001, 0.0001);
  glTranslatef(sin(g.angle) * g.radius, cos(g.angle) * g.radius, 0);
  glDrawArrays(GL_POINTS, 0, 1);
  glPopMatrix();
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void update_goal(Goal *g) {
  g->angle += 0.005;
}

Boid** n_neighbours(Boid *target, Boid** list, int n) {
  int size = sizeof(list) / sizeof(list[0]);
  int i;
  for( i = 0; i < size; i++) {
    if(list[i]->id == target->id){
      list[i]->dist = FLT_MAX;
    }
    list[i]->dist = get_dist(target, list[i]);
  }
  qsort(list, size, cmp);
  Boid **output = (Boid**) calloc(sizeof(Boid*) * n);
  
  memcpy(output, list, sizeof(Boid*) * n);
}

Boid** cache_linkedlist(Node *head){
  int size = get_ll_size(head);
  Boid* output = (Boid**) calloc(sizeof(Boid*) * size);
  Node *current = head->next;
  int i = 0;
  while(current->type == VAL) {
    memcpy(output[i], current->data, sizeof(Boid*));
    current = current->next;
    i++;
  }
  return output;
}

int cmp(Boid *a, Boid *b) {
  if(a->dist > b->dist) {
    return 1;
  }else if(a->dist < b->dist) {
    return -1
  }
  return 0;
}

GLfloat get_dist(Boid* a, Boid* b) {
  gsl_vector* copy;
  gsl_vector_memcpy(copy, a->location);
  gsl_vector_mul(copy, b->location);
  GLfloat dist = gsl_vector_get(copy, 0) + gsl_vector_get(copy, 1) + gsl_vector_get(copy, 2);
  return dist;
}

