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
Color trace(Ray r, int depth);
#endif
