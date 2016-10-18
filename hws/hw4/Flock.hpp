#ifndef FLOCK_H
#define FLOCK_H
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "common.hpp"
#define NUM_GROUP 2
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
  vector<int> *group;
  GLfloat center[NUM_GROUP][3];
  GLfloat ave_v[NUM_GROUP][3];
  GLfloat goal[3];
  GLfloat radius;
  int count;
  void update_centers();
  void update_ave_v();
  //vector<GLfloat> seperate();
  //vector<GLfloat> cohere();
  //vector<GLfloat> align();
  //void update_center();
  void add_boid();
  void remove_boid();
  void print_boids();

};
#endif
