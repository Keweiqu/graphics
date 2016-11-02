#include "meshManager.hpp"
#include "common.hpp"
#include "main.hpp"
#include "util.hpp"
using namespace std;

GLuint fs_shader, wire_shader, phong_shader;
GLuint model, view, project, vbo, ebo, vao, pos;
glm::mat4 model_mat, view_mat, project_mat, parallel_mat;
GLfloat spin[3] = {0.0f, 0.0f, 0.0f};
GLfloat eye_dist = 30.0;
bool isPaused = false, isParallel = false;
enum draw_mode d_mode = FACE;
enum shade_mode s_mode = SMOOTH;

meshManager mesh;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

GLfloat my_vertices[] = {
  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
};


GLuint my_indices[] = {
  0, 1, 2, 2, 1, 3, 2, 3, 0, 0, 3, 1
};

vector<GLfloat> vertices(my_vertices, my_vertices + 12);
vector<GLuint> indices(my_indices, my_indices + 12);


void init() {
  glEnable(GL_DEPTH_TEST);
  fs_shader = initshader("fs_vs.glsl", "fs_fs.glsl");
  wire_shader = initshader("wire_vs.glsl", "wire_fs.glsl");
  phong_shader = initshader("phong_vs.glsl", "phong_fs.glsl");
  glUseProgram(fs_shader);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  vbo = make_bo(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(GLfloat));
  ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(GLuint));
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  pos = glGetAttribLocation(fs_shader, "vPos");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  model = glGetUniformLocation(fs_shader, "Model");
  view = glGetUniformLocation(fs_shader, "View");
  project = glGetUniformLocation(fs_shader, "Project");
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    switch(key) {
    case 'e':
    case 'E':
      d_mode = EDGE;
      glBindVertexArray(mesh.vao);
      break;
    case 't':
    case 'T':
      if(s_mode == FLAT) {
	glBindVertexArray(mesh.flat_vao);
      } else {
	glBindVertexArray(mesh.vao);
      }
      d_mode = FACE;
      break;
    case 'v':
    case 'V':
      glBindVertexArray(mesh.vao);
      d_mode = VERTEX;
      break;
    case 'f':
    case 'F':
      if(d_mode == FACE) {
	glBindVertexArray(mesh.flat_vao);
      }
      glUseProgram(fs_shader);
      s_mode = FLAT;
      break;
    case 's':
    case 'S':
      if(d_mode == FACE) {
	glBindVertexArray(mesh.vao);
      }
      glUseProgram(fs_shader);
      s_mode = SMOOTH;
      break;
    case 'k':
    case 'K':
      if(d_mode == FACE) {
	glBindVertexArray(mesh.vao);
      }
      glUseProgram(phong_shader);
      s_mode = PHONG;
      break;
    case 'q':
    case 'Q':
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(w, true);
      break;
    case 'a':
    case 'A':
      isPaused = !isPaused;
      break;
    case 'p':
    case 'P':
      isParallel = !isParallel;
      break;
    }
  }

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch(key) {
      case GLFW_KEY_UP:
      zoom_in();
      break;
      case GLFW_KEY_DOWN:
      zoom_out();
      break;
    }
  }
}

int main(int argc, char* argv[]) {
   if(!glfwInit()) {
    cerr << "Error: cannot start GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "Triangle", NULL, NULL);
  if(!window) {
    cerr << "Error: Cannot open window with GLFW3" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();
  glfwSetKeyCallback(window, keyboard);

  init();

  mesh.readFiles(argc - 1, argv + 1);
  mesh.init();

  project_mat = glm::perspective(35 * M_PI / 180.0, 1.0, 0.1, 1000.0);
  parallel_mat = glm::ortho(-10.0, 10.0, -10.0, 10.0, 0.1, 1000.0);
  glm::vec3 eye = glm::vec3(0.0, 1.0, eye_dist);
  glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0, 1, 0);
  view_mat = glm::lookAt(eye, center, up);
  model_mat = glm::mat4(1.0);
  glBindVertexArray(mesh.vao);
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (isParallel) {
      glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(parallel_mat));
    } else {
      glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
    }
    mesh.draw();
    if (!isPaused) mesh.update_angle();
    eye[2] = eye_dist;
    view_mat = glm::lookAt(eye, center, up);
    glfwSwapBuffers(window);
    glfwPollEvents();

   }

  glfwTerminate();
  return 0;
}
