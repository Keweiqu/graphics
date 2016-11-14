#include "common.hpp"
#include <cmath>
#include <cstring>
#include <iostream>

#define SIDE_LEN 4 + 1
#define RECURSE 2

void initCorner(GLfloat height);
void genTerrain(GLfloat height);
void squareStep();
void diamondStep(int side_length);
void fillSquareCenter(GLint top_left_x, GLint top_left_y, GLint side);
void fillDiamondCenter(GLint center_x, GLint center_y, GLint side)
void printSquare();
