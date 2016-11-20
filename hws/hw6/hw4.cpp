#include "hw4.hpp"

Flock f;
int pause = FALSE, to_left = FALSE, to_right = FALSE;
int up = FALSE, down = FALSE;
float glTime;
enum VIEW_TYPE v_mode;
glm::mat4 model_mat, view_mat, project_mat;

GLfloat eye_dist = INITIAL_EYE_DIST;
int clicked = FALSE;
meshManager mesh;

extern GLfloat goal_vertices[24];
extern GLfloat goal_colors[8][4];
extern GLubyte goal_indices[36];
GLfloat boid_vertices[4][3] = {
  {0.0, 0.0, 0.75},
  {0.0, 10, 0.75},
  {-10, -4, 0.75},
  {10, -4, 0.75}
};

GLfloat boid_flap_vertices[4][3] = {
  {0.0, 0.0, 0.75},
  {0.0, 10, 0.75},
  {-8, -3.5, 14.75},
  {8, -3.5, 14.75}
};

GLubyte boid_indices[] = {
  0, 1, 2, // right wing
  3, 1, 0, // left wing
};

extern GLfloat ocean_vertices[18];
extern GLfloat ocean_tex_coords[12];
extern GLfloat ocean_normals[3];

GLuint boid_vao;
GLuint boid_vbo1, boid_vbo3, boid_idx;
GLuint vao2;
GLuint ocean_vbo_pos, ocean_vbo_tex, ocean_vbo_normal, ocean_pos, ocean_normal, ocean_texc, ocean_tex_sampler;
GLuint program, pos, pos1, model, view, project;
GLuint t;
GLuint texture;
Image ocean;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void read_images() {
  if (!read_ppm("glass.ppm", &ocean)) {
    cout << "Fail to read image for ocean\n" << endl;
    exit(EXIT_FAILURE);
  }
}

void init_ocean() {
  calc_ocean_vertices(WORLD_SIZE * 2);

  ocean_vbo_pos = make_bo(GL_ARRAY_BUFFER, ocean_vertices, sizeof(ocean_vertices));
  ocean_vbo_tex = make_bo(GL_ARRAY_BUFFER, ocean_tex_coords, sizeof(ocean_tex_coords));
  ocean_vbo_normal = make_bo(GL_ARRAY_BUFFER, ocean_normals, sizeof(ocean_normals));

  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_pos);
  ocean_pos = glGetAttribLocation(program, "vPos0");
  glEnableVertexAttribArray(ocean_pos);
  glVertexAttribPointer(ocean_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  ocean_pos = glGetAttribLocation(program, "vPos1");
  glEnableVertexAttribArray(ocean_pos);
  glVertexAttribPointer(ocean_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_normal);
  ocean_normal = glGetAttribLocation(program, "vNormal");
  glEnableVertexAttribArray(ocean_normal);
  glVertexAttribPointer(ocean_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_tex);
  ocean_texc = glGetAttribLocation(program, "vTex");
  glEnableVertexAttribArray(ocean_texc);
  glVertexAttribPointer(ocean_texc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  read_images();
  boid_vbo1 = make_bo(GL_ARRAY_BUFFER, boid_vertices, sizeof(boid_vertices));
  boid_vbo3 = make_bo(GL_ARRAY_BUFFER, boid_flap_vertices, sizeof(boid_flap_vertices));
  boid_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, boid_indices, sizeof(boid_indices));
  
  program = initshader("hw4_vs.glsl", "hw4_fs.glsl");
  glUseProgram(program);

  //Boid
  glGenVertexArrays(1, &boid_vao);
  glBindVertexArray(boid_vao);
  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo1);
  pos = glGetAttribLocation(program, "vPos0");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo3);
  pos1 = glGetAttribLocation(program, "vPos1");
  glEnableVertexAttribArray(pos1);
  glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  init_ocean();
  init_time();

  model = glGetUniformLocation(program, "Model");
  view = glGetUniformLocation(program, "View");
  project = glGetUniformLocation(program, "Project");

  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
        f.add_boid();
        break;
      case GLFW_KEY_BACKSPACE:
        f.remove_boid();
        break;
    case 'P':
    case 'p':
      pause = !pause;
      print_step_msg(&f);
      break;
    case 'd':
    case 'D':
      print_step_msg(&f);
      if(!pause) {
	pause = TRUE;
      } else {
	f.update_centers();
	f.update_ave_v();
	f.update_goal();
	f.update_boids();
      }
      break;
    case GLFW_KEY_UP:
      up = GOAL_DELTA;
      down = 0;
      break;
    case GLFW_KEY_DOWN:
      down = GOAL_DELTA;
      up = 0;
      break;
    case GLFW_KEY_LEFT:
      to_left = TRUE;
      to_right = FALSE;
      break;
    case GLFW_KEY_RIGHT:
      to_right = TRUE;
      to_left= FALSE;
      break;
      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(w, true);
        break;
      case GLFW_KEY_C:
        v_mode = CENTER;
        break;
      case GLFW_KEY_T:
        v_mode = TRAILING;
        break;
      case GLFW_KEY_S:
        v_mode = SIDE;
        break;
      case GLFW_KEY_F:
        v_mode = FIRST_PERSON;
        break;
    case GLFW_KEY_COMMA:
      if(f.speed > 5) {
	f.speed -= 1.0;
      }
      break;
    case GLFW_KEY_PERIOD:
      if(f.speed < 20) {
	f.speed += 1.0;
      }
      break;
    }
  }
}

void reshape(GLFWwindow *w, int width, int height) {
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
}

void framebuffer_resize(GLFWwindow *w, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
  v_mode = SIDE;
  glm::vec3 eye = glm::vec3(0.0, 0.0, 1700.0);
  glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0, 1, 0);
  view_mat = glm::lookAt(eye, center, up);
  project_mat = glm::perspective(30.0 * DEGREE_TO_RADIAN, 1.0, 1.0, 100000.0);

  if(!glfwInit()) {
    cerr << "Error: cannot start GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow *window = glfwCreateWindow(1000, 1000, "Triangle", NULL, NULL);
  if(!window) {
    cerr << "Error: Cannot open window with GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwMakeContextCurrent(window);
  
  glewExperimental = GL_TRUE;
  glewInit();
  
  glfwSetKeyCallback(window, keyboard);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);
  
  
  init();
  mesh.readFiles(argc - 1, argv + 1);
  mesh.init();
  
  glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glBindVertexArray(mesh.vao);
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_view(view_mat, f);
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
    draw_ocean(vao2);
    draw_flock(&f, model, boid_vao, boid_idx);
    if(!pause) {
      f.update_centers();
      f.update_ave_v();
      f.update_goal();
      f.update_boids();
    }
    mesh.draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
