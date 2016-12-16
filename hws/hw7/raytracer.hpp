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

Ray compute_ray(float i, float j);
void print_ray(Ray r);

float sphere_intersect(Ray& r, Sphere sphere);
float plane_intersect(Ray& r, Plane plane);
float polyhedron_intersect(Ray& r, Polyhedron poly, int& plane_index);
Status intersect(Ray& r);
vec3 sphere_normal(Sphere sphere, Point p, Ray& r);
vec3 plane_normal(Plane plane);
int visible(Point p, Light light);
Color lit(Ray r, int depth);
Color trace(Ray r, int depth);
int inside_poly(Point point, Polyhedron poly);
int inside_sphere(Point point, Sphere sphere);
int inside_plane(Point point, Plane plane);
int inside_obj(Point point, Object* obj);

#endif
