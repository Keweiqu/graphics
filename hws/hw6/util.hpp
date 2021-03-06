#ifndef UTIL_H
#define UTIL_H
#include "common.hpp"
#include "Flock.hpp"
#include "gl_replace.hpp"
#include "hw6.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include "meshManager.hpp"

#define VECTOR_LENGTH 3
#define SIDES 50
#define NUM_SQUARE_VERTICES 4

#define EYE_DIST_NEAR 14
#define EYE_DIST_FAR 46
#define SCALE_FACTOR_MAX 2.2
#define SCALE_FACTOR_MIN 0.68
#define FREE_VIEW_RADIUS 35000.0

enum VIEW_TYPE {SIDE, TRAILING, FREE, FIRST_PERSON};

class View {
public:
  glm::vec3 pos;
  glm::vec3 look;
  glm::vec3 up;
  View() {
    pos = glm::vec3(0.0);
    look = glm::vec3(0.0);
    up = glm::vec3(0.0);
  }
};

void calc_ocean_vertices(GLfloat len);

void draw_flock(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_goal(Flock* f, GLuint matrix, GLuint vao, GLuint index);
void draw_ocean(GLuint vao);
void draw_terrain(meshManager& mesh, GLuint vao, GLuint ebo);
void draw_ship();
void draw_statue(meshManager& mesh, GLuint vao, GLuint ebo);
void draw_sphere();
void draw_nike();
void draw_bear();
void bind_ocean_texture(int i);
void side_view(glm::mat4 &v, Flock& f);
void update_view(glm::mat4 &view, Flock& f);
void trailing_view(glm::mat4 &view, Flock& f);
void first_person_view(glm::mat4 &view, Flock& f);
void free_view(glm::mat4 &view);
vec3 ave_flock_center(Flock& f);
vec3 calc_middleway(Flock& f);
vec3 get_side_pos(Flock& f);
vec3 get_trailing_pos(Flock &f);
vec3 get_first_person_center_pos(Flock &f);
GLfloat center_goal_dist(Flock& f);
GLfloat max_boid_goal_dist(Flock& f);
void print_step_msg(Flock* f);
void pass_light_variables_to_shader(GLuint shader);
void init_time();
void update_time(int index);
void update_ocean_time();
void update_day_time(GLuint shader);
void update_frame_counter();
void update_light_position();
void zoom_in();
void zoom_out();
#endif
