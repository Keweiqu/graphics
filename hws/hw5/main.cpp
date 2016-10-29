#include "meshManager.hpp"
#include "common.hpp"
using namespace std;

GLuint fs_shader, p_shader;


void init() {
  fs_shader = initshader("lab_vs.glsl", "lab_fs.glsl");
  glUseProgram(fs_shader);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
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
  init();

   while(!glfwWindowShouldClose(window)) {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glfwSwapBuffers(window);
     glfwPollEvents();
     
   }
   glfwTerminate();
  /*
  meshManager mesh;
  mesh.readFiles(argc - 1, argv + 1);
  mesh.init();

  for(unsigned int i = 0; i < mesh.draw_sequence.size(); i++) {
    string filename = mesh.draw_sequence[i];
    metadata md = (*mesh.filename_metadata)[filename];
    cout << "Printing data for " << filename << endl;
    cout << "num vertices: " << md.num_of_vertices << " num indices: " << md.num_of_indices << endl;
    cout << "vn_offset: " << md.vn_offset << " indices_offset: " << md.indices_offset << endl;
  }
  cout << endl;
  cout << "Printing vertices..." << endl;
  for(unsigned int i = 0; i < mesh.vertices->size(); i++) {
    cout << mesh.vertices->at(i) << " ";
  }
  cout << endl;
  cout << "Printing indices..." << endl;
  for(unsigned int i = 0; i < mesh.indices->size(); i++) {
    cout << mesh.indices->at(i) << " ";
  }

  cout << endl;
  cout << "Print normals..." << endl;
  for(unsigned int i = 0; i < mesh.normals->size(); i++) {
    cout << mesh.normals->at(i) << " ";
  }
  cout << endl;
  cout << "Offsets: ";
  cout << "vn offset: " << mesh.vn_offset << ", " << "idx_offset: " << mesh.idx_offset << ", " << "flat offset: " << mesh.flat_offset << endl;

  cout << "Flat vertices and normals: ";
  for (unsigned int i = 0; i < mesh.flat_vertices->size(); i++) {
    cout << mesh.flat_vertices->at(i) << " ";
  }
  cout << endl;
  */
  return 0;
}
