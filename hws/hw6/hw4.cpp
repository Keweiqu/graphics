#include "hw4.hpp"

Flock f;
int pause = FALSE, to_left = FALSE, to_right = FALSE;
int up = FALSE, down = FALSE;
float glTime;
enum VIEW_TYPE v_mode;
glm::mat4 model_mat, view_mat, project_mat, parallel_mat;

// coming from main.cpp
// GLuint fs_shader, phong_shader;
// GLuint model, view, project, vbo, ebo, vao, pos;
GLfloat spin[3] = {0.0f, 0.0f, 0.0f};
GLfloat scale_factor = INITIAL_SCALE_FACTOR, eye_dist = INITIAL_EYE_DIST;
bool isParallel = false; // bool isPaused = false, isParallel = false;
enum draw_mode d_mode = FACE;
enum shade_mode s_mode = SMOOTH;
int clicked = FALSE;
double m_xpos, m_ypos;
GLfloat last_x_diff, last_y_diff;
GLfloat x_diff, y_diff;
GLfloat x_angle, y_angle;
glm::mat4 universe_rotate;

meshManager mesh;
mat4 view, project;

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

GLfloat colors[6][4] = {
  {0.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0, 1.0},
  {1.0, 1.0, 0.0, 1.0},
  {1.0, 0.0, 1.0, 1.0},
  {0.0, 1.0, 1.0, 1.0},
};

GLfloat shadow_colors[6][4] = {
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
};

GLubyte boid_indices[] = {
  0, 1, 2, // right wing
  3, 1, 0, // left wing
};

GLuint vbo3, vbo4, vao2, board_idx, board_pos, board_pos1, board_c;

extern GLfloat board_vertices[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH];
extern GLfloat board_colors[(SIDES+1)*(SIDES+1)][VECTOR_LENGTH+1];
extern GLshort board_indices[SIDES * SIDES * 6];
extern GLfloat ocean_vertices[18];
extern GLfloat ocean_tex_coords[12];
extern GLfloat ocean_normals[3];
GLuint boid_vbo1, boid_vbo2, boid_vbo3, goal_vbo1, goal_vbo2, shadow_vbo, shadow_vao, boid_vao, goal_vao, boid_idx, goal_idx;
GLuint program, pos, pos1, goal_pos, goal_pos1, shadow_pos, shadow_pos1, color, shadow_color, mo, vi, pro, modelView;
GLuint ocean_vbo_pos, ocean_vbo_tex, ocean_vbo_normal, ocean_pos, ocean_normal, ocean_texc, ocean_tex_sampler;
GLuint t;
// vector<GLuint> textures;
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

void init_checkerboard() {
  glShadeModel(GL_FLAT);

  calc_checkerboard_vertices(SIDES, WORLD_SIZE * 2);
  calc_checkerboard_indices(SIDES);
  calc_checkerboard_colors(SIDES);

  vbo3 = make_bo(GL_ARRAY_BUFFER, board_vertices, sizeof(board_vertices));
  vbo4 = make_bo(GL_ARRAY_BUFFER, board_colors, sizeof(board_colors));
  board_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, board_indices, sizeof(board_indices));

  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);

  glBindBuffer(GL_ARRAY_BUFFER, vbo3);
  board_pos = glGetAttribLocation(program, "vPos0");
  glEnableVertexAttribArray(board_pos);
  glVertexAttribPointer(board_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  board_pos1 = glGetAttribLocation(program, "vPos1");
  glEnableVertexAttribArray(board_pos1);
  glVertexAttribPointer(board_pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo4);
  board_c = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(board_c);
  glVertexAttribPointer(board_c, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  read_images();
  boid_vbo1 = make_bo(GL_ARRAY_BUFFER, boid_vertices, sizeof(boid_vertices));
  boid_vbo2 = make_bo(GL_ARRAY_BUFFER, colors, sizeof(colors));
  boid_vbo3 = make_bo(GL_ARRAY_BUFFER, boid_flap_vertices, sizeof(boid_flap_vertices));

  shadow_vbo = make_bo(GL_ARRAY_BUFFER, shadow_colors, sizeof(shadow_colors));

  goal_vbo1 = make_bo(GL_ARRAY_BUFFER, goal_vertices, sizeof(goal_vertices));
  goal_vbo2 = make_bo(GL_ARRAY_BUFFER, goal_colors, sizeof(goal_colors));
  boid_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, boid_indices, sizeof(boid_indices));
  goal_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, goal_indices, sizeof(goal_indices));
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

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo2);
  color = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  // Shadow
  glGenVertexArrays(1, &shadow_vao);
  glBindVertexArray(shadow_vao);
  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo1);
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo3);
  glEnableVertexAttribArray(pos1);
  glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, shadow_vbo);
  shadow_color = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(shadow_color);
  glVertexAttribPointer(shadow_color, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  //Goal
  glGenVertexArrays(1, &goal_vao);
  glBindVertexArray(goal_vao);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo1);
  goal_pos = glGetAttribLocation(program, "vPos0");
  glEnableVertexAttribArray(goal_pos);
  glVertexAttribPointer(goal_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo1);
  goal_pos1 = glGetAttribLocation(program, "vPos1");
  glEnableVertexAttribArray(goal_pos1);
  glVertexAttribPointer(goal_pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, goal_vbo2);
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  init_ocean();
  // init_checkerboard();
  init_time();

  mo = glGetUniformLocation(program, "Model");
  vi = glGetUniformLocation(program, "View");
  pro = glGetUniformLocation(program, "Project");

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

