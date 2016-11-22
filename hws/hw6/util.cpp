#include "util.hpp"

GLfloat ocean_vertices[12];
View v;
extern Flock f;
extern meshManager terrain_mesh, ship_mesh, athena_mesh;
extern enum VIEW_TYPE v_mode;

extern GLuint t, t2, day_time, ocean_shader, boid_shader, terrain_shader, athena_shader;
extern GLfloat glTime, glOceanTime;
extern GLuint project, view, model;
extern glm::mat4 project_mat, view_mat, model_mat;
extern GLfloat eye_dist, scale_factor;
extern GLuint boid_model, boid_view, boid_project;
extern GLuint terrain_vao, terrain_ebo, terrain_model, terrain_view, terrain_project;
extern GLuint athena_vao, athena_ebo, athena_view, athena_project, athena_model;
extern GLuint ocean_vbo_index;
extern GLuint light1, light2;
extern GLuint frame_counter;
extern GLfloat lighting_conditions[36];
extern GLuint light_pos;
extern glm::vec3 light_position;

void calc_ocean_vertices(GLfloat len) {
  ocean_vertices[0] = -len / 2;
  ocean_vertices[1] = len / 2;
  ocean_vertices[2] = 0;

  ocean_vertices[3] = -len / 2;
  ocean_vertices[4] = -len / 2;
  ocean_vertices[5] = 0;

  ocean_vertices[6] = len / 2;
  ocean_vertices[7] = -len / 2;
  ocean_vertices[8] = 0;

  ocean_vertices[9] = len / 2;
  ocean_vertices[10] = len / 2;
  ocean_vertices[11] = 0;
}

