#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <GL/glu.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define LEFT -1
#define RIGHT 1
#define UP 1
#define DOWN -1
#define ANGLE_CONVERTER 180 / M_PI
#endif
