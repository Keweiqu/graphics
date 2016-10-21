#include "Flock.hpp"
#define DELTA 250
#define INITIAL_NUM 20
#define WORLD_SIZE 5000
extern int up, down;
Flock::Flock() {
  srand(time(NULL));
  pos = new vector<vec3>();
  vel = new vector<vec3>();
  
  count = 0;
  radius = 300;
  
  center[0] = 0.0; center[1] = -200.0; center[2] = 500.0;
  goal[0] = 400.0; goal[1] = 0.0; goal[2] = 500.0;
  goal_v[0] = GOAL_SPEED_LIMIT; goal_v[1] = 0.0; goal_v[2] = 0.0;
 
  for(int i = 0; i < INITIAL_NUM; i++) {
    add_boid();
  }
}

void Flock::add_boid() {
  count++;
  vec3 boid_pos = vec3(center[0] + rand() % DELTA,center[1] + rand() % DELTA, center[2] + rand() % DELTA);
  pos->push_back(boid_pos);
  vel->push_back(vec3(0.0));
}

void Flock::remove_boid() {
  if(count > 0) {
    count--;
    pos->pop_back();
    vel->pop_back();
  }
}

void Flock::update_goal() {
  if(goal[0] < WORLD_SIZE * -1.0 || goal[0] > WORLD_SIZE) {
    goal_v[0] = goal_v[0] * -1.0;
  }
  if(goal[1] < WORLD_SIZE * -1.0 || goal[1] > WORLD_SIZE) {
    goal_v[1] = goal_v[1] * -1.0;
  }
  goal = goal + goal_v;
  if(up > 0 && goal[2] < 1000) {
    goal[2] += 1;
    up--;
  }
  if(down > 0 && goal[2] > 500) {
    goal[2] -= 1;
    down--;
  }
}

void Flock::update_center() {
  vec3 c;
  if(count > 0) {
    for(int i = 0; i < count; i++) {
      c = c + (*pos)[i];
    }
    c = c / (count * 1.0);
    center = c;
  }
}

void Flock::update_ave_v() {
  if(count > 0) {
    vec3 v;
    for(int i = 0; i < count; i++) {
      v = v + (*vel)[i];
    }
    v = v / (count * 1.0);
    ave_v = v;
  }
}


void Flock::update_boids() {
  for(int i = 0; i < count; i++) {
    vec3 v1 = v_to_goal(i) * 0.01;
    vec3 v2 = v_to_center(i) * 0.0001;
    vec3 v3 = v_to_align(i) * 0.001;
    vec3 v4 = v_to_separate(i) * 0.001;
    vec3 v = v1 + v2;
    v = v + v3;
    v = v + v4;
    (*vel)[i] = (*vel)[i] + v;
    (*vel)[i] = limit_speed((*vel)[i]);
    (*pos)[i] = (*pos)[i] + (*vel)[i];
  }
}

vec3 Flock::v_to_align(int i) {
  vec3 result;
  int num = 0;
  for(int j = 0; j < count; j++) {
    if(i != j && get_dist(i , j) < radius) {
      num++;
      result = result + (*vel)[i];
    }
  }
  return result;
}

vec3 Flock::v_to_goal(int i) {
  vec3 boid_pos = (*pos)[i];
  return goal - boid_pos;
}

vec3 Flock::v_to_center(int i) {

  vec3 boid_pos = (*pos)[i];

  return center - boid_pos;
}

vec3 Flock::v_to_separate(int i) {
  vec3 result;
  int num = 0;
  for(int j = 0; j < count; j++) {
    if(j != i  && get_dist(i, j) < radius) {
      num++;
      vec3 pos_j = (*pos)[j];
      result = result + pos_j;
    }
  }
  if(num == 0) {
    return vec3();
  }
  result = result * -1.0;
  return result;
}


void Flock::print_boids() {
  for(int i = 0; i < count; i++) {
    cout << "Boid No." << i << endl;
    cout << " @ location " << (*pos)[i] << " w/ velocity " << (*vel)[i] << endl;
  }
}

void Flock::print_goal() {
  cout << "Goal @location " << goal << " w/ velocity: " << goal_v << endl;
}

GLfloat Flock::get_dist(int i, int j) {
  vec3 pos_i = (*pos)[i];
  vec3 pos_j = (*pos)[j];
  vec3 d_v = pos_i - pos_j;
  return d_v.len();
}


vec3 Flock::limit_speed(vec3 v) {
  if(v.len() > SPEED_LIMIT) {
    return vec3::normalize(v) * SPEED_LIMIT;
  }
 
  return v;
}


Flock::~Flock() {
  delete pos;
  delete vel;
}
