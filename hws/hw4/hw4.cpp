#include <iostream>
#include <string>
#include <stdio.h>
#include "common.hpp"
#include "Flock.hpp"
#include "util.hpp"

#define GOAL_DELTA 50;
using namespace std;

Flock f;

int pause = FALSE, LEFT = FALSE, RIGHT = FALSE;
int up = 0, down = 0;
enum VIEW_TYPE v_mode;
mat4 view, project;
extern GLfloat goal_vertices[24];
extern GLfloat goal_colors[8][4];
extern GLubyte goal_indices[36];
GLfloat boid_vertices[4][3] = {
  {0.0, 0.0, 0.75},
  {0.0, 10, 0.75},  
  {-10, -4, 0.75},
  {10, -4, 0.75}
};

GLfloat colors[6][4] = {
  {0.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0, 1.0},
  {1.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 1.0, 1.0},
  {0.0, 1.0, 1.0, 1.0},
};

GLubyte boid_indices[] = {
  0, 1, 2, // right wing
  3, 1, 0, // left wing
};

GLfloat b_vertices[4][3] = {
  {10, 10, 600},
  {-10, 10, 600},
  {-10, -10, 600},
  {10, -10, 600}
};

GLfloat b_colors[9][4] = {
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {1.0, 1.0, 1.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {1.0, 1.0, 1.0, 1.0},
  {0.0, 0.0, 0.0, 1.0}
};

GLubyte b_indices[] = {
  1, 0, 4,
  5, 1, 4,
  2, 1, 5,
  6, 2, 5,
  3, 2, 6,
  7, 3, 6,
  5, 4, 8,
  9, 5, 8,
  6, 5, 9,
  10, 6, 9,
  7, 6, 10,
  11, 7, 10,
  9, 8, 12,
  13, 9, 12,
  10, 9, 13,
  14, 10, 13,
  11, 10, 14,
  15, 11, 14
};

GLuint vbo3, vbo4, vao2, board_idx, board_pos, board_c;

extern GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
extern GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
extern GLshort board_indices[SIDES * SIDES * 6];
GLuint boid_vbo1, boid_vbo2, goal_vbo1, goal_vbo2, boid_vao, goal_vao, boid_idx, goal_idx;
GLuint program, pos, color, modelView;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void init_checkerboard() {
  glShadeModel(GL_FLAT);

  calc_checkerboard_vertices(SIDES, 10000);
  calc_checkerboard_indices(SIDES);
  calc_checkerboard_colors(SIDES);
  
  vbo3 = make_bo(GL_ARRAY_BUFFER, board_vertices, sizeof(board_vertices));
  vbo4 = make_bo(GL_ARRAY_BUFFER, board_colors, sizeof(board_colors));
  board_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, board_indices, sizeof(board_indices));
  
  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo3);
  board_pos = glGetAttribLocation(program, "vPos");
  glEnableVertexAttribArray(board_pos);
  glVertexAttribPointer(board_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo4);
  board_c = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(board_c);
  glVertexAttribPointer(board_c, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  boid_vbo1 = make_bo(GL_ARRAY_BUFFER, boid_vertices, sizeof(boid_vertices));
  boid_vbo2 = make_bo(GL_ARRAY_BUFFER, colors, sizeof(colors));
  goal_vbo1 = make_bo(GL_ARRAY_BUFFER, goal_vertices, sizeof(goal_vertices));
  goal_vbo2 = make_bo(GL_ARRAY_BUFFER, goal_colors, sizeof(goal_colors));
  boid_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, boid_indices, sizeof(boid_indices));
  goal_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, goal_indices, sizeof(goal_indices));
  program = initshader("hw4_vs.glsl", "hw4_fs.glsl");
  glUseProgram(program);
  glGenVertexArrays(1, &boid_vao);

  glBindVertexArray(boid_vao);
  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo1);
  pos = glGetAttribLocation(program, "vPos");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo2);
  color = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);


  glGenVertexArrays(1, &goal_vao);
  glBindVertexArray(goal_vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo1);
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo2);
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  init_checkerboard();
  
  modelView = glGetUniformLocation(program, "M");

  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
        f.add_boid();
        break;
      case GLFW_KEY_BACKSPACE:
        f.remove_boid();
        break;
    case 'P':
    case 'p':
      pause = !pause;
      print_step_msg(&f);
      break;
    case 'd':
    case 'D':
      print_step_msg(&f);
      if(!pause) {
	pause = TRUE;
      } else {
	f.update_centers();
	f.update_ave_v();
	f.update_goal();
	f.update_boids();
      }
      break;
    case GLFW_KEY_UP:
      up = GOAL_DELTA;
      down = 0;
      break;
    case GLFW_KEY_DOWN:
      down = GOAL_DELTA;
      up = 0;
      break;
      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(w, true);
        break;
      case GLFW_KEY_C:
        v_mode = CENTER;
        break;
      case GLFW_KEY_T:
        v_mode = TRAILING;
        break;
      case GLFW_KEY_S:
        v_mode = SIDE;
        break;
    }
  }
}

void reshape(GLFWwindow *w, int width, int height) {
  mat4 result;
  result = project;
  glUniformMatrix4fv(modelView, 1, GL_FALSE, result.data);
}

void framebuffer_resize(GLFWwindow *w, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
  v_mode = SIDE;
     my_lookat(0, 0, 1700.0, 0, 0, 0, 0, 1, 0, view);
   my_perspective(60.0, 1.0, 5.0, 21000.0, project);
   mat4::print(view);

if(!glfwInit()) {
    cerr << "Error: cannot start GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow *window = glfwCreateWindow(1000, 1000, "Triangle", NULL, NULL);
  if(!window) {
    cerr << "Error: Cannot open window with GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();
  
  glfwSetKeyCallback(window, keyboard);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);
  
  init();
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //camera_look();
    update_view(view, f);
    draw_checkerboard(&f, modelView, vao2, board_idx);
    draw_goal(&f, modelView, goal_vao, goal_idx);
    draw_flock(&f, modelView, boid_vao, boid_idx);
    if(!pause) {
      f.update_centers();
      f.update_ave_v();
      f.update_goal();
      f.update_boids();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
