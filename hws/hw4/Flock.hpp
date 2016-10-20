#ifndef FLOCK_H
#define FLOCK_H
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "vec3.hpp"
#include "common.hpp"
#include <cmath>
#define NUM_GROUP 2
#define SPEED_LIMIT 10
using namespace std;

class Flock {
public:
  Flock();
  ~Flock();
  vector<vec3> *pos;
  vector<vec3> *vel;
  vector<int> *group;
  vec3 center[NUM_GROUP];
  vec3 ave_v[NUM_GROUP];
  vec3 goal;
  vec3 goal_v;
  GLfloat radius;
  int count;
  void update_goal();
  void update_centers();
  void update_ave_v();
  void update_boids();
  vec3 v_to_center(int i);
  vec3 v_to_goal(int i);
  vec3 v_to_separate(int i);
  vec3 v_to_align(int i);
  vec3 get_pos(int i);
  vec3 limit_speed(vec3 v);
 
  void add_boid();
  void remove_boid();
  void print_boids();
  GLfloat get_dist(int i, int j);
 

};

#endif
