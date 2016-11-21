#include "hw4.hpp"

Flock f;
int pause = FALSE, to_left = FALSE, to_right = FALSE;
int up = FALSE, down = FALSE;
float glTime, glOceanTime;
enum VIEW_TYPE v_mode;
glm::mat4 model_mat, view_mat, project_mat;
GLuint frame_counter = 0;

GLfloat eye_dist = INITIAL_EYE_DIST;
int clicked = FALSE;
meshManager terrain_mesh;

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

GLfloat boid_normals[] = {
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
};

GLfloat dawn[9] = {
  0.4, 0.4, 0.1,
  0.3, 0.3, 0.3,
  0.3, 0.3, 0.3
};

GLfloat day[9] = {
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0
};

GLfloat dusk[9] = {
  0.8, 0.2, 0.8,
  0.8, 0.5, 0.8,
  1.0, 0.5, 0.8
};

GLfloat night[9] = {
  0.0, 0.0, 0.4,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0
};

GLfloat lighting_conditions[36] = {
  // dawn
  0.4, 0.4, 0.1,
  0.3, 0.3, 0.3,
  0.3, 0.3, 0.3,

  // day
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,
  1.0, 1.0, 1.0,

  // dusk
  0.8, 0.2, 0.8,
  0.8, 0.5, 0.8,
  1.0, 0.5, 0.8,

  // night
  0.0, 0.0, 0.4,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0
};

extern GLfloat ocean_vertices[12];
extern GLfloat ocean_tex_coords[8];
extern GLubyte ocean_indices[6];
extern GLfloat ocean_normals[12];
GLuint vao2;
GLuint boid_vao, boid_vbo1, boid_vbo3, boid_idx;
GLuint program, boid_shader, pos, pos1, model, view, project;
GLuint ocean_vbo_pos, ocean_vbo_tex, ocean_vbo_index, ocean_vbo_normal, ocean_pos, ocean_normal, ocean_texc, ocean_tex_sampler0, ocean_tex_sampler1;
GLuint boid_vbo_normal, boid_vbo_tex, boid_normal, boid_texc, feather_tex_sampler, boid_view, boid_project, boid_model;
GLuint t, t2, day_time, light1, light2;
GLuint textures[4];
Image ocean0, ocean1, feather;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void read_images() {
  if (!read_ppm("blue-water3.ppm", &ocean0)) {
    cout << "Fail to read image for ocean0\n" << endl;
    exit(EXIT_FAILURE);
  }

  if (!read_ppm("blue-water4.ppm", &ocean1)) {
    cout << "Fail to read image for ocean1\n" << endl;
    exit(EXIT_FAILURE);
  }

  if (!read_ppm("feather2.ppm", &feather)) {
    cout << "Fail to read image for feather\n" << endl;
    exit(EXIT_FAILURE);
  }
}

void init_ocean() {
  calc_ocean_vertices(WORLD_SIZE * 2);

  program = initshader("hw4_vs.glsl", "hw4_fs.glsl");
  glUseProgram(program);

  ocean_vbo_pos = make_bo(GL_ARRAY_BUFFER, ocean_vertices, sizeof(ocean_vertices));
  ocean_vbo_tex = make_bo(GL_ARRAY_BUFFER, ocean_tex_coords, sizeof(ocean_tex_coords));
  ocean_vbo_index = make_bo(GL_ELEMENT_ARRAY_BUFFER, ocean_indices, sizeof(ocean_indices));
  ocean_vbo_normal = make_bo(GL_ARRAY_BUFFER, ocean_normals, sizeof(ocean_normals));
  ocean_tex_sampler0 = glGetUniformLocation(program, "ocean_tex0");
  ocean_tex_sampler1 = glGetUniformLocation(program, "ocean_tex1");
  light1 = glGetUniformLocation(program, "light1");
  light2 = glGetUniformLocation(program, "light2");
  day_time = glGetUniformLocation(program, "day_time");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3160, 2592, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean0.data);
  glUniform1i(ocean_tex_sampler0, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2804, 2592, 0, GL_RGB, GL_UNSIGNED_BYTE, ocean1.data);
  glUniform1i(ocean_tex_sampler1, 1);

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

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_pos);
  ocean_pos = glGetAttribLocation(program, "vPos1");
  glEnableVertexAttribArray(ocean_pos);
  glVertexAttribPointer(ocean_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ocean_vbo_index);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_normal);
  ocean_normal = glGetAttribLocation(program, "vNormal");
  glEnableVertexAttribArray(ocean_normal);
  glVertexAttribPointer(ocean_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, ocean_vbo_tex);
  ocean_texc = glGetAttribLocation(program, "vTex");
  glEnableVertexAttribArray(ocean_texc);
  glVertexAttribPointer(ocean_texc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}


