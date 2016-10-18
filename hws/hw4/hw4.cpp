#include <iostream>
#include <string>
#include <stdio.h>
#include "common.hpp"
#include "gl_replace.hpp"
#include "Flock.hpp"
#include "util.hpp"

using namespace std;
glm::mat4 view = glm::mat4(1.0);
glm::mat4 project = glm::mat4(1.0);
glm::vec3 world_scale = glm::vec3(0.0001, 0.0001, 0.001);
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
  {-.1, .1, 0},
  {-.1, -.1, 0},
  {.1, -.1, 0},
  {.1, .1, 0}
};

GLfloat b_colors[4][4] = {
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
};

GLfloat b_indices[4] = {
  1, 0, 2, 3
};

GLuint vbo1, vbo2, vao, idx, program, pos, color, modelView;
GLuint vbo3, vbo4, vao2, board_idx, board_pos, board_c;

extern GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
extern GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
extern GLfloat board_indices[SIDES * SIDES * NUM_SQUARE_VERTICES];
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
  calc_checkerboard_vertices(SIDES, 2 * 10000);
  calc_checkerboard_indices(SIDES);
  calc_checkerboard_colors(SIDES);
  
  vbo3 = make_bo(GL_ARRAY_BUFFER, b_vertices, sizeof(b_vertices));
  vbo4 = make_bo(GL_ARRAY_BUFFER, b_colors, sizeof(b_colors));
  board_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, b_indices, sizeof(b_indices));
  
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
  
  cout << "board vertices" << endl;
  for (int i = 0; i < (SIDES+1)*(SIDES+1); i++) {
    for (int j = 0; j < 3; j++) {
      cout << board_vertices[i][j] << " ";
    }
    cout << endl;
  }
  
  cout << "board colors" << endl;
  for (int i = 0; i < (SIDES+1)*(SIDES+1); i++) {
    for (int j = 0; j < 4; j++) {
      cout << board_colors[i][j] << " ";
    }
    cout << endl;
  }
  
  cout << "board indices" << endl;
  for (int i = 0; i < SIDES*SIDES*NUM_SQUARE_VERTICES; i++) {
    cout << board_indices[i] << " ";
  }
  cout << endl;
}

void draw_checkerboard(GLuint matrix) {
  
  glBindVertexArray(vao2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_idx);
  
  glm::mat4 result = glm::mat4(1.0);
  result = project;
  scalef(world_scale[0], world_scale[1], world_scale[2], &result);
  result = result * view;
  glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(result));
  
  glDrawElements(GL_TRIANGLES, SIDES*SIDES*4, GL_UNSIGNED_SHORT, (void*)0);
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
  
  init_checkerboard();

  glGenVertexArrays(1, &goal_vao);
  glBindVertexArray(goal_vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo1);
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo2);
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo2);
  glEnableVertexAttribArray(color);
  
  modelView = glGetUniformLocation(program, "M");

  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv) {
  Flock f;
  glm::mat4 bar = glm::mat4(1.0);
  lookat(0, 0, 800.0, 0, 0, 0, 0, 1, 0, &view, &project);
  //scalef(world_scale[0], world_scale[1], world_scale[2], &view);
  project = glm::perspective(30.0 / 180.0 * 3.1415, 1.0, 0.0000001, 10.0);
  print_mat(view);
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
  
  init();
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_checkerboard(modelView);

    //glBindVertexArray(goal_vao);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, goal_idx);
    //glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(project));
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*) 0);
    draw_goal(&f, modelView, goal_vao, goal_idx);
    draw_flock(&f, modelView, boid_vao, boid_idx);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
