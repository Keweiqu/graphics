#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <fstream>
#include <sstream>
#include "common.hpp"
#include "meshManager.hpp"
using namespace std;

#define EYE_DIST_NEAR 14
#define EYE_DIST_FAR 46
#define SCALE_FACTOR_MAX 2.2
#define SCALE_FACTOR_MIN 0.68

void zoom_in();
void zoom_out();
#endif