void init_terrain() {

}

void init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  read_images();

  glGenTextures(4, textures);

  boid_vbo1 = make_bo(GL_ARRAY_BUFFER, boid_vertices, sizeof(boid_vertices));
  boid_vbo3 = make_bo(GL_ARRAY_BUFFER, boid_flap_vertices, sizeof(boid_flap_vertices));
  boid_vbo_normal = make_bo(GL_ARRAY_BUFFER, boid_normals, sizeof(boid_normals));
  boid_vbo_tex = make_bo(GL_ARRAY_BUFFER, ocean_tex_coords, sizeof(ocean_tex_coords));

  boid_idx = make_bo(GL_ELEMENT_ARRAY_BUFFER, boid_indices, sizeof(boid_indices));

  boid_shader = initshader("boid_vs.glsl", "boid_fs.glsl");
  glUseProgram(boid_shader);

  //Boid
  feather_tex_sampler = glGetUniformLocation(boid_shader, "feather_tex");
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, feather.data);
  glUniform1i(feather_tex_sampler, 2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenVertexArrays(1, &boid_vao);
  glBindVertexArray(boid_vao);
  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo1);
  pos = glGetAttribLocation(boid_shader, "vPos0");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo3);
  pos1 = glGetAttribLocation(boid_shader, "vPos1");
  glEnableVertexAttribArray(pos1);
  glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo_normal);
  boid_normal = glGetAttribLocation(boid_shader, "vNormal");
  glEnableVertexAttribArray(boid_normal);
  glVertexAttribPointer(boid_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindBuffer(GL_ARRAY_BUFFER, boid_vbo_tex);
  boid_texc = glGetAttribLocation(boid_shader, "vTex");
  glEnableVertexAttribArray(boid_texc);
  glVertexAttribPointer(boid_texc, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);


  init_ocean();
  init_time();

  model = glGetUniformLocation(program, "Model");
  view = glGetUniformLocation(program, "View");
  project = glGetUniformLocation(program, "Project");

  boid_view = glGetUniformLocation(boid_shader, "View");
  boid_model = glGetUniformLocation(boid_shader, "Model");
  boid_project = glGetUniformLocation(boid_shader, "Project");

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

void update_day_time() {
  int i = (frame_counter / 1800) % 4;
  GLfloat *l1, *l2;
  l1 = lighting_conditions + i * 9;
  l2 = lighting_conditions + (i + 1) * 9 % 36;
  glUniformMatrix3fv(light1, 1, GL_FALSE, l1);
  glUniformMatrix3fv(light2, 1, GL_FALSE, l2);
  glUniform1f(day_time, (frame_counter % 1800) / 1800.0);
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
  terrain_mesh.readFile("terrain.off");
  terrain_mesh.init();
  terrain_mesh.scale = 1.0;
  terrain_mesh.trans_vec = glm::vec3(0.0, 0.0, -8000.0);
  glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
  glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
  glBindVertexArray(terrain_mesh.vao);
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_view(view_mat, f);
    update_day_time();
    update_frame_counter();
    terrain_mesh.draw();
    draw_ocean(vao2);
    draw_flock(&f, model, boid_vao, boid_idx);
    if(!pause) {
      f.update_centers();
      f.update_ave_v();
      f.update_goal();
      f.update_boids();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
