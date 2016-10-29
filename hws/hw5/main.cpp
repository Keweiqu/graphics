#include "meshManager.hpp"
#include "common.hpp"
#include "main.hpp"
using namespace std;

GLuint fs_shader, wire_shader, p_shader;
GLuint modelview, project, vbo, ebo, vao, pos;
glm::mat4 model_mat, view_mat, project_mat;
enum draw_mode mode = EDGE;

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
/*
GLfloat vertices[] = {
  -0.5, 0.5, 0.5,
  0.5, 0.5, 0.5,
  -0.5, -0.5, 0.5,
  0.5, -0.5, 0.5,
  -0.5, 0.5, -0.5,
  0.5, 0.5, -0.5,
  -0.5, -0.5, -0.5,
  0.5, -0.5, -0.5
};

GLuint indices[] = {
  1, 0, 2,
  1, 2, 3,

  5, 1, 3,
  5, 3, 7,

  6, 4, 5,
  6, 5, 7,

  0, 4, 6,
  0, 6, 2,

  5, 4, 0,
  5, 0, 1,

  2, 6, 7,
  2, 7, 3
};

*/

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
  // glPointSize(3);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    switch(key) {
    case 'e':
    case 'E':
      glUseProgram(wire_shader);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      mode = EDGE;
      break;
    case 't':
    case 'T':
      glUseProgram(fs_shader);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      mode = FACE;
      break;
    case 'v':
    case 'V':
      glUseProgram(wire_shader);
      mode = VERTEX;
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

  GLFWwindow *window = glfwCreateWindow(512, 512, "Triangle", NULL, NULL);
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

  project_mat = glm::perspective(45 * M_PI / 180.0, 1.0, 0.1, 1000.0);
  glm::vec3 eye = glm::vec3(0.0, 1.0, 5.0);
  glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0, 1, 0);
  view_mat = glm::lookAt(eye, center, up);
  model_mat = glm::mat4(1.0);
  GLfloat angle = 0.0;

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    model_mat = glm::rotate(angle, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 model_view_mat = view_mat * model_mat;
    glUniformMatrix4fv(modelview, 1, GL_FALSE, glm::value_ptr(model_view_mat));
    glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
    switch(mode) {
    case EDGE:
    case FACE:
      mesh.draw_default();
      break;
    case VERTEX:
      mesh.draw_vertex_mode();
      break;
    }
    angle += 0.05;
    glfwSwapBuffers(window);
    glfwPollEvents();

   }
  glfwTerminate();
  return 0;
}
