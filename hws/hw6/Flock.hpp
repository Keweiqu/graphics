#ifndef FLOCK_H
#define FLOCK_H
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include "vec3.hpp"
#include "common.hpp"
#include <cmath>
#define NUM_GROUP 2
#define SPEED_LIMIT 16
#define GOAL_SPEED 15
#define KNN 5
#define WORLD_SIZE 20000
#define FLIGHT_RADIUS 2000.0
enum FLIGHT_SEQUENCE {BEAR = 0, ATHENA = 2, NIKE = 4, DEFAULT};
using namespace std;

class Flock {
public:
  Flock();
  ~Flock();
  vector<vec3> *pos;
  vector<vec3> *vel;
  vector<int> *seed;
  vector<int> *group;
  vec3 center;
  vec3 ave_v;
  vec3 goal;
  vec3 goal_v;
  GLfloat radius;
  GLfloat speed;
  int count;
  int in_range;
  GLfloat angle;
  enum FLIGHT_SEQUENCE sequence;
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
  vec3 neighbour_center(vector<int> knn);
  void add_boid();
  void remove_boid();
  void print_boids();
  void print_goal();
  GLfloat get_dist(int i, int j);
  vector<int> get_knn(int i);


};

#endif
