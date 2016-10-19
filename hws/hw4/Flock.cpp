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
  
  goal[0] = 400.0; goal[1] = 0.0; goal[2] = 500.0;
  goal_v[0] = -1; goal_v[1] = 0; goal_v[2] = 0;
 
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

void Flock::update_goal() {
  goal[0] += goal_v[0];
  goal[1] += goal_v[1];
  goal[2] += goal_v[2];
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

  ave_v[0][0] = x1;
  ave_v[0][1] = y1;
  ave_v[0][2] = z1;
  ave_v[1][0] = x2;
  ave_v[1][1] = y2;
  ave_v[1][2] = z2;
}


void Flock::update_boids() {
  for(int i = 0; i < count; i++) {
    vector<GLfloat> v1 = v_to_goal(i) * 0.0005;
    vector<GLfloat> v2 = v_to_center(i) * 0.000;
    vector<GLfloat> v3 = v_to_align(i) * 0.0001;
    vector<GLfloat> v4 = v_to_separate(i) * 0.0001;
    vector<GLfloat> v = v1 + v2;
    v = v + v3;
    v = v + v4;
    v = limit_speed(v);
    (*v_x)[i] += v[0];
    (*v_y)[i] += v[1];
    (*v_z)[i] += v[2];
    

    (*pos_x)[i] += (*v_x)[i];
    (*pos_y)[i] += (*v_y)[i];
    (*pos_z)[i] += (*v_z)[i];
  }
}

vector<GLfloat> Flock::v_to_align(int i) {
  vector<GLfloat> result;
  if((*group)[i] == 0) {
    result.push_back(ave_v[0][0]);
    result.push_back(ave_v[0][1]);
    result.push_back(ave_v[0][2]);
  } else {
    result.push_back(ave_v[1][0]);
    result.push_back(ave_v[1][1]);
    result.push_back(ave_v[1][2]);
  }
  return result;
}

vector<GLfloat> Flock::v_to_goal(int i) {
  vector<GLfloat> result;
  vector<GLfloat> pos;
  pos.push_back((*pos_x)[i]);
  pos.push_back((*pos_y)[i]);
  pos.push_back((*pos_z)[i]);
  for(int j = 0; j < 3; j++) {
    result.push_back(goal[j] - pos[j]);
  }
  return result;
}

vector<GLfloat> Flock::v_to_center(int i) {
  GLfloat *c;
  if((*group)[i] == 0) {
    c = center[0];
  } else {
    c = center[1];
  }
  vector<GLfloat> result;
  vector<GLfloat> pos;
  pos.push_back((*pos_x)[i]);
  pos.push_back((*pos_y)[i]);
  pos.push_back((*pos_z)[i]);
  for(int j = 0; j < 3; j++) {
    result.push_back(c[j] - pos[j]);
  }
  return result;
}

vector<GLfloat> Flock::v_to_separate(int i) {
  vector<GLfloat> result;
  result.push_back(0);
  result.push_back(0);
  result.push_back(0);
  int num = 0;
  for(int j = 0; j < count; j++) {
    if(get_dist(i, j) < radius) {
      num++;
      vector<GLfloat> pos_j = get_pos(j);
      result = result + pos_j;
    }
  }
  result = (result / (num * 1.0)) * -1.0;
  return result;
}


void Flock::print_boids() {
  for(int i = 0; i < count; i++) {
    cout << "Boid No." << i <<"(Group) " << (*group)[i] << " @ pos_x " << (*pos_x)[i] << " pos_y " << (*pos_y)[i] << " pos_z " << (*pos_z)[i] << endl;
  }
}

GLfloat Flock::get_dist(int i, int j) {
  vector<GLfloat> pos_i = get_pos(i);
  vector<GLfloat> pos_j = get_pos(j);
  vector<GLfloat> d_v = pos_i - pos_j;
  GLfloat dist = 0;
  for(int k = 0; k < d_v.size(); k++) {
    dist += pow(d_v[k],2);
  }
  return pow(dist, 0.5);
}

vector<GLfloat> Flock::get_pos(int i) {
  vector<GLfloat> result;
  result.push_back((*pos_x)[i]);
  result.push_back((*pos_y)[i]);
  result.push_back((*pos_z)[i]);
  return result;
}

vector<GLfloat> Flock::limit_speed(vector<GLfloat> v) {
  if(vector_len(v) > SPEED_LIMIT) {
    return normalize(v) * SPEED_LIMIT;
  }
  return v;
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


vector<GLfloat> operator*(const vector<GLfloat> &l, double f) {
  vector<GLfloat> result;
  for(int i = 0; i < l.size(); i++) {
    result.push_back(l[i] * f);
  }
  return result;
}


vector<GLfloat> operator+(const vector<GLfloat> &l, const vector<GLfloat> &r) {
  if(l.size() != r.size()) {
    throw runtime_error("Cannot add vector of different sizes");
  }
  vector<GLfloat> result;
  for(int i = 0; i < l.size(); i++) {
    result.push_back(l[i] + r[i]);
  }
  return result;
}

vector<GLfloat> operator-(const vector<GLfloat> &l, const vector<GLfloat> &r) {
  if(l.size() != r.size()) {
    throw runtime_error("Cannot subtract vector of different sizes");
  }
  vector<GLfloat> result;
  for(int i = 0; i < l.size(); i++) {
    result.push_back(l[i] - r[i]);
  }
  return result;
}

vector<GLfloat> operator/(const vector<GLfloat> &l, double f) {
  vector<GLfloat> result;
  for(int i = 0; i < l.size(); i++) {
    result.push_back(l[i] / f);
  }
  return result;
}


vector<GLfloat> normalize(vector<GLfloat> v) {
  GLfloat len = vector_len(v);
  return v / len;
}


GLfloat vector_len(vector<GLfloat> v) {
  GLfloat sum = 0;
  for(int i = 0; i < v.size(); i++) {
    sum += v[i] * v[i];
  }
  sum = pow(sum, 0.5);
  return sum;
}
