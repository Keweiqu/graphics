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
