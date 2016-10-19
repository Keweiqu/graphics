#include <cstddef>
#include <iostream>
#include <cmath>
#include "common.hpp"

using namespace std;

class vec3 {
public:
  GLfloat data[3];
  vec3() {
    for(int i = 0; i < 3; i++) {
      data[i] = 0.0;
    }
  }

  vec3(GLfloat val){
    for(int i = 0; i < 3; i++) {
      data[i] = val;
    }
  }

  vec3(GLfloat array[]) {
    for(int i = 0; i < 3; i++) {
      data[i] = array[i];
    }
  }

  vec3(GLfloat val1, GLfloat val2, GLfloat val3) {
    data[0] = val1;
    data[1] = val2;
    data[2] = val3;
  }

  GLfloat len() const {
    GLfloat result = 0;
    for(int i = 0; i< 3; i++) {
      result += pow(this->data[i], 2);
    }
    return pow(result, 0.5);
  }

  static vec3 normalize(const vec3 &v) {
    GLfloat len = v.len();
    vec3 result = v;
    return result / len;
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
      result.data[i] = this->data[i] - v2.data[i];
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

  GLfloat operator*(const vec3 &v) {
    GLfloat result = 0;
    for(int i = 0; i < 3; i++) {
      result += this->data[i] * v.data[i];
    }
    return result;
  }

  GLfloat& operator[](int i) {
    if( i > 3) {
      throw runtime_error("vec3 index out of bound");
    }
    return data[i];
  }

  GLfloat operator[](int i) const {
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


