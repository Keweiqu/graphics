#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif

#define WIDTH 500
#define HEIGHT 500
#define SCALE_X_AXIS(x) x / WIDTH * 2 - 1
#define SCALE_Y_AXIS(y) (y / HEIGHT * 2 - 1) * -1

double mouseX, mouseY;
void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glColor3f(0.0, 0.0, 1.0);
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
  double xpos, ypos;
  printf("Got a Mouse %s Event", action==GLFW_PRESS?"press":"release");
  glfwGetCursorPos(window, &xpos, &ypos);
  printf(":\t x = %f, y = %f, button = ", xpos, ypos);
  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT:
    mouseX = SCALE_X_AXIS(xpos);
    mouseY = SCALE_Y_AXIS(ypos);
    printf("left\n");
    printf("mouseX %f", mouseX);
    printf("mouseY %f", mouseY);
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

// void drawTriangle()

int main(int argc, char **argv) {
  GLFWwindow* window;

  // Initialize the library 
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Create a window and its OpenGL context 
  window = glfwCreateWindow(500, 500, "Triangle!", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(window, 100, 100);

  // Make the window's context current 
  glfwMakeContextCurrent(window);

  // Register callbacks
  glfwSetMouseButtonCallback(window, mouse);

  init();

 while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    // Specify which primitive type is to be drawn
    glBegin(GL_TRIANGLES);
    // Specify verticies in quad in ccw order
    glVertex2f(mouseX - 0.5, mouseY - 0.5);
    glVertex2f(mouseX + 0.5, mouseY - 0.5);
    glVertex2f(mouseX + 0.5, mouseY + 0.5);
    glEnd();
        
    // swap the front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);

}
