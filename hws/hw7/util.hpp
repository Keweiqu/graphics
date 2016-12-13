#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include "common.hpp"
#include "extern.hpp"
#include "Color.hpp"
#include "Point.hpp"
#include "Ray.hpp"

using namespace std;

unsigned char convert_rgb(float c);
float clamp(float f, float lower, float upper);
void dump_image();
void write_pixel(int i, int j, Color c);
void init_image();
Color background_color();
Color phong(Light light, Point point, Object* obj, vec3 normal);
Color trace(Ray r, int depth);
void free_pixels();
void free_objects();
#endif
