#include "Flock.hpp"
#define DELTA 200
#define INITIAL_NUM 20
#define WORLD_SIZE 5000
extern int up, down;
Flock::Flock() {
  srand(time(NULL));
  pos = new vector<vec3>();
  vel = new vector<vec3>();
  group = new vector<int>();
  
  count = 0;
  radius = 300;
  
  center[0][0] = 0.0; center[0][1] = 0.0; center[0][2] = 500.0;
  center[1][0] = 200.0; center[1][1] = 200.0; center[1][2] = 500.0;
  
  goal[0] = 400.0; goal[1] = 0.0; goal[2] = 500.0;
  goal_v[0] = -7.5; goal_v[1] = 0.0; goal_v[2] = 0.0;
 
  for(int i = 0; i < INITIAL_NUM; i++) {
    add_boid();
  }
}

void Flock::add_boid() {
  count++;

  int id = rand() - (RAND_MAX >> 1) > 0 ? 1 : 0;
  group->push_back(id);
  vec3 boid_pos = vec3(center[id][0] + rand() % DELTA,center[id][1] + rand() % DELTA, center[id][2] + rand() % DELTA);
  pos->push_back(boid_pos);
  vel->push_back(vec3(0.0));
}

void Flock::remove_boid() {
  if(count > 0) {
    count--;
    pos->pop_back();
    vel->pop_back();
    group->pop_back();
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

void Flock::update_centers() {
  int count1 = 0, count2 = 0;
  vec3 pos_1, pos_2;
  for(int i = 0; i < count; i++) {
    if((*group)[i] == 0) {
      count1++;
      pos_1 = pos_1 + (*pos)[i];
    } else {
      count2++;
      pos_2 = pos_2 + (*pos)[i];
    }
  }
  
  if(count1 > 0) {
    pos_1 = pos_1 / (count1 * 1.0);
    center[0] = pos_1;
  }
  if(count2 > 0) {
    pos_2 = pos_2 /(count2 * 1.0);
    center[1] = pos_2;
  }
}

void Flock::update_ave_v() {
 int count1 = 0, count2 = 0;
  vec3 v_1, v_2;
  for(int i = 0; i < count; i++) {
    if((*group)[i] == 0) {
      count1++;
      v_1 = v_1 + (*vel)[i];
    } else {
      count2++;
      v_2 = v_2 + (*vel)[i];
    }
  }

  v_1 = v_1 / (count1 * 1.0);
  v_2 = v_2 / (count2 * 1.0);
  
  ave_v[0] = v_1;
  ave_v[1] = v_2;

}


void Flock::update_boids() {
  for(int i = 0; i < count; i++) {
    vec3 v1 = v_to_goal(i) * 0.005;
    vec3 v2 = v_to_center(i);
    if(v2.len() > 0) {
      v2 = v2 / v2.len() * 1.0 / v2.len() * 10;
    }
    vec3 v3 = v_to_align(i) * 0.0001;
    vec3 v4 = v_to_separate(i);
    if(v4.len() > 0) {
      v4 = v4 / v4.len() * pow(1.0 / v4.len(), 2) * 50.0;
    }
    vec3 v5 = v_to_other_flock(i);
    if(v5.len() > 0) { 
      v5 = v5 / v5.len() * 1.0 / v5.len() * 500;
    }
    vec3 v = v1 + v2;
    v = v + v3;
    v = v + v4;
    v = v + v5;
    (*vel)[i] = (*vel)[i] + v;
    (*vel)[i] = limit_speed((*vel)[i]);
    (*pos)[i] = (*pos)[i] + (*vel)[i];
  }
}

vec3 Flock::v_to_align(int i) {
  vec3 result;
  if((*group)[i] == 0) {
    result = ave_v[0];
  } else {
    result = ave_v[1];
  }
  return result;
}

vec3 Flock::v_to_goal(int i) {
  vec3 boid_pos = (*pos)[i];
  return goal - boid_pos;
}

vec3 Flock::v_to_center(int i) {
  vec3 c;
  if((*group)[i] == 0) {
    c = center[0];
  } else {
    c = center[1];
  }

  vec3 boid_pos = (*pos)[i];

  return c - boid_pos;
}

vec3 Flock::v_to_separate(int i) {
  vec3 result = vec3();
  int num = 0;
  int group_id = (*group)[i];
  for(int j = 0; j < count; j++) {
    int group_j =(*group)[j];
    if(j != i  && group_id == group_j && get_dist(i, j) < radius) {
      num++;
      vec3 pos_j = (*pos)[j];
      result = result + pos_j;
    }
  }
  if(num == 0) {
    return vec3();
  }
  result = (result / (num * 1.0)) * -1.0;
  return result;
}

vec3 Flock::v_to_other_flock(int i) {
  vec3 other_center;
  if((*group)[i] == 0) {
    other_center = center[1];
  } else {
    other_center = center[0];
  }
  vec3 result = (*pos)[i] - other_center;
  return result;
}


void Flock::print_boids() {
  for(int i = 0; i < count; i++) {
    cout << "Boid No." << i <<"(Group) " << (*group)[i] << endl;
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
  delete group;
}
