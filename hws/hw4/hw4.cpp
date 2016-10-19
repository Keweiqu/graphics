#include <iostream>
#include <string>
#include <stdio.h>
#include "common.hpp"
#include "gl_replace.hpp"
#include "Flock.hpp"
#include "util.hpp"
#include "vec3.hpp"


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

GLuint boid_vbo1, boid_vbo2, goal_vbo1, goal_vbo2, boid_vao, goal_vao, boid_idx, goal_idx;
GLuint program, pos, color, modelView;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
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
  
  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo2);
  glEnableVertexAttribArray(color);

  modelView = glGetUniformLocation(program, "M");

  glClearColor(1.0, 1.0, 1.0, 1.0);
}


int main(int argc, char** argv) {
  Flock f;
  glm::mat4 bar = glm::mat4(1.0);
  vec3<GLfloat> v = vec3<GLfloat>(1.0);
  cout << v;
  lookat(0, 0, 800.0, 0, 0, 0, 0, 1, 0, &view, &project);
  project = glm::perspective(glm::radians(30.0), 1.0, 0.1, 10.0);
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
    draw_goal(&f, modelView, goal_vao, goal_idx);
    draw_flock(&f, modelView, boid_vao, boid_idx);
    f.update_centers();
    f.update_ave_v();
    f.update_goal();
    f.update_boids();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
