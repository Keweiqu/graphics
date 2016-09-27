#include "boid.h"
Boid init_boid() {
  Boid b;
  
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

Node* n_neighbours(Node *target, Node* list, int n) {
  Node* output = create_linkedlist();
  int size = 0, i;
  Node *current = list->next;
  while(current->type == VAL){
    if(current->id == target->id){
      current = current->next;
      continue;
    }
    
    gsl_vector dist_vct = gsl_vector_sub(target->location, current->location);
    GLfloat dist = gsl_vector_mul(dist_vct, dist_vct);
    current->dist = dist;
    
    if(size < n) {
      insert(output, current);
    }else {
      attemp_swap(output, current);
    }
    current = current->next;
  }
  return output;
}

void insert(Node *head, Node *item) {
  Node * current = head->next;
  while(current->type == VAL && item->dist < current->dist) {
    current = current->next;
  }
  if(current->type == HEAD_TAIL) {
    
  }
}

