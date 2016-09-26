#include "boid.h"

void init() {
  glPointSize(10);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 0.1, 10);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);

  angle = 0;
  calc_checkerboard_vertices(SIDES, 2);
  calc_checkerboard_indices(SIDES);
  calc_checkerboard_colors(SIDES);
   int i;
  for(i = 0; i < pow(SIDES + 1, 2); i++) {
    printf("No.%d: r %f, g %f, b %f\n",i, board_colors[i][0], board_colors[i][1], board_colors[i][2]);
  }
  for(i = 0; i < pow(SIDES, 2); i++ ) {
    printf("index No.%d, value %d\n",i,  board_indices[i *4 ]);
  }
}

void draw_checkerboard() {
  glColor3f(1.0, 1.0, 1.0);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, board_vertices);
  glColorPointer(3, GL_FLOAT, 0, board_colors);
  glDrawElements(GL_QUADS, SIDES*SIDES*4, GL_UNSIGNED_SHORT, board_indices);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

/*
 * calculate all vertices coordinates for checkerboard.
 * @param n n * n checkerboard
 * @param len side-length of the checkboard
 */
void calc_checkerboard_vertices(int n, GLfloat len) {
  printf("calculate vertices\n");
  GLfloat lx = -len / 2, ly = len / 2;
  GLfloat num_of_points = pow(n + 1, 2); 
  for (int i = 0; i < num_of_points; i++) {
    GLfloat xpos = lx + (i % (n + 1)) * (len / n);
    GLfloat ypos = ly - (i / (n + 1)) * (len / n);
    board_vertices[i][0] = xpos;
    board_vertices[i][1] = ypos;
    board_vertices[i][2] = 0; //z
  }
}

/*
 * Calculate the indices for each square.
 */
void calc_checkerboard_indices(int n) {
  printf("calclulate indices\n");
  int i;
  for (i = 0; i < pow(n, 2); i++) {
    board_indices[4 * i] = (i / n) * (n + 1) + i % n + 1;    
    board_indices[4 * i + 1] = (i / n) * (n + 1) + i % n;
    board_indices[4 * i + 2] = (i / n) * (n + 1) + i % n + n + 1;
    board_indices[4 * i + 3] = (i / n) * (n + 1) + i % n + n + 2;
  }
}

void calc_checkerboard_colors(int n) {
  for (int i = 0; i < (n+1)*(n+1); i++) {
    if (i % 2) {
      board_colors[i][0] = 1.0;
      board_colors[i][1] = 1.0;
      board_colors[i][2] = 1.0;
    } else {
      board_colors[i][0] = 0.0;
      board_colors[i][1] = 0.0;
      board_colors[i][2] = 0.0;
    }
  }
}

int main(int argc, char **argv) {
	GLFWwindow *window;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Flocking Boids", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	/* glfwSetWindowSizeCallback(window, reshape); */
	/* glfwSetFramebufferSizeCallback(window, framebuffer_resize); */
	/* glfwSetKeyCallback(window, keyboard); */
	/* glfwSetMouseButtonCallback(window, mouse); */
	/* glfwSetCursorPosCallback(window, cursor); */

	init();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0);
		draw_checkerboard();
		glLoadIdentity();
		gluLookAt(1.5 * cos(angle), 1.5 * sin(angle), 1, 0, 0, 0, 0, 0, 1);
		//angle += M_PI / 200;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
