#include "util.hpp"

GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
GLshort board_indices[SIDES * SIDES * 6];

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
    board_vertices[i][2] = 400; //z
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
//  if (n % 2 == 0) {
//    for (int i = 0; i < (n+1)*(n+1); i++) {
//      if (i % 2) {
//        board_colors[i][0] = 1.0;
//        board_colors[i][1] = 1.0;
//        board_colors[i][2] = 1.0;
//        board_colors[i][3] = 1.0;
//      } else {
//        board_colors[i][0] = 0.0;
//        board_colors[i][1] = 0.0;
//        board_colors[i][2] = 0.0;
//        board_colors[i][3] = 1.0;
//      }
//    }
//  } else {
//    for (int i = 0; i < (n+1)*(n+1); i++) {
//      if ((i % 2 == 1 && (i / (n+1)) % 2 == 1) || (i % 2 == 0 && (i / (n+1)) % 2 == 0)) {
//        board_colors[i][0] = 1.0;
//        board_colors[i][1] = 1.0;
//        board_colors[i][2] = 1.0;
//        board_colors[i][3] = 1.0;
//      } else {
//        board_colors[i][0] = 0.0;
//        board_colors[i][1] = 0.0;
//        board_colors[i][2] = 0.0;
//        board_colors[i][3] = 1.0;
//      }
//    }
//  }
//  for (int i = 0; i < SIDES * SIDES; i++) {
//    if ((i / 6) % 2 == 0) {
//      board_colors[i][0] = 0.0;
//      board_colors[i][1] = 0.0;
//      board_colors[i][2] = 0.0;
//      board_colors[i][3] = 1.0;
//    } else {
//      board_colors[i][0] = 1.0;
//      board_colors[i][1] = 1.0;
//      board_colors[i][2] = 1.0;
//      board_colors[i][3] = 1.0;
//    }
//  }
}

extern glm::mat4 view, project;
extern glm::vec3 world_scale;

void draw_checkerboard(Flock* f, GLuint matrix, GLuint vao, GLuint index) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
  
//  for(int i = 0; i < f->count; i++) {
    glm::mat4 result = glm::mat4(1.0);
    result = project;
    scalef(world_scale[0], world_scale[1], world_scale[2], &result);
    result = result * view;
//    translatef((*f->pos_x)[i], (*f->pos_y)[i], (*f->pos_z)[i], &result);
    glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
    glDrawElements(GL_TRIANGLES, SIDES * SIDES * 6, GL_UNSIGNED_SHORT, (void*)0);
    
//  }
}

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
