// OpenGL Tutorial
// simple.c

/*************************************************************************
This program essentially opens a window, clears it to default background
color. It then sits in an infinite loop.
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif


int main(int argc, char **argv) {
  GLFWwindow* window;

  glfwInit();

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(250, 250, "Hello GL!", NULL, NULL);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}
 
