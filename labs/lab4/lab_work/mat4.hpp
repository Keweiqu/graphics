#include <cstddef>
#include <iostream>
using namespace std;

class mat4 {
public:
  GLfloat data[16];
  mat4() {
    fill(data, data + 16, 0.0);
    data[0] = 1.0;
    data[5] = 1.0;
    data[10] = 1.0;
    data[15] = 1.0;
  }
  
  mat4 operator*(const mat4 &m) {
    mat4 result;
    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
	GLfloat row[4] = {this->data[i], this->data[i + 4], this->data[i + 8], this->data[i + 12]};
	GLfloat col[4] = {m[j * 4], m[j * 4 + 1], m[j * 4 + 2], m[j * 4 + 3]};
	GLfloat sum = 0;
	for(int k = 0; k < 4; k++) {
	  sum += row[k] * col[k];
	}
	result[i + j * 4] = sum;
      }
    }
    return result;
  }
  
  GLfloat operator[](int i) const {
    if(i > 16) {
      throw runtime_error("mat4 index out of bound");
    }
    return data[i];
  }
  
  GLfloat& operator[](int i) {
    if(i > 16) {
      throw runtime_error("mat4 index out of bound");
    }
    return data[i];
  }

  static void print(const mat4 &m) {
    for(int i = 0; i < 4; i++) {
      cout << m[i] << " " << m[i + 4] << " " << m[i + 8] << " " << m[i + 12] << endl;
    }
  }
};
