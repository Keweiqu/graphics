#include "Flock.hpp"
#define DELTA 51
#define INITIAL_NUM 20
extern int up, down, to_left, to_right;
extern GLfloat flight_centers[6];

Flock::Flock() {
  srand(time(NULL));
  pos = new vector<vec3>();
  vel = new vector<vec3>();
  seed = new vector<int>();
  count = 0;
  radius = 300;
  speed = 15;
  angle = 0;
  sequence = DEFAULT;
  center[0] = 2375.0; center[1] = 125.0; center[2] = 10000.0;

  goal[0] = 2000.0; goal[1] = 1500.0; goal[2] = 10000.0;
  goal_v[0] = 0.0; goal_v[1] = GOAL_SPEED; goal_v[2] = 0.0;

  for(int i = 0; i < INITIAL_NUM; i++) {
    add_boid();
  }
}

void Flock::add_boid() {
  count++;
  vec3 boid_pos = vec3(center[0] + rand() % DELTA,center[1] + rand() % DELTA, center[2] + rand() % DELTA);
  pos->push_back(boid_pos);
  vel->push_back(vec3(0.0));
  seed->push_back(rand() % DELTA);
}

void Flock::remove_boid() {
  if(count > 0) {
    count--;
    pos->pop_back();
    vel->pop_back();
    seed->pop_back();
  }
}

void Flock::update_goal() {
  GLfloat dist;
  switch (sequence) {
    case DEFAULT:
      if(goal[0] < WORLD_SIZE * -0.9 || goal[0] > WORLD_SIZE * 0.9) {
        goal_v[0] = goal_v[0] * -1.0;
      }
      if(goal[1] < WORLD_SIZE * -0.9 || goal[1] > WORLD_SIZE * 0.9) {
        goal_v[1] = goal_v[1] * -1.0;
      }
      if(to_left) {
        vec3 z = vec3(0.0, 0.0, 1.0);
        vec3 left = vec3::cross(z, goal_v);
        left = vec3::normalize(left) * 0.5;
        goal_v = vec3::normalize(vec3::normalize(goal_v) + left) * speed;
        to_left = FALSE;
      }

      if(to_right) {
        vec3 z = vec3(0.0, 0.0, 1.0);
        vec3 right = vec3::cross(goal_v, z);
        right = vec3::normalize(right) * 0.5;
        goal_v = vec3::normalize(vec3::normalize(goal_v) + right) * speed;
        to_right = FALSE;
      }
      goal = goal + goal_v;
      return;
    case ATHENA:
      dist = sqrt(pow(flight_centers[0] - goal[0], 2) + pow(flight_centers[1] - goal[1], 2));
      break;
    case NIKE:
      dist = sqrt(pow(flight_centers[2] - goal[0], 2) + pow(flight_centers[3] - goal[1], 2));
      break;
    case BEAR:
      dist = sqrt(pow(flight_centers[4] - goal[0], 2) + pow(flight_centers[5] - goal[1], 2));
      break;
  }
  if (abs(dist - FLIGHT_RADIUS) < 0.1) {
    goal_v[0] = flight_centers[0] + cos(this->angle) * FLIGHT_RADIUS - goal[0];
    goal_v[1] = flight_centers[1] + sin(this->angle) * FLIGHT_RADIUS - goal[1];
  } else {
    goal_v[0] = 0.001 * (flight_centers[0] - goal[0]);
    goal_v[1] = 0.001 * (flight_centers[1] - goal[1]);
  }
  this->angle += 0.001;
  if (this->angle >= 2) {
    this->angle -= 2;
  }
  cout << "goal x: " << goal[0] << " " << "goal y: " << goal[1] << endl;

  goal = goal + goal_v;
}

void Flock::update_centers() {
  vec3 result;
  for(int i = 0; i < count; i++) {
    result = result + (*pos)[i];
  }
  if(count == 0) {
    center = goal;
  }else {
    center =  result / (count * 1.0);
  }
}

void Flock::update_ave_v() {
  vec3 result;
  for(int i = 0; i < count; i++) {
    result = result + (*vel)[i];
  }

  if (count == 0) {
    ave_v = goal_v;
  } else {
    ave_v = result / (count * 1.0);
  }
}


void Flock::update_boids() {
  for(int i = 0; i < count; i++) {
    vec3 v1 = v_to_goal(i);
    vec3 v2 = v_to_center(i);
    vec3 v3 = v_to_align(i);
    vec3 v4 = v_to_separate(i);
    vec3 v = v1 + v2;
    v = v + v3;
    v = v + v4;
    v = v * 4.0;

    //(*vel)[i] = (*vel)[i] * 0.99;
    (*vel)[i] = (*vel)[i] + v;
    (*vel)[i] = limit_speed((*vel)[i]);
    (*pos)[i] = (*pos)[i] + (*vel)[i];
  }
}

vec3 Flock::v_to_align(int i) {
  vec3 result;
  vector<int> knn = get_knn(i);
  for(unsigned int j = 0; j < knn.size(); j++) {
    result = result + (*vel)[knn[j]];
  }
  result = result * (1.0 / (KNN * 1.0)) * 0.0001;
  return result;
}

vec3 Flock::v_to_goal(int i) {
  vec3 boid_pos = (*pos)[i];
  return (goal - boid_pos) * 0.00004;
}

vec3 Flock::v_to_center(int i) {
  vector<int> knn = get_knn(i);
  vec3 knn_center = neighbour_center(knn);
  vec3 boid_pos = (*pos)[i];
  return (knn_center - boid_pos) * 0.0001;
}

vec3 Flock::v_to_separate(int i) {
  vector<int> knn = get_knn(i);
  vec3 result;
  for(unsigned int j = 0; j < knn.size(); j++) {
    result = (*pos)[knn[j]] - (*pos)[i];
  }
  GLfloat sum = pow(result.len(), 2);
  if(sum > 0) {
    result = result * 1.0 / (sum * 1.0);
  }
  result[0] = result[0] * 1.5;
  result[1] = result[1] * 0.8;
  result[2] = result[2] * 0.3;
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


vector<int> Flock::get_knn(int i) {
  vector<GLfloat> my_group;
  vector<int> result;
  for(int j = 0; j < count; j++) {
    if(i != j) {
      my_group.push_back(get_dist(i, j));
    }
  }
  if(count <= KNN) {
    for(int k = 0; k < count; k++) {
      result.push_back(k);
    }
    return result;
  }

  sort(my_group.begin(), my_group.begin() + count - 1);
  GLfloat cutoff = my_group[KNN - 1] + 0.01 /*epislon*/;
  for(int k = 0; k <count; k++) {
    if(get_dist(i,k) < cutoff) {
      result.push_back(k);
    }
  }
  return result;
}

vec3 Flock::neighbour_center(vector<int> knn) {
  vec3 result;
  for(unsigned int i = 0; i < knn.size(); i++) {
    result = result + (*pos)[knn[i]];
  }
  if(knn.size() > 0) {
    result = result / (knn.size() * 1.0);
  }
  return result;
}

Flock::~Flock() {
  delete pos;
  delete vel;
  delete seed;
}