void draw_ocean(GLuint vao) {
  glUseProgram(ocean_shader);
  update_ocean_time();
  update_day_time(ocean_shader);
  glUniform3fv(glGetUniformLocation(ocean_shader, "light_position"), 1, glm::value_ptr(light_position));
  glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ocean_vbo_index);
  glm::mat4 ocean_model = glm::mat4(1.0);
  glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(ocean_model));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
}

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glUseProgram(boid_shader);
  update_day_time(boid_shader);
  glUniform3fv(glGetUniformLocation(ocean_shader, "light_position"), 1, glm::value_ptr(light_position));
  glUniformMatrix4fv(boid_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glUniformMatrix4fv(boid_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

  for(int i = 0; i < f->count; i++) {
    glm::vec3 translate_vec = glm::vec3((*f->pos)[i][0], (*f->pos)[i][1], (*f->pos)[i][2]);
    glm::mat4 b_model = glm::translate(translate_vec);
    GLfloat xy_angle = (atan2((*f->vel)[i][1], (*f->vel)[i][0]) + 1.5708 * 3);
    b_model = b_model * glm::rotate(xy_angle, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(boid_model, 1, GL_FALSE, glm::value_ptr(b_model));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
    update_time(i);
  }
}

void draw_terrain() {
  glUseProgram(terrain_shader);
  glBindVertexArray(terrain_vao);
  glUniformMatrix4fv(terrain_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(terrain_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glm::mat4 model_mat =
    glm::translate(terrain_mesh.trans_vec) *
    glm::scale(glm::vec3(terrain_mesh.scale));
  glUniformMatrix4fv(terrain_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_ebo);
  glDrawElements(GL_TRIANGLES, terrain_mesh.num_of_indices, GL_UNSIGNED_INT, (void*)0);
}

void draw_ship() {

}

void draw_athena() {
  glUseProgram(athena_shader);
  glBindVertexArray(athena_vao);
  glUniformMatrix4fv(athena_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(athena_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glm::mat4 model_mat =
    glm::translate(athena_mesh.trans_vec) *
    glm::scale(glm::vec3(athena_mesh.scale));
  glUniformMatrix4fv(athena_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, athena_ebo);
  glDrawElements(GL_TRIANGLES, athena_mesh.num_of_indices, GL_UNSIGNED_INT, (void*)0);
}

void update_view(glm::mat4 &view, Flock& f) {
  switch(v_mode) {
    case CENTER:
      center_view(view, f);
      break;
    case TRAILING:
      trailing_view(view, f);
      break;
    case SIDE:
      side_view(view, f);
      break;
    case FIRST_PERSON:
      first_person_view(view, f);
      break;
  }
}

void center_view(glm::mat4& view, Flock &f) {
  glm::vec3 pos = glm::vec3(0.0, 0.0, 1800);
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  vec3 my_look = calc_middleway(f);
  glm::vec3 look = glm::vec3(my_look[0], my_look[1], my_look[2]);
  view = glm::lookAt(pos, look, up);
}

void side_view(glm::mat4& view, Flock &f) {
  vec3 my_pos = get_side_pos(f);
  glm::vec3 pos = glm::vec3(my_pos[0], my_pos[1], my_pos[2]);

  vec3 my_look = calc_middleway(f);
  glm::vec3 look = glm::vec3(my_look[0], my_look[1], my_look[2]);

  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(pos, look, up);
}

void trailing_view(glm::mat4& view, Flock &f) {
  vec3 my_pos = get_trailing_pos(f);
  glm::vec3 pos = glm::vec3(my_pos[0], my_pos[1], my_pos[2]);
  vec3 my_look = calc_middleway(f);
  glm::vec3 look = glm::vec3(my_look[0], my_look[1], my_look[2]);
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(pos, look, up);
}

void first_person_view(glm::mat4& view, Flock &f) {
  vec3 boid_pos = f.pos->at(0);
  glm::vec3 pos = glm::vec3(boid_pos[0], boid_pos[1], boid_pos[2]);
  vec3 my_look = get_center_pos(f);
  glm::vec3 look = glm::vec3(my_look[0], my_look[1], my_look[2]);
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(pos, look, up);
}

vec3 ave_flock_center(Flock& f) {
  return f.center;
}

vec3 calc_middleway(Flock& f) {
  vec3 ave_center = ave_flock_center(f);
  return (ave_center + f.goal) / 2;
}

vec3 get_side_pos(Flock& f) {
  vec3 z = vec3(0.0, 0.0, 1.0);
  vec3 u, m;
  GLfloat d;
  GLfloat r;
  if(f.count == 0 ) {
    m = f.goal;
    u = f.goal;
    d = 400;
    r = 200;
  } else {
    d = center_goal_dist(f);
    r = max_boid_goal_dist(f);
    m = calc_middleway(f);
    vec3 c = ave_flock_center(f);
    u = f.goal - c;
  }
  vec3 p = vec3::cross(u, z);
  p = vec3::normalize(p);
  p = p * (d + 2 * r);
  m = m + p;
  m = m + vec3(0.0, 0.0, d + r);
  return m;
}

vec3 get_trailing_pos(Flock& f) {
  vec3 m;
  GLfloat d = center_goal_dist(f);
  GLfloat r = max_boid_goal_dist(f);
  if(f.count == 0) {
    m = f.goal;
    m = f.goal - (f.goal_v) * 50;
    m[2] = m[2] + 400;
  } else {
    m = calc_middleway(f);
    vec3 c = ave_flock_center(f);
    vec3 u = f.goal - c;
    m = m - vec3::normalize(u) * (d + 5 * r);
    m[2] = m[2] + d + r;
  }
  return m;
}

vec3 get_center_pos(Flock& f) {
  vec3 boid_pos = f.pos->at(0);
  vec3 boid_v = f.vel->at(0);
  vec3 center = boid_pos + (boid_v * 100);
  center[2] = 0;
  return center;
}

GLfloat center_goal_dist(Flock& f) {
  vec3 center = ave_flock_center(f);
  return (center - f.goal).len();
}

GLfloat max_boid_goal_dist(Flock& f) {
  GLfloat max = 0;
  vec3 center = ave_flock_center(f);
  for (int i = 0; i < f.count; i++) {
    GLfloat dist = ((*(f.pos))[i] - center).len();
    if (dist > max) {
      max = dist;
    }
  }
  return max;
}

void init_time() {
  glTime = (sin(glfwGetTime() * 10) + 1) / 2;
  t = glGetUniformLocation(boid_shader, "time");
  glUniform1f(t, glTime);

  t2 = glGetUniformLocation(ocean_shader, "ocean_time");
  glOceanTime = sin(glfwGetTime());
  glUniform1f(t2, glOceanTime);
}

void update_time(int index) {
  glTime = (sin(glfwGetTime() * 10 + (*(f.seed))[index]) + 1) / 2;
  glUniform1f(t, glTime);
}

void update_ocean_time() {
  glOceanTime = sin(glfwGetTime() / 2);
  glUniform1f(t2, glOceanTime);
}

void update_day_time(GLuint shader) {
  light1 = glGetUniformLocation(shader, "light1");
  light2 = glGetUniformLocation(shader, "light2");
  int i = (frame_counter / 1800) % 4;
  GLfloat *l1, *l2;
  l1 = lighting_conditions + i * 9;
  l2 = lighting_conditions + (i + 1) * 9 % 36;
  glUniformMatrix3fv(light1, 1, GL_FALSE, l1);
  glUniformMatrix3fv(light2, 1, GL_FALSE, l2);
  glUniform1f(day_time, (frame_counter % 1800) / 1800.0);
}

void update_frame_counter() {
  frame_counter++;
  if (frame_counter >= 7200) {
    frame_counter -= 7200;
  }
}

void update_light_position() {
  if (frame_counter >= 0 && frame_counter < 5400) {
    light_position[0] += WORLD_SIZE / 5400.0;
  } else {
    light_position[0] -= WORLD_SIZE / 1800.0;
  }
}

void print_step_msg(Flock* f) {
  cout << "*******************************************************************************" << endl;
  f->print_boids();
  cout << "-------------------------------------------------------------------------------" << endl;
  f->print_goal();
  cout << "*******************************************************************************" << endl;
}
