#include "main.hpp"

int antialias;
string out_name;
float width, height;
int nCols, nRows;
Camera camera;
float fovy_deg;
vector<Light> lights;
vector<Pigment> pigments;
vector<SurfaceFinish> surface_finishes;
vector<Trans> trans_vecs;
vector<Object*> objects;
vector<Color*> pixels;

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "Please include input file!" << endl;
    return 0;
  }
  readin(argv[1]);
  readin_debugg_output();
  init_image();
  for(int i = 0; i < nRows; i++) {
    for(int j = 0; j < nCols; j++) {
      Ray r = compute_ray(i, j);
      Color c = trace(r, 0);
      write_pixel(i, j, c);
    }
  }
  dump_image();

  return 0;
}
