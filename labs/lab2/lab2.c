// OpenGL Tutorial
// events.c 
/*************************************************************************
This example shows how to accept and handle events.  The user should try 
produce each of the following events: key press, mouse movement, mouse 
button press, mouse button release, reshape window, and expose window.
*************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif

#define TRUE 1
#define FALSE 0

GLfloat angle;
int is_rotating;
void init() {
  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(key) {
  case 'a':
  case 'A':
    if(action == GLFW_PRESS)
      is_rotating = is_rotating == TRUE? FALSE: TRUE;
  }
}

GLfloat incr_angle(GLfloat angle) {
  angle = angle + 2.0 < 360 ?: angle + 2.0 - 360;
  return angle;
}

int main(int argc, char **argv) {
  is_rotating = FALSE;
  angle = 0;
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


  // Assign keyboard() to be the function called whenever
  // a key is pressed or released
  glfwSetKeyCallback(window, keyboard);

  init();
  // Loop until the user closes the window 
  glMatrixMode(GL_MODELVIEW);
  while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    glRotatef(angle, 0, 0, 1);
    glBegin(GL_POLYGON);
    // Specify verticies in quad in ccw order
    glVertex2f(-.5, -.5);
    glVertex2f(.5, -.5);
    glVertex2f(.5, .5);
    glVertex2f(-.5, .5);
    glPopMatrix();
    angle = incr_angle(angle);

    glEnd();

    // Swap front and back buffers 
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
