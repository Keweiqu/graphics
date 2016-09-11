

#include "main.h"

void init() {
  // Set the clear color to white
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

int main() {
  Alien alien_array[N_ROW][N_COL]; 
  init_alien_array(alien_array);
  GLFWwindow* window;
  if (!glfwInit())
    exit(EXIT_FAILURE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Homework 2", NULL, NULL);
  if(!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 100, 100);
  // Make the window's context current 
  glfwMakeContextCurrent(window);

  init();
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_alien_array(alien_array);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init_alien_array(Alien (*alien_array)[N_COL]) {
  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      alien_array[i][j] = create_alien(i, j);
    }
  }
} 

void draw_alien_array(Alien (*alien_array)[N_COL]) {
  int i, j;
  for(i = 0; i < N_ROW; i++) {
    for(j = 0; j < N_COL; j++) {
      draw_alien(alien_array[i][j]);
    }
  }
}

void draw_alien(Alien a) {
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  glVertex2f(a.x_coord - 0.05, a.y_coord - 0.05);
  glVertex2f(a.x_coord - 0.05, a.y_coord + 0.05);
  glVertex2f(a.x_coord + 0.05, a.y_coord + 0.05);
  glVertex2f(a.x_coord + 0.05, a.y_coord - 0.05);
  glEnd();
}

Alien create_alien(int row, int col) {
  Alien a;
  a.alive = TRUE;
  a.x_coord = (col - 5) * 0.13;
  a.y_coord = 0.9 - row * 0.13;
  return a;
}
