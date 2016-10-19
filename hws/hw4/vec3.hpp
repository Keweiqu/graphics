#include <cstddef>
#include <iostream>

using namespace std;

template<class T>
class vec3 {
public:
  T data[3];
  vec3<T>(): vec3<T>(NULL) {}

  vec3<T>(T val){
    for(int i = 0; i < 3; i++) {
      data[i] = val;
    }
  }

  vec3<T>(T array[]) {
    for(int i = 0; i < 3; i++) {
      data[i] = array[i];
    }
  }

  vec3<T>(T val1, T val2, T val3) {
    data[0] = val1;
    data[1] = val2;
    data[2] = val3;
  }

  friend ostream& operator<<(ostream &strm, const vec3<T> &v) {
    return strm << v.data[0] << " " << v.data[1] << " " << v.data[2];
  }
  /*
  vec3<T> operator+(const vec3<T> &v1, const vec3<T> &v2);
  vec3<T> operator-(const vec3<T> &v1, const vec3<T> &v2);
  vec3<T> operator*(const vec3<T> &v, float f);
  vec3<T> operator/(const vec3<T> &v, float f);
  T& operator[](int i);
  */
};


