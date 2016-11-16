#include "util.hpp"

GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
GLshort board_indices[SIDES * SIDES * 6];
View v;
extern mat4 view;
extern Flock f;
extern enum VIEW_TYPE v_mode;
extern GLuint t, program;
extern float glTime;

/*
 * calculate all vertices coordinates for checkerboard.
 * @param n n * n checkerboard
 * @param len side-length of the checkboard
 */
void calc_checkerboard_vertices(int n, GLfloat len) {
  GLfloat lx = -len / 2, ly = len / 2;
  GLfloat num_of_points = pow(n + 1, 2);
  for (int i = 0; i < num_of_points; i++) {
    GLfloat xpos = lx + (i % (n + 1)) * (len / n);
    GLfloat ypos = ly - (i / (n + 1)) * (len / n);
    board_vertices[i][0] = xpos;
    board_vertices[i][1] = ypos;
    board_vertices[i][2] = 0; //z
  }
}

/*
 * Calculate the indices for each square.
 */
void calc_checkerboard_indices(int n) {
  int i;
  for (i = 0; i < pow(n, 2); i++) {
    board_indices[6 * i] = (i / n) * (n + 1) + i % n + 1;
    board_indices[6 * i + 1] = (i / n) * (n + 1) + i % n;
    board_indices[6 * i + 2] = (i / n) * (n + 1) + i % n + n + 1;
    board_indices[6 * i + 3] = (i / n) * (n + 1) + i % n + n + 2;
    board_indices[6 * i + 4] = (i / n) * (n + 1) + i % n + 1;
    board_indices[6 * i + 5] = (i / n) * (n + 1) + i % n + n + 1;
  }
}

void calc_checkerboard_colors(int n) {
  if (n % 2 == 0) {
    for (int i = 0; i < (n+1)*(n+1); i++) {
      if (i % 2 == 1) {
        board_colors[i][0] = 0.8;
        board_colors[i][1] = 0.8;
        board_colors[i][2] = 0.8;
        board_colors[i][3] = 1.0;
      } else {
        board_colors[i][0] = 0.2;
        board_colors[i][1] = 0.2;
        board_colors[i][2] = 0.2;
        board_colors[i][3] = 1.0;
      }
    }
  } else {
    for (int i = 0; i < (n+1)*(n+1); i++) {
      if ((i / (n+1)) % 2 == 1) {
        if (i % 2 == 0) {
          board_colors[i][0] = 0.8;
          board_colors[i][1] = 0.8;
          board_colors[i][2] = 0.8;
          board_colors[i][3] = 1.0;
        } else {
          board_colors[i][0] = 0.2;
          board_colors[i][1] = 0.2;
          board_colors[i][2] = 0.2;
          board_colors[i][3] = 1.0;
        }
      } else {
        if (i % 2 == 1) {
          board_colors[i][0] = 0.8;
          board_colors[i][1] = 0.8;
          board_colors[i][2] = 0.8;
          board_colors[i][3] = 1.0;
        } else {
          board_colors[i][0] = 0.2;
          board_colors[i][1] = 0.2;
          board_colors[i][2] = 0.2;
          board_colors[i][3] = 1.0;
        }
      }
    }
  }
}

extern mat4 view, project;

void draw_checkerboard(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

  mat4 result;
  result = project;
  result = result * view;
  glUniformMatrix4fv(matrix, 1, GL_FALSE, result.data);

  glDrawElements(GL_TRIANGLES, SIDES * SIDES * 6, GL_UNSIGNED_SHORT, (void*)0);
}

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

  for(int i = 0; i < f->count; i++) {
    mat4 result;
    result = result * project;

    result = result * view;
    my_translatef((*f->pos)[i][0], (*f->pos)[i][1], (*f->pos)[i][2], result);
    GLfloat xy_angle = (atan2((*f->vel)[i][1], (*f->vel)[i][0]) + 1.5708 * 3) * 180.0 / 3.1415926;
    my_rotatef(xy_angle, 0, 0, 1, result);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, result.data);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
    update_time(i);

  }
}

void draw_shadows(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

  for (int i = 0; i < f->count; i++) {
    mat4 result;
    result = project;
    result = result * view;

    my_translatef((*f->pos)[i][0], (*f->pos)[i][1], (*f->pos)[i][2] * 0, result);
    GLfloat xy_angle = (atan2((*f->vel)[i][1], (*f->vel)[i][0]) + 1.5708 * 3) * 180.0 / 3.1415926;
    my_rotatef(xy_angle, 0, 0, 1, result);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, result.data);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  }
}


void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  mat4 result;
  result = project;
  result = result * view;

  my_translatef(f->goal[0], f->goal[1], f->goal[2], result);
  glUniformMatrix4fv(matrix, 1, GL_FALSE, result.data);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)0);
}

void update_view(mat4 &view, Flock& f) {

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

void center_view(mat4& view, Flock &f) {
  View v;
  v.pos = vec3(0.0, 0.0, 1800);
  v.up = vec3(0.0, 0.0, 1.0);
  v.look = calc_middleway(f);
  my_lookat(v.pos[0], v.pos[1], v.pos[2], v.look[0], v.look[1], v.look[2], v.up[0], v.up[1], v.up[2], view);
}

void side_view(mat4& view, Flock &f) {
  View v;
  v.pos = get_side_pos(f);
  v.look = calc_middleway(f);
  v.up = vec3(0.0, 0.0, 1.0);
  my_lookat(v.pos[0], v.pos[1], v.pos[2], v.look[0], v.look[1], v.look[2], v.up[0], v.up[1], v.up[2], view);
}

void trailing_view(mat4& view, Flock &f) {
  View v;
  v.pos = get_trailing_pos(f);
  v.look = calc_middleway(f);
  v.up = vec3(0.0, 0.0, 1.0);
  my_lookat(v.pos[0], v.pos[1], v.pos[2], v.look[0], v.look[1], v.look[2], v.up[0], v.up[1], v.up[2], view);
}

void first_person_view(mat4& view, Flock &f) {
  View v;
  v.pos = f.pos->at(0);
  v.look = get_center_pos(f);
  v.up = vec3(0.0, 0.0, 1.0);
  my_lookat(v.pos[0], v.pos[1], v.pos[2], v.look[0], v.look[1], v.look[2], v.up[0], v.up[1], v.up[2], view);
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
  t = glGetUniformLocation(program, "time");
  glUniform1f(t, glTime);
}

void update_time(int index) {
  glTime = (sin(glfwGetTime() * 10 + (*(f.seed))[index]) + 1) / 2;
  glUniform1f(t, glTime);
}

void print_step_msg(Flock* f) {
  cout << "*******************************************************************************" << endl;
  f->print_boids();
  cout << "-------------------------------------------------------------------------------" << endl;
  f->print_goal();
  cout << "*******************************************************************************" << endl;
}
