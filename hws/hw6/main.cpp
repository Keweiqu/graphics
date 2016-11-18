#include "meshManager.hpp"
#include "common.hpp"
#include "main.hpp"
#include "util.hpp"
using namespace std;

// GLuint fs_shader, phong_shader;
// GLuint model, view, project, vbo, ebo, vao, pos;
// glm::mat4 model_mat, view_mat, project_mat, parallel_mat;
// GLfloat spin[3] = {0.0f, 0.0f, 0.0f};
// GLfloat scale_factor = INITIAL_SCALE_FACTOR, eye_dist = INITIAL_EYE_DIST;
// bool isPaused = false, isParallel = false;
// enum draw_mode d_mode = FACE;
// enum shade_mode s_mode = SMOOTH;
// int clicked = FALSE;
// double m_xpos, m_ypos;
// GLfloat last_x_diff, last_y_diff;
// GLfloat x_diff, y_diff;
// GLfloat x_angle, y_angle;
// glm::mat4 universe_rotate;
//
// meshManager mesh;

// static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
//   GLuint bufnum;
//   glGenBuffers(1, &bufnum);
//   glBindBuffer(type, bufnum);
//   glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
//   return bufnum;
// }

GLfloat my_vertices[] = {
  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
};


GLuint my_indices[] = {
  0, 1, 2, 2, 1, 3, 2, 3, 0, 0, 3, 1
};

vector<GLfloat> vertices(my_vertices, my_vertices + 12);
vector<GLuint> indices(my_indices, my_indices + 12);


// void init() {
//   glEnable(GL_DEPTH_TEST);
//   fs_shader = initshader("fs_vs.glsl", "fs_fs.glsl");
//   phong_shader = initshader("phong_vs.glsl", "phong_fs.glsl");
//   glUseProgram(fs_shader);
//   glEnable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
//
//   vbo = make_bo(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(GLfloat));
//   ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(GLuint));
//   glGenVertexArrays(1, &vao);
//
//   glBindVertexArray(vao);
//   glBindBuffer(GL_ARRAY_BUFFER, vbo);
//   pos = glGetAttribLocation(fs_shader, "vPos");
//   glEnableVertexAttribArray(pos);
//   glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
//
//   model = glGetUniformLocation(fs_shader, "Model");
//   view = glGetUniformLocation(fs_shader, "View");
//   project = glGetUniformLocation(fs_shader, "Project");
//   glClearColor(1.0, 1.0, 1.0, 1.0);
// }

// void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
//   if(action == GLFW_PRESS) {
//     switch(key) {
//     case 'e':
//     case 'E':
//       d_mode = EDGE;
//       glBindVertexArray(mesh.vao);
//       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.edge_ebo);
//       break;
//     case 't':
//     case 'T':
//       if(s_mode == FLAT) {
// 	glBindVertexArray(mesh.flat_vao);
//       } else {
// 	glBindVertexArray(mesh.vao);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
//       }
//       d_mode = FACE;
//       break;
//     case 'v':
//     case 'V':
//       glBindVertexArray(mesh.vao);
//       d_mode = VERTEX;
//       break;
//     case 'f':
//     case 'F':
//       if(d_mode == FACE) {
// 	glBindVertexArray(mesh.flat_vao);
//       }
//       glUseProgram(fs_shader);
//       s_mode = FLAT;
//       break;
//     case 's':
//     case 'S':
//       if(d_mode == FACE) {
// 	glBindVertexArray(mesh.vao);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
//       }
//       glUseProgram(fs_shader);
//       s_mode = SMOOTH;
//       break;
//     case 'k':
//     case 'K':
//       if(d_mode == FACE) {
// 	glBindVertexArray(mesh.vao);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
//       }
//       glUseProgram(phong_shader);
//       s_mode = PHONG;
//       break;
//     case 'q':
//     case 'Q':
//     case GLFW_KEY_ESCAPE:
//       glfwSetWindowShouldClose(w, true);
//       break;
//     case 'a':
//     case 'A':
//       isPaused = !isPaused;
//       break;
//     case 'p':
//     case 'P':
//       isParallel = !isParallel;
//       break;
//     }
//   }
//
//   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//     switch(key) {
//       case GLFW_KEY_UP:
//         zoom_in();
//       break;
//       case GLFW_KEY_DOWN:
//         zoom_out();
//       break;
//     }
//   }
// }
//
// void mouse(GLFWwindow *w, int button, int action, int mods) {
//   switch(button) {
//   case GLFW_MOUSE_BUTTON_LEFT:
//     if(action== GLFW_PRESS){
//       glfwGetCursorPos(w, &m_xpos, &m_ypos);
//       printf("pressed\n");
//       clicked = TRUE;
//     } else {
//       last_x_diff += x_diff;
//       last_y_diff += y_diff;
//       x_diff = 0;
//       y_diff = 0;
//       printf("released\n");
//       clicked = FALSE;
//     }
//   default:
//     break;
//   }
// }
//
// void cursor(GLFWwindow* window, double xpos, double ypos) {
//   if(clicked) {
//     x_diff = (xpos - m_xpos) / DRAG_SPEED_FACTOR;
//     y_diff = (ypos - m_ypos) / DRAG_SPEED_FACTOR;
//     if(x_diff < 0) {
//       x_diff += 2 * M_PI;
//     } else if(x_diff > 2 * M_PI) {
//       x_diff -= 2 * M_PI;
//     }
//
//     if(y_diff < 0) {
//       y_diff += 2 * M_PI;
//     } else if (y_diff > 2 * M_PI) {
//       y_diff -= 2 * M_PI;
//     }
//   }
// }
//
// void reshape(GLFWwindow* window, int width, int height) {
//   if (isParallel) {
//     glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(parallel_mat));
//   } else {
//     glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
//   }
// }
//
// void framebuffer_resize(GLFWwindow* window, int width, int height) {
//   glViewport(0, 0, width, height);
// }

