#include "util.hpp"

extern glm::mat4 view, project;
extern glm::vec3 world_scale;
void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  
  for(int i = 0; i < f->count; i++) {
    glm::mat4 result = glm::mat4(1.0);
    result = project;
    scalef(world_scale[0], world_scale[1], world_scale[2], &result);
    result = result * view;
    translatef((*f->pos_x)[i], (*f->pos_y)[i], (*f->pos_z)[i], &result);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

  }
}


void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  glm::mat4 result = glm::mat4(1.0);
  result = project;
  scalef(world_scale[0], world_scale[1], world_scale[2], &result);
  result = result * view;

  translatef(f->goal[0], f->goal[1], f->goal[2], &result);
  glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)0);
}


