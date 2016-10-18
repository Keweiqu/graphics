#include "Flock.hpp"
#define DELTA 40
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
  center[1][0] = 200.0; center[1][1] = 200.0; center[1][2] = 500.0;
  
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

void Flock::update_centers() {
  int count1 = 0, count2 = 0;
  GLfloat x1 = 0, y1 = 0, z1 = 0, x2 = 0, y2 = 0, z2 = 0;
  for(int i = 0; i < count; i++) {
    if((*group)[i] == 0) {
      count1++;
      x1 += (*pos_x)[i];
      y1 += (*pos_y)[i];
      z1 += (*pos_z)[i];
    } else {
      count2++;
      x2 += (*pos_x)[i];
      y2 += (*pos_y)[i];
      z2 += (*pos_z)[i];
    }
  }

  x1 /= count1;
  y1 /= count1;
  z1 /= count1;
  x2 /= count2;
  y2 /= count2;
  z2 /= count2;

  center[0][0] = x1;
  center[0][1] = y1;
  center[0][2] = z1;
  center[1][0] = x2;
  center[1][1] = y2;
  center[1][2] = z2;
}

void Flock::update_ave_v() {
 int count1 = 0, count2 = 0;
  GLfloat x1 = 0, y1 = 0, z1 = 0, x2 = 0, y2 = 0, z2 = 0;
  for(int i = 0; i < count; i++) {
    if((*group)[i] == 0) {
      count1++;
      x1 += (*v_x)[i];
      y1 += (*v_y)[i];
      z1 += (*v_z)[i];
    } else {
      count2++;
      x2 += (*v_x)[i];
      y2 += (*v_y)[i];
      z2 += (*v_z)[i];
    }
  }

  x1 /= count1;
  y1 /= count1;
  z1 /= count1;
  x2 /= count2;
  y2 /= count2;
  z2 /= count2;

  cout << x1 << " " << y1 << endl;

  ave_v[0][0] = x1;
  ave_v[0][1] = y1;
  ave_v[0][2] = z1;
  ave_v[1][0] = x2;
  ave_v[1][1] = y2;
  ave_v[1][2] = z2;
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

