#ifndef MAIN_H
#define MAIN_H
enum draw_mode {VERTEX, EDGE, FACE};
enum shade_mode {SMOOTH, FLAT, PHONG};

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define DEGREES_TO_RADIANS M_PI / 180.0
#define DRAG_SPEED_FACTOR 250.0
#define INITIAL_EYE_DIST 30.0
#define INITIAL_SCALE_FACTOR 1.0

#endif