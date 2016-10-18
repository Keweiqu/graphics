#include "util.hpp"

extern glm::mat4 model, view, project;
extern glm::vec3 world_scale;
void draw_flock(Flock f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  for(int i = 0; i < f.count; i++) {
    glm::mat4 model = glm::mat4(1.0);
    model = project * view * model;
    scalef(world_scale[0], world_scale[1], world_scale[2], &model);
    translatef((*f.pos_x)[i], (*f.pos_y)[i], (*f.pos_z)[i], &model);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

  }
}
