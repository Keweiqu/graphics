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

void init() {
  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void entry(GLFWwindow* window, int entered) {
  printf("Got an Window Entry Event:");
  printf(":\t %s\n", entered?"entered":"left");
 }

void reshape(GLFWwindow *w, int width, int height){  
  printf("Got a Reshape Event");
  printf(":\t width = %d height = %d\n", width, height);

  // Set the new viewport size
  glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  printf("Got a Keyboard Event: ");
  printf(" %d, ", key);
  
  switch(action) {
  case GLFW_PRESS :
    printf("pressed,\t");
    break;
  case GLFW_RELEASE :
    printf("released,\t");
    break;
  case GLFW_REPEAT :
    printf("repeated,\t");
    break;
  default :
    printf("shouldn't happen\t");
    break;
  }  

  switch(mods) {
  case GLFW_MOD_SHIFT :
    printf("shift held\n");
    break;
  case GLFW_MOD_ALT :
    printf("alt held\n");
    break;
  case GLFW_MOD_CONTROL :
    printf("ctrl held\n");
    break;
  default :
    printf("no modifiers.\n");
    break;
  }

  switch(key) {
  case GLFW_KEY_ESCAPE :
  case 'q' :
  case 'Q':
    glfwSetWindowShouldClose(w, GL_TRUE);
    break;
  default:
    break;
  }
}


void mouse(GLFWwindow* window, int button, int action, int mods) {
  double xpos, ypos;
  printf("Got a Mouse %s Event", action==GLFW_PRESS?"press":"release");
  glfwGetCursorPos(window, &xpos, &ypos);
  printf(":\t x = %f, y = %f, button = ", xpos, ypos);
  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT: 
    printf("left\n");
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    printf("right\n");
    break;
  case GLFW_MOUSE_BUTTON_MIDDLE: 
    printf("MIDDLE\n");
    break;
  default : 
    printf("none\n");
    break;
  }
}

void cursor(GLFWwindow* window, double xpos, double ypos) {
  printf("Got a Cursor Event");
  printf(":\t x = %f, y = %f\n", xpos, ypos);  
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

  // Assign entry() to be the function called whenever
  // a window entry event occurs
  glfwSetCursorEnterCallback(window, entry);

 // Assign reshape() to be the function called whenever 
 // a reshape event occurs
  glfwSetWindowSizeCallback(window, reshape);

  // Assign keyboard() to be the function called whenever
  // a key is pressed or released
  glfwSetKeyCallback(window, keyboard);

  // Assign mouse() to be the function called whenever
  // a mouse button is pressed or released
  glfwSetMouseButtonCallback(window, mouse);

  // Assign cursor() to be the function called whenever
  // a cursor moves
  glfwSetCursorPosCallback(window, cursor);

  init();
  // Loop until the user closes the window 
  while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
    // Specify verticies in quad in ccw order
    glVertex2f(-.5, -.5);
    glVertex2f(.5, -.5);
    glVertex2f(.5, .5);
    glVertex2f(-.5, .5);
    glEnd();

    // Swap front and back buffers 
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
