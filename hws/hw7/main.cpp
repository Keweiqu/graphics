#include "main.hpp"
int antialias;
string out_name;
int width, height;
glm::vec3 eye, look, up;
float fovy_deg;
vector<Light> lights;
vector<Pigment> pigments;
vector<SurfaceFinish> surface_finishes;
vector<Trans> trans_vecs;
vector<Object*> objects;
vector<Color*> pixels;

void readin_debugg_output() {
  cout << "antialias? " << antialias << endl;
  cout << "out file name is " << out_name << endl;
  cout << "width " << width << " height " << height << endl;
  cout << "look is " << look[0] << " " << look[1] << " " << look[2] << endl;
  cout << "fovy is " << fovy_deg << endl;

  for (unsigned int i = 0; i < pigments.size(); i++) {
    Pigment p = pigments[i];
    cout << p.toString() << endl;
  }

  for (unsigned int i = 0; i < surface_finishes.size(); i++) {
    SurfaceFinish sf = surface_finishes[i];
    cout << sf.toString() << endl;
  }

  for (unsigned int i = 0; i < trans_vecs.size(); i++) {
    cout << trans_vecs[i].toString() << endl;
  }

  for (unsigned int i = 0; i < objects.size(); i++) {
    Object* obj = objects[i];
    cout << "object: " << obj->toString() << "\n" << endl;
  }
}

void init_image() {
  for (int i = 0; i < width * height; i++) {
    Color* c = new Color(0.5, 0.5, 0.5);
    pixels.push_back(c);
  }
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "Please include input file!" << endl;
    return 0;
  }
  readin(argv[1]);
  readin_debugg_output();
  init_image();
  dump_image();
  return 0;
}
