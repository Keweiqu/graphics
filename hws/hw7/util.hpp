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
Color phong_diffuse(vec3 toLight, vec3 normal, Light light, Point p, Object* obj);
Color trace(Ray r, int depth);
float attenuation(Light light, Point point);
void free_pixels();
void free_objects();
void free_planes();
void free_triangles();
Color map_color(Object* obj, Point point);
Triangle* calc_cof(vec3 a1, vec3 a2, vec3 a3);
#endif
