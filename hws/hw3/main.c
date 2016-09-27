#include "boid.h"
#include "linkedlist.h"

void init() {
  glPointSize(10);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 0.1, 20);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);

  angle = 0;
  count = BOID_COUNT;
  calc_checkerboard_vertices(SIDES, 20000);
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

void init_boids() {
  head = create_linkedlist();
  tail = head->next;
  for (int i = 0; i < 11; i++) {
    Boid *b = init_boid(i+1);
    Node *new_node = create_node(b, VAL);
    append(new_node, tail);    
  }
}


void draw_boid(Boid* b) {
  gsl_vector *location = b->location;
  gsl_vector *velocity = b->velocity;
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
  glColorPointer(3, GL_FLOAT, 0, boid_colors);
  glPushMatrix();
  glTranslatef(gsl_vector_get(location, 0)/200, gsl_vector_get(location, 1)/200, gsl_vector_get(location, 2)/200);  
  glScalef(0.005, 0.005, 1);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
  glPopMatrix();
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void draw_boids() {
  Node* current;
  current = head->next;
  while (current->next->type != HEAD_TAIL) {
    draw_boid(current->data);
    current = current->next;
  }
}

void draw_checkerboard() {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glPushMatrix();
  glScalef(0.0001,0.0001,1);
  glVertexPointer(3, GL_FLOAT, 0, board_vertices);
  glColorPointer(3, GL_FLOAT, 0, board_colors);
  glDrawElements(GL_QUADS, SIDES*SIDES*4, GL_UNSIGNED_SHORT, board_indices);
  glPopMatrix();
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
  if (n % 2 == 0) {
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
  } else {
    printf("yay");
    for (int i = 0; i < (n+1)*(n+1); i++) {
      if ((i % 2 == 1 && (i / (n+1)) % 2 == 1) || (i % 2 == 0 && (i / (n+1)) % 2 == 0)) {
	printf("white vertex: %d\n", i);
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
}

void add_boid() {
  Boid *b = init_boid(count+1);
  Node *new_node = create_node(b, VAL);
  append(new_node, tail);
  count++;
}

void keyboard(GLFWwindow *w, int key, int scancode,  int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
        add_boid();
        break;
      case GLFW_KEY_BACKSPACE:
	if (count > 10) {
	  delete_last(tail);
	}       
	count--;
        break;
      case GLFW_KEY_Q:
        glfwSetWindowShouldClose(w, TRUE);
        break;
    }
  }
  
}

int main(int argc, char **argv) {
  Goal g = init_goal();
  init_boids();
  GLFWwindow *window;
  print_boids(head);
  GLfloat dist = get_dist((Boid *)head->next->data, (Boid *)head->next->next->data);
  printf("Dist is %f\n", dist);
  
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
  glfwSetKeyCallback(window, keyboard);
  /* glfwSetMouseButtonCallback(window, mouse); */
  /* glfwSetCursorPosCallback(window, cursor); */

  init();
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    draw_boids();
    draw_checkerboard();
    draw_goal(g);
    update_goal(&g);
    glLoadIdentity();
    gluLookAt(
	      1.5 * sin(angle), -1.5 * cos(angle), 2,
	      0, 0, 0,
	      0, 0, 1
	      );
    //angle += M_PI / 200;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
