#ifndef EXTERN_H
#define EXTERN_H

#include "Light.hpp"
#include "Pigment.hpp"
#include "SurfaceFinish.hpp"
#include "Trans.hpp"
#include "Object.hpp"
#include "Camera.hpp"

using namespace std;

extern int antialias;
extern string out_name;
extern float width, height;
extern int nCols, nRows;
extern Camera camera;
extern float fovy_deg;
extern vector<Light> lights;
extern vector<Pigment> pigments;
extern vector<SurfaceFinish> surface_finishes;
extern vector<Trans> trans_vecs;
extern vector<Object*> objects;
extern vector<Color*> pixels;
extern vector<Plane*> planes;
extern int print;
#endif
