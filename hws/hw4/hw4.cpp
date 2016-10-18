#include <iostream>
#include <string>
#include <stdio.h>
#include "common.hpp"
#include "gl_replace.hpp"
#include "Flock.hpp"
#include "util.hpp"

using namespace std;
glm::mat4 model = glm::mat4(1.0);
glm::mat4 view = glm::mat4(1.0);
glm::mat4 project = glm::mat4(1.0);
glm::vec3 world_scale = glm::vec3(0.0001, 0.0001, 1);GLfloat vertices[4][3] = {
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
  {0.0, 1.0, 1.0, 1.0}
};

GLubyte boid_indices[] = {
  0, 1, 2, // right wing
  3, 1, 0, // left wing
};

GLuint vbo1, vbo2, vao, idx, program, pos, color, modelView;
GLuint vbo3, vbo4, vao2, board_idx, board_pos, board_c;

extern GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
extern GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
extern GLfloat board_indices[SIDES * SIDES * NUM_SQUARE_VERTICES];

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
  
  vbo3 = make_bo(GL_ARRAY_BUFFER, board_vertices, sizeof(board_vertices));
  vbo4 = make_bo(GL_ARRAY_BUFFER, board_colors, sizeof(board_colors));
  board_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, board_indices, sizeof(board_indices));
  
  glGenVertexArrays(1, &vao2);
  
  board_pos = glGetAttribLocation(program, "vPos");
  board_c = glGetAttribLocation(program, "vColor");
  
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 4; j++) {
      cout << board_vertices[i][j] << " ";
    }
    cout << endl;
  }
}

void draw_checkerboard() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_idx);
  glBindVertexArray(vao2);
  glBindBuffer(GL_ARRAY_BUFFER, vbo3);
  glEnableVertexAttribArray(board_pos);
  glVertexAttribPointer(board_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo4);
  glEnableVertexAttribArray(board_c);
  glVertexAttribPointer(board_c, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  
  glDrawElements(GL_QUADS, SIDES*SIDES*4, GL_UNSIGNED_SHORT, board_indices);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  vbo1 = make_bo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
  vbo2 = make_bo(GL_ARRAY_BUFFER, colors, sizeof(colors));
  idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, boid_indices, sizeof(boid_indices));
  program = initshader("hw4_vs.glsl", "hw4_fs.glsl");
  glUseProgram(program);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  pos = glGetAttribLocation(program, "vPos");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  color = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  
  init_checkerboard();

  modelView = glGetUniformLocation(program, "M");
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv) {
  Flock f;
  f.print_boids();
  f.remove_boid();
  f.print_boids();
  glm::vec3 trans = glm::vec3(0.0, 0.0, 0.0);
  glm::mat4 foo = glm::translate(model, trans);
  glm::mat4 bar = glm::mat4(1.0);
  lookat(0, 0, 0.8, 0, 0, 0, 0, 1, 0, &view);
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
    foo = project * view * foo;
    //translatef(xx, yy, zz, &foo);

    //rotatef(angle, x, y, z, &foo);

    scalef(world_scale[0], world_scale[1], world_scale[2], &foo);

    glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(foo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL);
    //glm::mat4 bar = glm::mat4(1.0);
    //glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(bar));

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (void*)3);
    
    //draw_checkerboard();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    foo = glm::mat4(1.0);
  }
  glfwTerminate();
}
