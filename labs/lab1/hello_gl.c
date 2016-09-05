// OpenGL Tutorial
// hello_gl.c

/*************************************************************************
This program essentially opens a window, clears it to default background
color and draws a square. 
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif

void init() {
  // set the background color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Set the drawing color to white
  glColor3f(1.0, 1.0, 1.0);    
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

  init();

  // Loop until the user closes the window 
  while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Specify which primitive type is to be drawn
    glBegin(GL_POLYGON);
    // Specify verticies in quad in ccw order
    glVertex2f(-.5, -.5);
    glVertex2f(.5, -.5);
    glVertex2f(.5, .5);
    glVertex2f(-.5, .5);
    glEnd();
        
    // swap the front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
 
