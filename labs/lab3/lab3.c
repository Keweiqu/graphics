#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#endif

GLfloat cube_vertices[][3] = {
  {-0.5,-0.5,0.5},
  {-0.5,0.5,0.5},
  {0.5,0.5,0.5},
  {0.5,-0.5,0.5},
  {-0.5,-0.5,-0.5},
  {-0.5,0.5,-0.5},
  {0.5,0.5,-0.5},
  {0.5,-0.5,-0.5}
};


GLfloat cube_colors[][3] ={
  {0.0,0.0,0.0},
  {1.0,0.0,0.0},
  {1.0,1.0,0.0},
  {0.0,1.0,0.0},
  {0.0,0.0,1.0},
  {1.0,0.0,1.0},
  {1.0,1.0,1.0},
  {0.0,1.0,1.0}
};

GLubyte cube_indices[24] = {
  0,3,2,1,
  2,3,7,6,
  0,4,7,3,
  1,2,6,5,
  4,5,6,7,
  0,1,5,4
};

GLubyte cube_wireframe_indices[48] = {
  0, 1,
  1, 2,
  2, 3,
  3, 0,
  6, 5, 
  5, 1,
  1, 2, 
  2, 6,
  7, 4,
  4, 5,
  5, 6,
  6, 7,
  7, 3, 
  3, 0,
  0, 4,
  4, 7,
  4, 0,
  0, 1,
  1, 5, 
  5, 4,
  3, 7,
  7, 6, 
  6, 2,
  2, 3
};


void draw_cube() {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
  glColorPointer(3, GL_FLOAT, 0, cube_colors);
  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_wireframe_cube() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
  glColor3f(1,1,1);
  glDrawElements(GL_LINES, 48, GL_UNSIGNED_BYTE, cube_wireframe_indices);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void init() {
  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 0.4, 20);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
}


void draw_cube_wireframe(){
  glDisableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
}

void reshape(GLFWwindow *w, int width, int height){  
  printf("Got a Reshape Event");
  printf(":\t width = %d height = %d\n", width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 0.4, 20);
  glMatrixMode(GL_MODELVIEW);

  // Set the new viewport size
  glViewport(0, 0, width, height);
}

int main(int argc, char **argv) {
    GLFWwindow* window;

    // Initialize the library 
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Create a window and its OpenGL context 
  window = glfwCreateWindow(250, 250, "Hello GL!", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 100, 100);
  // Make the window's context current 
  glfwMakeContextCurrent(window);

 // Assign reshape() to be the function called whenever 
 // a reshape event occurs
  glfwSetWindowSizeCallback(window, reshape);

  init();
  // Loop until the user closes the window 
  while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 1, 1, 0.5, 0.5 ,0.5, 0, 1, 0);
    draw_cube();
    draw_wireframe_cube();
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
