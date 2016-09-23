// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
const int numPoints = 50000;
GLuint vao, vbo, program, pos;
GLfloat points[numPoints][2];
  
// Specifiy the vertices for a triangle
GLfloat vertices[3][2] = {{-1.0, -1.0}, {0.0, 1.0}, {1.0, -1.0}};

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void init( void ) {
  int i, j;

  // Select an arbitrary initial point inside of the triangle
  points[0][0] = 0.25; 
  points[0][1] = 0.50;

  // compute and store N-1 new points
  for (i = 1; i < numPoints; ++i) {
    j = rand() % 3;   // pick a vertex at random

    // Compute the point halfway between the selected vertex
    //   and the previous point
    points[i][0] = (points[i-1][0] + vertices[j][0]) / 2.0;
    points[i][1] = (points[i-1][1] + vertices[j][1]) / 2.0;
  }

  // Create and initialize a buffer object
  //glGenBuffers(1, &vbo);
  //glBindBuffer(GL_ARRAY_BUFFER, vbo);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
  vbo = make_bo(GL_ARRAY_BUFFER, points, sizeof(points));

  // Load shaders and use the resulting shader program
  program = initshader( "example1_vs.glsl", "example1_fs.glsl" );
  glUseProgram(program);

  // Create a vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Initialize the vertex position attribute from the vertex shader
  pos = glGetAttribLocation(program, "vPos");
  //must have vbo bound
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(w, GL_TRUE);
    break;
  }
}

int main(int argc, char **argv) {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit ()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
  } 

  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow (512, 512, "Hello Triangle", NULL, NULL);
  if (!window) {
    fprintf (stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent (window);
  
  // will segfault otherwise in init()
  glewExperimental = GL_TRUE;
  glewInit();
  
  printf("%s\n", glGetString(GL_VERSION));

  init();
  glfwSetKeyCallback(window, keyboard);

  while (!glfwWindowShouldClose (window)) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_POINTS, 0, numPoints);    // draw the points
    glfwSwapBuffers (window);
    // update other events like input handling 
    glfwPollEvents ();
  }
    
  glfwTerminate();
  return 0;
}
