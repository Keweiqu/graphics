#ifndef RAY_TRACER_H
#define RAY_TRACER_H
#include <iostream>
#include "common.hpp"
#include "extern.hpp"
#include "Ray.hpp"
#include "Light.hpp"
#include "Point.hpp"
#include "util.hpp"
using namespace std;

Ray compute_ray(int i, int j);
void print_ray(Ray r);

float sphere_intersect(Ray& r, Sphere sphere);
Status intersect(Ray& r);
vec3 sphere_normal(Sphere sphere, Point p);
int visible(Point p, Light light);
Color lit(Ray r);
Color trace(Ray r, int depth);

#endif
