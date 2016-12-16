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
vector<Plane*> planes;
vector<Triangle*> triangles;
int print = FALSE;

int main(int argc, char* argv[]) {
  #if DEBUG == 1
  if (argc < 4) {
    cout << "Please include row and column numbers" << endl;
    return 0;
  }
  #else
  if(argc < 2) {
    cout << "Please include input file!" << endl;
    return 0;
  }
  #endif
  readin(argv[1]);
  init_image();
  #if DEBUG == 1
  int i = atoi(argv[2]);
  int j = atoi(argv[3]);
  cout << "Tracing ray from row " << i << " and column " << j << endl;
  if(antialias) {
    Ray A = compute_ray(i, j);
  	Ray B = compute_ray(i, j + 1);
  	Ray C = compute_ray(i + 1, j + 1);
  	Ray D = compute_ray(i + 1, j);

  	Ray E = compute_ray(i + 0.5, j + 0.5);

  	Ray F = compute_ray(i + 0.5, j);
  	Ray G = compute_ray(i + 1, j + 0.5);
  	Ray H = compute_ray(i + 0.5, j + 1);
  	Ray I = compute_ray(i, j + 0.5);
  	Color c1 = (((trace(A, 0) + trace(B, 0)) + trace(C, 0)) + trace(D, 0)) * (1 / 8.0);
  	Color c2 = (((trace(F, 0) + trace(G, 0)) + trace(H, 0)) + trace(I, 0)) * (1 / 16.0);
  	Color c3 = trace(E, 0) * (1 / 4.0);
  	Color c = (c1 + c2) + c3;
  	write_pixel(i, j, c);
    cout << "Writing color " << c.toString() << " to pixel at row " << i << " and column " << j << endl;
  } else {
  	Ray r = compute_ray(i, j);
  	Color c = trace(r, 0);
  	write_pixel(i, j, c);
    cout << "Writing color " << c.toString() << " to pixel at row " << i << " and column " << j << endl;
  }
  #else
  for(int row = 0; row < nRows; row++) {
    for(int col = 0; col < nCols; col++) {
      float i = (float) row;
      float j = (float) col;
      if(antialias) {
        Ray A = compute_ray(i, j);
        Ray B = compute_ray(i, j + 1);
        Ray C = compute_ray(i + 1, j + 1);
        Ray D = compute_ray(i + 1, j);

        Ray E = compute_ray(i + 0.5, j + 0.5);

        Ray F = compute_ray(i + 0.5, j);
        Ray G = compute_ray(i + 1, j + 0.5);
        Ray H = compute_ray(i + 0.5, j + 1);
        Ray I = compute_ray(i, j + 0.5);
        Color c1 = (((trace(A, 0) + trace(B, 0)) + trace(C, 0)) + trace(D, 0)) * (1 / 8.0);
        Color c2 = (((trace(F, 0) + trace(G, 0)) + trace(H, 0)) + trace(I, 0)) * (1 / 16.0);
        Color c3 = trace(E, 0) * (1 / 4.0);
        Color c = (c1 + c2) + c3;
        write_pixel(i, j, c);
      } else {
      	Ray r = compute_ray(i, j);
      	Color c = trace(r, 0);
      	write_pixel(i, j, c);
      }
    }
  }
  #endif
  dump_image();
  free_pixels();
  free_objects();
  free_planes();
  free_triangles();
  free_images();
  return 0;
}
