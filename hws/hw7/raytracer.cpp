#include "raytracer.hpp"

Ray compute_ray(int i, int j) {
  float px = width * (j / (float) nCols) - width / 2.0;
  float py = -height * (i / (float) nRows) + height / 2.0;
  float pz = -1.0;
  vec3 dest = vec3(px, py, pz);
  vec3 source = camera.o;
  vec3 direction = vec3::normalize(dest - source);
  Ray r = Ray(source, direction);
  return r;
}

Color trace(Ray r, int depth) {
  if(depth > MAX_DEPTH) {
    return background_color();
  }
  //for each light source, compute!
}

void print_ray(Ray r) {
  cout << "===============Print Ray==============" << endl;
  cout << "origin is " << r.o[0]  << " " << r.o[1] << " " << r.o[2] << endl;
  cout << "direction is " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
  cout << "t is " << r.t << endl;
}
