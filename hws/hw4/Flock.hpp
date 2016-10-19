#ifndef FLOCK_H
#define FLOCK_H
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "common.hpp"
#include <cmath>
#define NUM_GROUP 2
#define SPEED_LIMIT 20
using namespace std;

class Flock {
public:
  Flock();
  ~Flock();
  vector<GLfloat> *pos_x;
  vector<GLfloat> *pos_y;
  vector<GLfloat> *pos_z;
  vector<GLfloat> *v_x;
  vector<GLfloat> *v_y;
  vector<GLfloat> *v_z;
  //vector<GLfloat> *pos[3];
  // vector<GLfloat> *velocity[3];
  vector<int> *group;
  GLfloat center[NUM_GROUP][3];
  GLfloat ave_v[NUM_GROUP][3];
  GLfloat goal[3];
  GLfloat goal_v[3];
  GLfloat radius;
  int count;
  void update_goal();
  void update_centers();
  void update_ave_v();
  void update_boids();
  vector<GLfloat> v_to_center(int i);
  vector<GLfloat> v_to_goal(int i);
  vector<GLfloat> v_to_separate(int i);
  vector<GLfloat> v_to_align(int i);
  vector<GLfloat> get_pos(int i);
  vector<GLfloat> limit_speed(vector<GLfloat> v);
 
  void add_boid();
  void remove_boid();
  void print_boids();
  GLfloat get_dist(int i, int j);
 

};

vector<GLfloat> operator*(const vector<GLfloat> &l, double f);
vector<GLfloat> operator/(const vector<GLfloat> &l, double f);
vector<GLfloat> operator+(const vector<GLfloat> &l, const vector<GLfloat> &r);
vector<GLfloat> operator-(const vector<GLfloat> &l, const vector<GLfloat> &r);

vector<GLfloat> normalize(vector<GLfloat> v);
GLfloat vector_len(vector<GLfloat> v);
#endif
