#include "util.hpp"

GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
GLshort board_indices[SIDES * SIDES * 6];
View v;
View center_view, trailing_view, side_view;
extern Flock f;
extern int v_mode;

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
        board_colors[i][0] = 1.0;
        board_colors[i][1] = 1.0;
        board_colors[i][2] = 1.0;
        board_colors[i][3] = 1.0;
      } else {
        board_colors[i][0] = 0.0;
        board_colors[i][1] = 0.0;
        board_colors[i][2] = 0.0;
        board_colors[i][3] = 1.0;
      }
    }
  } else {
    for (int i = 0; i < (n+1)*(n+1); i++) {
      if ((i / (n+1)) % 2 == 1) {
        if (i % 2 == 0) {
          board_colors[i][0] = 1.0;
          board_colors[i][1] = 1.0;
          board_colors[i][2] = 1.0;
          board_colors[i][3] = 1.0;
        } else {
          board_colors[i][0] = 0.0;
          board_colors[i][1] = 0.0;
          board_colors[i][2] = 0.0;
          board_colors[i][3] = 1.0;
        }
      } else {
        if (i % 2 == 1) {
          board_colors[i][0] = 1.0;
          board_colors[i][1] = 1.0;
          board_colors[i][2] = 1.0;
          board_colors[i][3] = 1.0;
        } else {
          board_colors[i][0] = 0.0;
          board_colors[i][1] = 0.0;
          board_colors[i][2] = 0.0;
          board_colors[i][3] = 1.0;
        }
      }
    }
  }
}

extern glm::mat4 view, project;
extern glm::vec3 world_scale;

void draw_checkerboard(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

  glm::mat4 result = glm::mat4(1.0);
  result = project;
  result = result * view;
  glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
  glDrawElements(GL_TRIANGLES, SIDES * SIDES * 6, GL_UNSIGNED_SHORT, (void*)0);
}

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  
  for(int i = 0; i < f->count; i++) {
    glm::mat4 result = glm::mat4(1.0);
    result = result * project;
    
    result = result * view;
    translatef((*f->pos)[i][0], (*f->pos)[i][1], (*f->pos)[i][2], &result);
    GLfloat xy_angle = (atan2((*f->vel)[i][1], (*f->vel)[i][0]) + 1.5708 * 3) * 180.0 / 3.1415926;
    rotatef(xy_angle, 0, 0, 1, &result);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);

  }
}


void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  glm::mat4 result = glm::mat4(1.0);
  result = project;
  result = result * view;

  translatef(f->goal[0], f->goal[1], f->goal[2], &result);
  glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)0);
}

void init_views() {
  init_center_view();
  init_trailing_view();
  init_side_view();
  v = center_view;
}

void update_view() {
  switch(v_mode) {
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
  center_view.pos = vec3(0.0, 0.0, 700);
  center_view.up = vec3(0.0, 0.0, 1.0);
  center_view.look = calc_middleway();
}

void init_trailing_view() {
  trailing_view.pos = trailing_position();
  trailing_view.look = calc_middleway();
  trailing_view.up = vec3(0.0, 0.0, 1.0);
}

void init_side_view() {
  side_view.pos = side_position();
  side_view.look = calc_middleway();
  side_view.up = vec3(0.0, 0.0, 1.0);
}

void update_center_view() {
  center_view.look = calc_middleway();
  v = center_view;
}

void update_trailing_view() {
  trailing_view.pos = trailing_position();
  trailing_view.look = calc_middleway();
}

void update_side_view() {
//  side_view
}

void camera_look() {
  lookat(
    v.pos[0], v.pos[1], v.pos[2],
    v.look[0], v.look[1], v.look[2],
    v.up[0], v.up[1], v.up[2],
    &view
  );
}

vec3 calc_middleway() {
  vec3 ave_center = ave_flock_center();
  return (ave_center + f.goal) / 2;
}

vec3 trailing_position() {
  vec3 res = ave_flock_center();;
  vec3 z = vec3(0.0, 0.0, 1.0);
  vec3 c = ave_flock_center();
  vec3 u = f.goal - c;
  GLfloat d = center_goal_dist();
  GLfloat r = max_boid_goal_dist();
  res = res + u * -1 * (d + 5 * r);
  res = res + z * (d + r);
  return res;
}

vec3 side_position() {
  vec3 m = calc_middleway();
  vec3 c = ave_flock_center();
  vec3 u = f.goal - c;
  vec3 z = vec3(0.0, 0.0, 1.0);
  vec3 p = vec3::cross(u * -1, z);
  GLfloat d = center_goal_dist();
  GLfloat r = max_boid_goal_dist();
  p = vec3::normalize(p);
  p = p * (d + 2 * r);
  p = m + p;
  p = p + vec3(0.0, 0.0, d + r);
  return p;
}

GLfloat max_boid_goal_dist() {
  GLfloat max = 0;
  for (int i = 0; i < f.count; i++) {
    GLfloat dist = ((*(f.pos))[i] - f.goal).len();
    if (dist > max) {
      max = dist;
    }
  }
  return max;
}

vec3 ave_flock_center() {
  int num1 = 0, num2 = 0;
  for (int i = 0 ; i < f.group->size(); i++) {
    if ((*(f.group))[i] == 0) {
      num1++;
    } else {
      num2++;
    }
  }
  vec3 center = (f.center[0] * num1 + f.center[1] * num2) / (num1 + num2);
  return center;
}

GLfloat center_goal_dist() {
  vec3 center = ave_flock_center();
  return (center - f.goal).len();
}