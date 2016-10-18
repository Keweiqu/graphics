#include "Flock.hpp"
#define DELTA 20
#define INITIAL_NUM 20
Flock::Flock() {
  srand(time(NULL));
  pos_x = new vector<GLfloat>();
  pos_y = new vector<GLfloat>();
  pos_z = new vector<GLfloat>();
  v_x = new vector<GLfloat>();
  v_y = new vector<GLfloat>();
  v_z = new vector<GLfloat>();
  group = new vector<int>();
  count = 0;
  radius = 250;
  
  center[0][0] = 0.0; center[0][1] = 0.0; center[0][2] = 500.0;
  center[2][0] = 200.0; center[1][1] = 200.0; center[1][2] = 500.0;
  
  goal[0] = 200.0; goal[1] = 200.0; goal[2] = 500.0;
  
  for(int i = 0; i < INITIAL_NUM; i++) {
    add_boid();
  }
}

void Flock::add_boid() {
  count++;

  int id = rand() - (RAND_MAX >> 1) > 0 ? 1 : 0;
  group->push_back(id);
  pos_x->push_back(center[id][0] + rand() % DELTA);
  pos_y->push_back(center[id][1] + rand() % DELTA);
  pos_z->push_back(center[id][2] + rand() % DELTA);
  v_x->push_back(0.0);
  v_y->push_back(0.0);
  v_z->push_back(0.0);
}

void Flock::remove_boid() {
  count--;
  pos_x->pop_back();
  pos_y->pop_back();
  pos_z->pop_back();
  v_x->pop_back();
  v_y->pop_back();
  v_z->pop_back();
  group->pop_back();
}

void Flock::print_boids() {
  for(int i = 0; i < count; i++) {
    cout << "Boid No." << i <<"(Group) " << (*group)[i] << " @ pos_x " << (*pos_x)[i] << " pos_y " << (*pos_y)[i] << " pos_z " << (*pos_z)[i] << endl;
  }
}

Flock::~Flock() {
  delete pos_x;
  delete pos_y;
  delete pos_z;
  delete v_x;
  delete v_y;
  delete v_z;
  delete group;
}