void mouse(GLFWwindow *w, int button, int action, int mods) {
  switch(button) {
  case GLFW_MOUSE_BUTTON_LEFT:
    if(action== GLFW_PRESS){
      glfwGetCursorPos(w, &m_xpos, &m_ypos);
      printf("pressed\n");
      clicked = TRUE;
    } else {
      last_x_diff += x_diff;
      last_y_diff += y_diff;
      x_diff = 0;
      y_diff = 0;
      printf("released\n");
      clicked = FALSE;
    }
  default:
    break;
  }
}

void cursor(GLFWwindow* window, double xpos, double ypos) {
  if(clicked) {
    x_diff = (xpos - m_xpos) / DRAG_SPEED_FACTOR;
    y_diff = (ypos - m_ypos) / DRAG_SPEED_FACTOR;
    if(x_diff < 0) {
      x_diff += 2 * M_PI;
    } else if(x_diff > 2 * M_PI) {
      x_diff -= 2 * M_PI;
    }

    if(y_diff < 0) {
      y_diff += 2 * M_PI;
    } else if (y_diff > 2 * M_PI) {
      y_diff -= 2 * M_PI;
    }
  }
}

void reshape(GLFWwindow *w, int width, int height) {
  mat4 result;
  result = project;
  glUniformMatrix4fv(modelView, 1, GL_FALSE, result.data);
}

void framebuffer_resize(GLFWwindow *w, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
  v_mode = SIDE;
  my_lookat(0, 0, 1700.0, 0, 0, 0, 0, 1, 0, view);
  my_perspective(60.0, 1.0, 5.0, 21000.0, project);
  // my_lookat(0.0, 1.0, eye_dist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, view);
  // my_perspective(35 * DEGREES_TO_RADIANS, 1.0, 0.1, 1000.0, project);

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
  glfwSetMouseButtonCallback(window, mouse);
  glfwSetCursorPosCallback(window, cursor);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);


  init();
  mesh.readFiles(argc - 1, argv + 1);
  mesh.init();
  glBindVertexArray(mesh.vao);

  while(!glfwWindowShouldClose(window)) {
    GLfloat new_x = last_x_diff + x_diff;
    GLfloat new_y = last_y_diff + y_diff;
    universe_rotate =
      glm::rotate(new_y, glm::vec3(cos(new_x), 0.0, sin(-new_x))) *
      glm::rotate(new_x, glm::vec3(0.0, 1.0, 0.0));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_view(view, f);
    // draw_checkerboard(&f, modelView, vao2, board_idx);
    draw_ocean(vao2);
    draw_flock(&f, modelView, boid_vao, boid_idx);
    if(!pause) {
      f.update_centers();
      f.update_ave_v();
      f.update_goal();
      f.update_boids();
    }
    mesh.draw_default();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
