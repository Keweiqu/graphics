#include <iostream>
#include <string>
#include <stdio.h>
#include "common.hpp"
#include "gl_replace.hpp"


using namespace std;
glm::mat4 model = glm::mat4(1.0);

GLfloat vertices[6][3] = {
  {1.0, -1.0, -0.0},
  {0.0, 1.0, -0.0},
  {-1.0, -1.0, -0.0},
  {0.7, 0.2, -0.2},
  {0.0, 0.4, -1.0},
  {-0.7, 0.2, -0.2}
};

GLfloat colors[6][4] = {
  {0.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0, 1.0},
  {1.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 1.0, 1.0},
  {0.0, 1.0, 1.0, 1.0}
};

GLuint vbo1, vbo2, vao, program, pos, color, modelView;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void init() {
  glEnable(GL_DEPTH_TEST);
  vbo1 = make_bo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
  vbo2 = make_bo(GL_ARRAY_BUFFER, colors, sizeof(colors));
  program = initshader("lab_vs.glsl", "lab_fs.glsl");
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

  modelView = glGetUniformLocation(program, "M");
  glClearColor(1.0, 1.0, 1.0, 1.0);
}


int main(int argc, char** argv) {
  glm::vec3 trans = glm::vec3(0.0, 0.0, 0.0);
  glm::mat4 foo = glm::translate(model, trans);
  glm::mat4 bar = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(0.0);
  lookat(0, 0.0, 0.5, 0, 0.5, 0, 0, 1, 0, &view);
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
  
  GLFWwindow *window = glfwCreateWindow(512, 512, "Triangle", NULL, NULL);
  if(!window) {
    cerr << "Error: Cannot open window with GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();
  
  init();
  GLfloat x = 0;
  GLfloat y = 1;
  GLfloat z = 0;
  GLfloat xx = 0;
  GLfloat yy = 0;
  GLfloat zz = 0;
  GLfloat xxx = 1;
  GLfloat angle = 0;
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    foo = view * foo;
    //translatef(xx, yy, zz, &foo);
    xx += 0.01;
    yy += 0.01;
    //rotatef(angle, x, y, z, &foo);
    angle += 2;
    if(angle > 360) {
      angle -= 360;
    }
    //scalef(xxx, xxx, xxx, &foo);
    xxx *= 0.98;
    //z -= 0.01;
    glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(foo));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
    glfwPollEvents();
    foo = glm::mat4(1.0);
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
