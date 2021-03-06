#include "common.hpp"
#include "read_ppm.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

Image image;

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

GLfloat tex_coord[] = {
  0.0, 1.0,
  1.0, 1.0,
  0.0, 0.0,
  1.0, 0.0,

  0.0, 1.0,
  1.0, 1.0,
  0.0, 0.0,
  1.0, 0.0
};

GLubyte indices[] = {
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

GLfloat normals[24];
map<int, vector<int> > index_faces;

GLuint program;
GLuint vao, vbo_pos, vbo_normal, vbo_text, texture, idx_vbo;
GLuint pos, normal, model, view, project, Tex/*for real texels*/, texc;// addr getter

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

void init() {
  glEnable(GL_DEPTH_TEST);
  program = initshader("lab_vs.glsl", "lab_fs.glsl");
  glUseProgram(program);

  vbo_pos = make_bo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
  vbo_normal = make_bo(GL_ARRAY_BUFFER, normals, sizeof(normals));
  vbo_text = make_bo(GL_ARRAY_BUFFER, tex_coord, sizeof(tex_coord));
  idx_vbo = make_bo(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices));


  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  //target, level, components, w, h, borader, format, type, texels
  glTexImage2D(GL_TEXTURE_2D, // target
	       0, //level
	       GL_RGB, // components
	       image.sizeX, // w
	       image.sizeY, // h
	       0, // border
	       GL_RGB, // format
	       GL_UNSIGNED_BYTE, // type
	       image.data // texels
	       );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  glGenVertexArrays(1, &vao);
  
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
  pos = glGetAttribLocation(program, "vPos");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
  normal = glGetAttribLocation(program, "vNormal");
  glEnableVertexAttribArray(normal);
  glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
  texc = glGetAttribLocation(program, "vTex");
  glEnableVertexAttribArray(texc);
  glVertexAttribPointer(texc, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  Tex = glGetUniformLocation(program, "tex");
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniform1i(Tex, 0);

  model = glGetUniformLocation(program, "Model");
  view = glGetUniformLocation(program, "View");
  project = glGetUniformLocation(program, "Project");

  
  //TODO figure out mipmapping...
  glClearColor(0.0, 0.0, 0.0, 1.0);
}

map<int, vector<int> > get_index_faces() {
  map<int, vector<int> > index_faces;
  int face_index = 0;
  for(unsigned int i = 0; i < 36; i += 3) {
    index_faces[indices[i]].push_back(face_index);
    index_faces[indices[i + 1]].push_back(face_index);
    index_faces[indices[i + 2]].push_back(face_index);  
    face_index++;
  }
  return index_faces;
}

glm::vec3 get_face_normal(int i0, int i1, int i2) {
  glm::vec3 v0 = glm::vec3(vertices[i0 * 3], vertices[i0 * 3 + 1], vertices[i0 * 3 + 2]);
  glm::vec3 v1 = glm::vec3(vertices[i1 * 3], vertices[i1 * 3 + 1], vertices[i1 * 3 + 2]);
  glm::vec3 v2 = glm::vec3(vertices[i2 * 3], vertices[i2 * 3 + 1], vertices[i2 * 3 + 2]);

  glm::vec3 first = v2 - v1;
  glm::vec3 second = v0 - v1;
  glm::vec3 result = glm::cross(first, second);
  result = glm::normalize(result);
  return result;
}

void calc_normals() {
  /*
  for(int i = 0; i < 8; i++) {
    glm::vec3 normal = glm::vec3(0.0);
    vector<int> faces = index_faces[i];
    cout << "*************Vertex No." << i << " **************" << endl;
    for(int j = 0; j < faces.size(); j++) {
      int face = faces[j];
      glm::vec3 face_normal =  get_face_normal(indices[face * 3], indices[face * 3 + 1], indices[face * 3 + 2]);
      cout << "normal for face No." << faces[j] << " " << face_normal[0] << " " << face_normal[1] << " " << face_normal[2] << endl;
      normal += face_normal;
    }
    cout << "============summary=============" << endl;
    normal = normal / (GLfloat) faces.size();
    cout << "This vertex share " << faces.size() << " faces" << endl;
    cout << "average normal " << normal[0] << " " << normal[1] << " " << normal[2] << endl;
    cout << "********************************" << endl;

    normals[3 * i] = normal[0];
    normals[3 * i + 1] = normal[1];
    normals[3 * i + 2] = normal[2];
  }
  */
  //0
  int i = 0;
  normals[i++] = -0.333;
  normals[i++] = 0.333;
  normals[i++] = 0.333;

  //1
  normals[i++] = 0.333;
  normals[i++] = 0.333;
  normals[i++] = 0.333;

  //2
  normals[i++] = -0.333;
  normals[i++] = -0.333;
  normals[i++] = 0.333;

  //3
  normals[i++] = 0.333;
  normals[i++] = -0.333;
  normals[i++] = 0.333;

  //4
  normals[i++] = -0.333;
  normals[i++] = 0.333;
  normals[i++] = -0.333;

  //5
  normals[i++] = 0.333;
  normals[i++] = 0.333;
  normals[i++] = -0.333;

  //6
  normals[i++] = -0.333;
  normals[i++] = -0.333;
  normals[i++] = -0.333;

  //7
  normals[i++] = 0.333;
  normals[i++] = -0.333;
  normals[i++] = -0.333;
 
}


void print_mat(glm::mat4 mat) {
  for(unsigned int i = 0; i < 4; i++) {
    for(unsigned int j = 0; j < 4; j++) {
      cout << mat[j][i] << " ";
    }
    cout << endl;
  }
}


int main(int argc, char* argv[]) {
  
  index_faces = get_index_faces();
  calc_normals();
  /*
  for(unsigned int i = 0; i < 8; i++) {
    cout << "Normal " << i << " " << normals[3 * i] << " " << normals[3 * i + 1] << " " <<normals[3 * i + 2] << endl;
  }
  */
  if(read_ppm("kira.ppm", &image) == FALSE) {
    printf("Fail to read image\n");
    exit(EXIT_FAILURE);
  }

  
  glm::mat4 model_mat = glm::mat4(1.0);
  glm::mat4 view_mat = glm::mat4(1.0);
  glm::mat4 project_mat = glm::mat4(1.0);
  glm::vec3 eye = glm::vec3(0.0, 4.0, 2.0);
  glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
  glm::vec3 up = glm::vec3(0, 1, 0);
  view_mat = glm::lookAt(eye, center, up);
 
  project_mat = glm::perspective(45 * M_PI / 180.0, 1.0, 0.1, 10.0);

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
  GLfloat angle = 0.0;
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    model_mat = glm::rotate(angle, glm::vec3(0.0, 1.0, 0.0));
    angle += 0.01;
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(model_mat));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
    glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_vbo);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*) 0);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
  

}
