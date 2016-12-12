#ifndef RAY_TRACER_H
#define RAY_TRACER_H
#include <iostream>
#include "common.hpp"
#include "extern.hpp"
#include "Ray.hpp"
#include "Point.hpp"
#include "util.hpp"
using namespace std;

Ray compute_ray(int i, int j);
void print_ray(Ray r);
float sphere_intersect(Ray r, Sphere sphere);
Point intersect(Ray& r, Status& status, vec3& normal, int& obj_index);
#endif
