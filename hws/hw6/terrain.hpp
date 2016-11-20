#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

#define RECURSE 7
#define SIDE_LEN 129
#define MIDDLE 64.5
#define RAND_RANGE 4000

void initCorner(GLfloat height);
void genTerrain(GLfloat height);
void squareStep(int side_length);
void diamondStep(int side_length);
void fillSquareCenter(GLint top_left_x, GLint top_left_y, GLint side);
void fillDiamondCenter(GLint center_x, GLint center_y, GLint side);
void printSquare();
void genMeshOff();
void genVertexOFF(ofstream &mesh);
void genFacesOFF(ofstream &mesh);
void genVertex();
void genFaces();
void clampContour();
GLfloat distToCenter(GLint x, GLint y);

#endif
