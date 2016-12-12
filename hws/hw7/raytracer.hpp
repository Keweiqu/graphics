#ifndef RAY_TRACER_H
#define RAY_TRACER_H
#include <iostream>
#include "common.hpp"
#include "extern.hpp"
#include "Ray.hpp"
#include "Point.hpp"
using namespace std;

Ray compute_ray(int i, int j);
void print_ray(Ray r);
#endif
