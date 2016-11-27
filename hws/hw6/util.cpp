#include "util.hpp"

GLfloat ocean_vertices[12];
GLfloat free_view_angle = 0.0;
View v;
extern Flock f;
extern meshManager terrain_mesh, ship_mesh, athena_mesh, sphere_mesh, nike_mesh, bear_mesh;
extern enum VIEW_TYPE v_mode;
extern int up_down, left_right;
extern GLuint t, t2, day_time, ocean_shader, boid_shader, terrain_shader, athena_shader, bear_shader;
extern GLfloat glTime, glOceanTime;
extern GLuint project, view, model;
extern glm::mat4 project_mat, view_mat, model_mat;
extern GLfloat view_angle;
extern GLuint boid_model, boid_view, boid_project;
extern GLuint terrain_vao, terrain_ebo, terrain_model, terrain_view, terrain_project;
extern GLuint athena_model, athena_view, athena_project, athena_view_pos;
extern GLuint athena_vao, athena_ebo;
extern GLuint sphere_vao, sphere_ebo;
extern GLuint nike_vao, nike_ebo;
extern GLuint bear_vao, bear_ebo, bear_view, bear_project, bear_model;
extern GLuint ocean_vbo_index;
extern GLuint ocean_tex_sampler0, ocean_tex_sampler1;
extern Image ocean0, ocean1;
extern GLuint light1, light2;
extern GLuint frame_counter, at_night;
extern GLuint textures[5], box_texture[4];
extern Image box[4];
extern GLfloat lighting_conditions[36];
extern GLfloat sphere_trans[9];
extern GLfloat sphere_scale[3];
extern GLfloat ocean_trans[18];
extern GLfloat ocean_rotate_angles[18];
extern GLuint light_pos, spotlight_pos, spotlight_dire;
extern glm::vec3 light_position, spotlight_position, spotlight_direction, cursor_position;
extern glm::vec3 eye_pos, look_pos, eye_trans;

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

  vec3 boid_pos = f.pos->at(0);
  spotlight_position = glm::vec3(boid_pos[0], boid_pos[1] + 10, boid_pos[2]);
  vec3 center_pos = get_first_person_center_pos(f);
  spotlight_direction = glm::vec3(center_pos[0] - boid_pos[0], center_pos[1] - (boid_pos[1] + 10), center_pos[2] - boid_pos[2]);
  glUniform3fv(spotlight_pos, 1, glm::value_ptr(spotlight_position));
  glUniform3fv(spotlight_dire, 1, glm::value_ptr(spotlight_direction));
  glUniform3fv(glGetUniformLocation(ocean_shader, "light_position"), 1, glm::value_ptr(light_position));
  glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniform1i(glGetUniformLocation(ocean_shader, "atNight"), 1);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ocean_vbo_index);
  for (int i = 0; i < 6; i++) {
    bind_ocean_texture(i);
    glm::mat4 ocean_model =
      glm::translate(glm::vec3(ocean_trans[i * 3], ocean_trans[i * 3 + 1], ocean_trans[i * 3 + 2])) *
      glm::rotate((float)(ocean_rotate_angles[i * 3] * DEGREE_TO_RADIAN), glm::vec3(1.0, 0.0, 0.0)) *
      glm::rotate((float)(ocean_rotate_angles[i * 3 + 1] * DEGREE_TO_RADIAN), glm::vec3(0.0, 1.0, 0.0)) *
      glm::rotate((float)(ocean_rotate_angles[i * 3 + 2] * DEGREE_TO_RADIAN), glm::vec3(0.0, 0.0, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(ocean_model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  }
}

void bind_ocean_texture(int i) {
  if (i == 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3160, 2592, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean0.data);
    glUniform1i(ocean_tex_sampler0, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2804, 2592, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean1.data);
    glUniform1i(ocean_tex_sampler1, 1);
  } else {
    glActiveTexture(GL_TEXTURE6 + i - 1);
    glBindTexture(GL_TEXTURE_2D, box_texture[i - 1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, box[i - 1].data);
    glUniform1i(ocean_tex_sampler0, 6 + i - 1);
    glUniform1i(ocean_tex_sampler1, 6 + i - 1);
  }
}

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glUseProgram(boid_shader);
  update_day_time(boid_shader);
  glUniform3fv(glGetUniformLocation(boid_shader, "spotlight_position"), 1, glm::value_ptr(spotlight_position));
  glUniform3fv(glGetUniformLocation(boid_shader, "spotlight_direction"), 1, glm::value_ptr(spotlight_direction));
  glUniform1i(glGetUniformLocation(boid_shader, "atNight"), 1);
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

void draw_terrain(meshManager& mesh, GLuint vao, GLuint ebo) {
  glUseProgram(terrain_shader);
  glBindVertexArray(vao);
  glUniformMatrix4fv(terrain_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(terrain_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glm::mat4 model_mat =
    glm::translate(mesh.trans_vec) *
    glm::scale(glm::vec3(mesh.scale));
  glUniformMatrix4fv(terrain_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glDrawElements(GL_TRIANGLES, mesh.num_of_indices, GL_UNSIGNED_INT, (void*)0);
}

void draw_statue(meshManager& mesh, GLuint vao, GLuint ebo) {
  glUseProgram(athena_shader);
  glBindVertexArray(vao);
  glUniformMatrix4fv(athena_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(athena_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glm::mat4 model_mat =
    glm::translate(mesh.trans_vec) *
    glm::scale(glm::vec3(mesh.scale)) *
    glm::rotate(mesh.rotate_angles[0], glm::vec3(1.0, 0.0, 0.0)) *
    glm::rotate(mesh.rotate_angles[1], glm::vec3(0.0, 1.0, 0.0)) *
    glm::rotate(mesh.rotate_angles[2], glm::vec3(0.0, 0.0, 1.0));
  glUniformMatrix4fv(athena_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  glUniform4fv(athena_view_pos, 1, glm::value_ptr(eye_pos));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glDrawElements(GL_TRIANGLES, mesh.num_of_indices, GL_UNSIGNED_INT, (void*)0);
}

void draw_sphere() {
  glUseProgram(athena_shader);
  glBindVertexArray(sphere_vao);
  glUniformMatrix4fv(athena_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(athena_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glUniform4fv(athena_view_pos, 1, glm::value_ptr(eye_pos));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);

  for (int i = 0; i < 3; i++) {
    glm::mat4 model_mat =
      glm::translate(sphere_mesh.trans_vec + glm::vec3(sphere_trans[i * 3], sphere_trans[i * 3 + 1], sphere_trans[i * 3 + 2])) *
      glm::scale(glm::vec3(sphere_mesh.scale * sphere_scale[i])) *
      glm::rotate(sphere_mesh.rotate_angles[0], glm::vec3(1.0, 0.0, 0.0)) *
      glm::rotate(sphere_mesh.rotate_angles[1], glm::vec3(0.0, 1.0, 0.0)) *
      glm::rotate(sphere_mesh.rotate_angles[2], glm::vec3(0.0, 0.0, 1.0));
      glUniformMatrix4fv(athena_model, 1, GL_FALSE, glm::value_ptr(model_mat));
      glDrawElements(GL_TRIANGLES, sphere_mesh.num_of_indices, GL_UNSIGNED_INT, (void*)0);
  }
}

void draw_bear() {
  glUseProgram(bear_shader);
  glBindVertexArray(bear_vao);
  glUniformMatrix4fv(bear_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(bear_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glm::mat4 model_mat =
    glm::translate(bear_mesh.trans_vec) *
    glm::scale(glm::vec3(bear_mesh.scale)) *
    glm::rotate(bear_mesh.rotate_angles[0], glm::vec3(1.0, 0.0, 0.0)) *
    glm::rotate(bear_mesh.rotate_angles[1], glm::vec3(0.0, 1.0, 0.0)) *
    glm::rotate(bear_mesh.rotate_angles[2], glm::vec3(0.0, 0.0, 1.0));
  glUniformMatrix4fv(bear_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bear_ebo);
  glDrawElements(GL_TRIANGLES, bear_mesh.num_of_indices, GL_UNSIGNED_INT, (void*)0);
}

void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glUseProgram(boid_shader);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  glUniformMatrix4fv(boid_project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glUniformMatrix4fv(boid_view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glm::mat4 model_mat = glm::translate(glm::vec3(f->goal[0], f->goal[1], f->goal[2]));
  glUniformMatrix4fv(boid_model, 1, GL_FALSE, glm::value_ptr(model_mat));
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)0);
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
  case FREE:
    free_view(view);
    break;
    case FIRST_PERSON:
      first_person_view(view, f);
      break;
  }
}

void center_view(glm::mat4& view, Flock &f) {
  glm::vec3 pos = glm::vec3(0.0, 0.0, 1800) + eye_trans;
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  vec3 my_look = calc_middleway(f);
  glm::vec3 look = glm::vec3(my_look[0], my_look[1], my_look[2]);
  view = glm::lookAt(pos, look, up);
  eye_pos = pos;
  look_pos = look;
}

void side_view(glm::mat4& view, Flock &f) {
  vec3 my_pos = get_side_pos(f);
  eye_pos = glm::vec3(my_pos[0], my_pos[1], my_pos[2]) + eye_trans;

  vec3 my_look = ave_flock_center(f);
  look_pos = glm::vec3(my_look[0], my_look[1], my_look[2]);

  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(eye_pos, look_pos, up);
}

void trailing_view(glm::mat4& view, Flock &f) {
  vec3 my_pos = get_trailing_pos(f);
  glm::vec3 pos = glm::vec3(my_pos[0], my_pos[1], my_pos[2]) + eye_trans;
  vec3 my_look = ave_flock_center(f);
  glm::vec3 look = glm::vec3(my_look[0], my_look[1], my_look[2]);
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(pos, look, up);
  eye_pos = pos;
  look_pos = look;
}

void first_person_view(glm::mat4& view, Flock &f) {
  vec3 boid_pos = f.pos->at(0);
  eye_pos = glm::vec3(boid_pos[0], boid_pos[1], boid_pos[2]) + eye_trans;
  vec3 my_look = get_first_person_center_pos(f);
  glm::vec3 look_pos = glm::vec3(my_look[0], my_look[1], my_look[2]);
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(eye_pos, look_pos, up);
}

void free_view(glm::mat4& view) {
  eye_pos = glm::vec3(sin(free_view_angle) * FREE_VIEW_RADIUS, cos(free_view_angle) * FREE_VIEW_RADIUS, 7000);
  look_pos = glm::vec3(0.0, 0.0, 7000);
  glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
  view = glm::lookAt(eye_pos, look_pos, up);
  free_view_angle += 0.005;
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
    m = calc_a_thirdway(f);
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

vec3 get_first_person_center_pos(Flock& f) {
  vec3 boid_pos = f.pos->at(0);
  vec3 boid_v = vec3::normalize(f.vel->at(0));
  vec3 up = vec3(0.0, 0.0, 1.0);
  vec3 to_right = vec3::normalize(vec3::cross(boid_v, up)) * left_right;
  vec3 look_v = vec3::normalize(boid_v + to_right * 0.1);
  vec3 center = boid_pos + look_v * 10000;
  center[2] = up_down * 50;
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
  at_night = (frame_counter >= 5400 && frame_counter < 7200);
}

void update_light_position() {
  if (frame_counter >= 0 && frame_counter < 5400) {
    light_position[0] += WORLD_SIZE / 5400.0;
  } else {
    light_position[0] -= WORLD_SIZE / 1800.0;
  }
}

void zoom_in() {
  glm::vec3 look_direction = look_pos - eye_pos;
  if (glm::length(look_direction) > 500) {
    eye_trans += 50.0f * glm::normalize(look_direction);
  }
  cout << glm::length(look_direction) << endl;
  cout << "eye pos: " << eye_pos[0] << " " << eye_pos[1] << " " << eye_pos[2] << endl;
  cout << "eye trans: " << eye_trans[0] << " " << eye_trans[1] << " " << eye_trans[2] << endl;
}

void zoom_out() {
  glm::vec3 look_direction = look_pos - eye_pos;
  if (glm::length(look_direction) < 20000) {
    eye_trans -= 50.0f * glm::normalize(look_direction);
  }
  // view_mat = glm::lookAt(eye_pos, look_pos, glm::vec3(0.0, 0.0, 1.0));
  cout << glm::length(look_direction) << endl;
  cout << "eye pos: " << eye_pos[0] << " " << eye_pos[1] << " " << eye_pos[2] << endl;
  cout << "eye trans: " << eye_trans[0] << " " << eye_trans[1] << " " << eye_trans[2] << endl;
}

void print_step_msg(Flock* f) {
  cout << "*******************************************************************************" << endl;
  f->print_boids();
  cout << "-------------------------------------------------------------------------------" << endl;
  f->print_goal();
  cout << "*******************************************************************************" << endl;
}
