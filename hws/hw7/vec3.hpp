#ifndef VEC_3
#define VEC_3
#include <cstddef>
#include <iostream>
#include <string>
#include <cmath>
#include "common.hpp"

using namespace std;

class vec3 {
public:
  float data[3];
  vec3() {
    for(int i = 0; i < 3; i++) {
      data[i] = 0.0;
    }
  }

  vec3(float val){
    for(int i = 0; i < 3; i++) {
      data[i] = val;
    }
  }

  vec3(float array[]) {
    for(int i = 0; i < 3; i++) {
      data[i] = array[i];
    }
  }

  vec3(float val1, float val2, float val3) {
    data[0] = val1;
    data[1] = val2;
    data[2] = val3;
  }

  float len() const {
    float result = 0;
    for(int i = 0; i< 3; i++) {
      result += pow(this->data[i], 2);
    }
    return pow(result, 0.5);
  }

  static vec3 dot(const vec3& v, const vec3& u) {
    return vec3(v[0] * u[0], v[1] * u[1], v[2] * u[2]);
  }
  static vec3 normalize(const vec3 &v) {
    float len = v.len();
    vec3 result = v;
    return result / len;
  }

  static vec3 cross(const vec3 &u, const vec3 &v) {
    float d0 = u[1] * v[2] - u[2] * v[1];
    float d1 = u[2] * v[0] - u[0] * v[2];
    float d2 = u[0] * v[1] - u[1] * v[0];
    vec3 result = vec3(d0, d1, d2);
    return result;
  }

  static vec3 reflect(const vec3 &v, const vec3 & normal) {
    vec3 v_n = vec3::normalize(v);
    vec3 n_n = vec3::normalize(normal);
    float cos = v_n * n_n;
    return vec3::normalize(n_n * (2 * cos) - v_n);
  }
  
  static string to_string(const vec3 &u) {
    return to_string(u[0]) + " " + to_string(u[1]) + " " + to_string(u[2]);
  }

  friend ostream& operator<<(ostream &strm, const vec3 &v) {
    return strm << v.data[0] << " " << v.data[1] << " " << v.data[2];
  }
  
  vec3 operator+(const vec3 &v2) {
    vec3 result = vec3();
    for(int i = 0; i < 3; i++) {
      result.data[i] = this->data[i] + v2.data[i];
    }
    return result;
  }

  vec3 operator-(const vec3 &v2) {
    vec3 result = vec3();
     for(int i = 0; i < 3; i++) {
       result.data[i] = (double)this->data[i] - (double)v2.data[i];
    }
    return result;
  }

  vec3 operator*(float f) {
     vec3 result = vec3();
     for(int i = 0; i < 3; i++) {
      result.data[i] = this->data[i] * f;
    }
    return result;
  }

  vec3 operator/(float f) {
    vec3 result = vec3();
     for(int i = 0; i < 3; i++) {
      result.data[i] = this->data[i] / f;
     }
     return result;
  }

  float operator*(const vec3 &v) {
    float result = 0;
    for(int i = 0; i < 3; i++) {
      result += this->data[i] * v.data[i];
    }
    return result;
  }

  float& operator[](int i) {
    if( i > 3) {
      throw runtime_error("vec3 index out of bound");
    }
    return data[i];
  }

  float operator[](int i) const {
    if( i > 3) {
      throw runtime_error("vec3 index out of bound");
    }
    return data[i];
  }
  
  void operator=(const vec3 &v) {
    for(int i = 0; i < 3; i++) {
      this->data[i] = v.data[i];
    }
  }

};

#endif
