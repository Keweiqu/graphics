#ifndef HW4_H
#define HW4_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "common.hpp"
#include "Flock.hpp"
#include "util.hpp"
#include "read_ppm.hpp"
#include "meshManager.hpp"

#define GOAL_DELTA 100;
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define DEGREES_TO_RADIANS M_PI / 180.0
#define DRAG_SPEED_FACTOR 250.0
#define INITIAL_EYE_DIST 30.0
#define INITIAL_SCALE_FACTOR 1.0

using namespace std;

// coming from main.hpp
enum draw_mode {VERTEX, EDGE, FACE};
enum shade_mode {SMOOTH, FLAT, PHONG};

#endif
