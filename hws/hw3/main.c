#include "boid.h"
#include "linkedlist.h"

void init() {
  glPointSize(10);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 0.000001, 10);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);

  angle = 0;
  count = BOID_COUNT;
  calc_checkerboard_vertices(SIDES, 20000);
  calc_checkerboard_indices(SIDES);
  calc_checkerboard_colors(SIDES);
 
  /*print checkerboard data
  int i;
  for(i = 0; i < pow(SIDES + 1, 2); i++) {
    printf("No.%d: r %f, g %f, b %f\n",i, board_colors[i][0], board_colors[i][1], board_colors[i][2]);
  }
  for(i = 0; i < pow(SIDES, 2); i++ ) {
    printf("index No.%d, value %d\n",i,  board_indices[i *4 ]);
  }
  */
}

void init_boids() {
  head = create_linkedlist();
  tail = head->next;
  for (int i = 0; i < 10; i++) {
    Boid *b = init_boid(i);
    Node *new_node = create_node(b, VAL);
    append(new_node, tail);    
  }
}


void draw_boid(Boid* b) {
  gsl_vector *location = b->location;
  glPushMatrix();
  glScalef(0.0001, 0.0001, 0.0005);
  glTranslatef(gsl_vector_get(location, 0), gsl_vector_get(location, 1), gsl_vector_get(location, 2));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
  glPopMatrix();
 
}

void draw_boids() {
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
  glColorPointer(3, GL_FLOAT, 0, boid_colors);
  Node* current;
  current = head->next;
  while (current->type != HEAD_TAIL) {
    draw_boid(current->data);
    current = current->next;
  }
   glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void update_boids() {
  Node* current = head->next;
  while (current->type != HEAD_TAIL) {
    Boid* this_b = (Boid *)current->data;
    Boid ** knn = n_neighbours(this_b, cache, count, 5);
    update_boid(this_b, knn, g);
    current = current->next;
  }
}

void update_boid(Boid* b, Boid** neighbors, Goal g) {
  gsl_vector_add(b->location, b->velocity);
  
  gsl_vector* s = separation(b, neighbors);
  gsl_vector* c = cohesion(b, neighbors);
  gsl_vector* a = alignment(b, neighbors);
  gsl_vector* g_s = goal_seeking(g, b);
  gsl_vector_add(g_s, s);
  gsl_vector_add(g_s, c);
  gsl_vector_add(g_s, a);
  gsl_vector_scale(b->velocity, 0.99);
  gsl_vector_scale(g_s, 4);
  gsl_vector_add(b->velocity, g_s);
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
	free(cache);
	cache = cache_linkedlist(head);
        break;
      case GLFW_KEY_BACKSPACE:
	if (count > 10) {
	  delete_last(tail);
	  free(cache);
	  cache = cache_linkedlist(head);
	  count--;
	}       
        break;
      case GLFW_KEY_Q:
        glfwSetWindowShouldClose(w, TRUE);
        break;
    }
  }
  
}

void cursor(GLFWwindow* w, double xpos, double ypos) {
  x_pos = xpos;
  y_pos = ypos;
}

int main(int argc, char **argv) {
  init_boids();
  cache = cache_linkedlist(head);
  g = init_goal();
  ave_multiplier = 0.2;

  GLFWwindow *window;
  print_boids(head);
  Boid** bs= cache_linkedlist(head);
  print_boids_array(bs, 10);
  Boid** sorted = n_neighbours(bs[0], bs, 10, 5);

  print_boids_array(sorted, 5);
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
  glfwSetCursorPosCallback(window, cursor);

  init();
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    draw_boids();
    update_boids();
    draw_checkerboard();
    draw_goal(g);
    update_goal(&g);
    glLoadIdentity();
    gluLookAt(
	      0.2, 0.01, 1.5,
	      0.2, 0.01, 0,
	      0, 1, 0
	      );
    //angle += M_PI / 200;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
