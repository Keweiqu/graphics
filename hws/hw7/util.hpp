#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include "common.hpp"
#include "extern.hpp"

using namespace std;

unsigned char convert_rgb(float c);
float clamp(float f, float lower, float upper);
void dump_image();

#endif
