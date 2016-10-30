#include "meshManager.hpp"
#include "common.hpp"
#include "main.hpp"
using namespace std;

GLuint fs_shader, wire_shader, p_shader;
GLuint modelview, project, vbo, ebo, vao, pos;
glm::mat4 model_mat, view_mat, project_mat, parallel_mat;
GLfloat angle = 0.0;
bool isPaused = false, isParallel = false;
enum draw_mode d_mode = FACE;
enum shade_mode s_mode = SMOOTH;

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

  modelview = glGetUniformLocation(fs_shader, "ModelView");
  project = glGetUniformLocation(fs_shader, "Project");
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    switch(key) {
    case 'e':
    case 'E':
      glUseProgram(wire_shader);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      d_mode = EDGE;
      break;
    case 't':
    case 'T':
      glUseProgram(fs_shader);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      d_mode = FACE;
      break;
    case 'v':
    case 'V':
      glUseProgram(wire_shader);
      d_mode = VERTEX;
      break;
    case 'f':
    case 'F':
      if (d_mode == FACE) {
        glUseProgram(fs_shader);
        s_mode = FLAT;
      }
      break;
    case 's':
    case 'S':
      if (d_mode == FACE) {
        glUseProgram(fs_shader);
        s_mode = SMOOTH;
      }
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

  meshManager mesh;
  mesh.readFiles(argc - 1, argv + 1);
  mesh.init();

  project_mat = glm::perspective(50 * M_PI / 180.0, 1.0, 0.1, 1000.0);
  parallel_mat = glm::ortho(-13.5, 13.5, -13.5, 13.5, 0.1, 1000.0);
  glm::vec3 eye = glm::vec3(0.0, 1.0, 15.0);
  glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0, 1, 0);
  view_mat = glm::lookAt(eye, center, up);
  model_mat = glm::mat4(1.0);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (isParallel) {
      glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(parallel_mat));
    } else {
      glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
    }
    mesh.draw();
    if (!isPaused) angle += 0.01;
    glfwSwapBuffers(window);
    glfwPollEvents();

   }

  glfwTerminate();
  return 0;
}
