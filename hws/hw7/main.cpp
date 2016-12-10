#include "main.hpp"
int antialias;
string out_name;
int width, height;
glm::vec3 eye, look, up;
float fovy_deg;
int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "Please include input file!" << endl;
    return 0;
  }
  readin(argv[1]);
  cout << "antialias? " << antialias << endl;
  cout << "out file name is " << out_name << endl;
  cout << "width " << width << " height " << height << endl;
  cout << "look is " << look[0] << " " << look[1] << " " << look[2] << endl;
  cout << "fovy is " << fovy_deg << endl;;
  return 0;
}
