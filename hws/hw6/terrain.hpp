#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

#define RECURSE 7
#define SIDE_LEN 129
#define MIDDLE SIDE_LEN * 1.0 / 2
#define RAND_RANGE 4000

void initCorner(GLfloat height);
void genTerrain(GLfloat height);
void squareStep(int side_length);
void diamondStep(int side_length);
void fillSquareCenter(GLint top_left_x, GLint top_left_y, GLint side);
void fillDiamondCenter(GLint center_x, GLint center_y, GLint side);
void printSquare(GLfloat hgts[][SIDE_LEN / 2 + 1], int length);
void genMeshOff(string filename, int length, GLfloat hgts[][SIDE_LEN / 2 + 1]);
void genVertexOFF(ofstream &mesh, int length, GLfloat hgts[][SIDE_LEN / 2 + 1]);
void genFacesOFF(ofstream &mesh, int length);
void genVertex();
void genFaces();
void clampContour();
GLfloat distToCenter(GLint x, GLint y);
void simplify(string filename);

#endif