// int main(int argc, char* argv[]) {
//    if(!glfwInit()) {
//     cerr << "Error: cannot start GLFW3" << endl;
//     glfwTerminate();
//     exit(EXIT_FAILURE);
//   }
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//   GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Triangle", NULL, NULL);
//   if(!window) {
//     cerr << "Error: Cannot open window with GLFW3" << endl;
//     glfwTerminate();
//     exit(EXIT_FAILURE);
//   }
//
//   glfwMakeContextCurrent(window);
//
//   glewExperimental = GL_TRUE;
//   glewInit();
//   glfwSetKeyCallback(window, keyboard);
//   glfwSetMouseButtonCallback(window, mouse);
//   glfwSetCursorPosCallback(window, cursor);
//   glfwSetWindowSizeCallback(window, reshape);
//   glfwSetFramebufferSizeCallback(window, framebuffer_resize);
//
//   init();
//
//   mesh.readFiles(argc - 1, argv + 1);
//   mesh.init();
//
//   project_mat = glm::perspective(35 * DEGREES_TO_RADIANS, 1.0, 0.1, 1000.0); // magic numbers, parallel and perspective don't match if modifed
//   parallel_mat = glm::ortho(-10.0, 10.0, -10.0, 10.0, 0.1, 1000.0); // magic numbers, parallel and perspective don't match if modifed
//   glm::vec3 eye = glm::vec3(0.0, 1.0, eye_dist);
//   glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
//   glm::vec3 up = glm::vec3(0, 1, 0);
//   view_mat = glm::lookAt(eye, center, up);
//   model_mat = glm::mat4(1.0);
//   glBindVertexArray(mesh.vao);
//
//   while(!glfwWindowShouldClose(window)) {
//     GLfloat new_x = last_x_diff + x_diff;
//     GLfloat new_y = last_y_diff + y_diff;
//     universe_rotate =
//       glm::rotate(new_y, glm::vec3(cos(new_x), 0.0, sin(-new_x))) *
//       glm::rotate(new_x, glm::vec3(0.0, 1.0, 0.0));
//
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     if (isParallel) {
//       glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(parallel_mat));
//     } else {
//       glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(project_mat));
//     }
//     mesh.draw();
//     if (!isPaused) mesh.update_angle();
//     eye[2] = eye_dist;
//     view_mat = glm::lookAt(eye, center, up);
//     glfwSwapBuffers(window);
//     glfwPollEvents();
//
//    }
//
//   glfwTerminate();
//   return 0;
// }
