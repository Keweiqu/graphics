#include "terrain.hpp"

using namespace std;

GLfloat heights[SIDE_LEN][SIDE_LEN];
int recurse = RECURSE;



int main() {
  initCorner(123.5);
  printSquare();
  return 0;
}

void genTerrain(GLfloat height) {
  initCorner(height);
  
  while(recurse >= 1) {
    diamondStep(pow(2, recurse));
    squareStep(pow(2, recurse));
    recurse--;
  }
}

void initCorner(GLfloat height) {
  memset(heights, 0, pow(SIDE_LEN, 2));
  heights[0][0] = height;
  heights[0][SIDE_LEN - 1] = height;
  heights[SIDE_LEN - 1][0] = height;
  heights[SIDE_LEN - 1][SIDE_LEN - 1] = height;
}


void diamondStep(int side_length) {
  //for each square, fill the center
  int top_left_x = 0, top_left_y = 0;
  while(top_left_x + side_length < SIDE_LEN) {
    while(top_left_y + side_length < SIDE_LEN) {
      fillSquareCenter(top_left_x, top_left_y, side_length);
      top_left_y += side_length;
    }
    top_left_x += side_length;
    top_left_y = 0;
  }  
}


void squareStep(int side_length) {
  //for each diamond, fill the center
  
}


void fillSquareCenter(GLint top_left_x, GLint top_left_y, GLint side) {
  GLfloat sum = 0;
  sum += heights[top_left_x][top_left_y];
  sum += heights[top_left_x + side][top_left_y];
  sum += heights[top_left_x][top_left_y + side];
  sum += heights[top_left_x + side][top_left_y + side];
  sum /= 4.0;
  GLint center_x = (top_left_x + side) / 2;
  GLint center_y = (top_left_y + side) / 2;
  heights[center_x][center_y] = sum;
}

void fillDiamondCenter(GLint center_x, GLint center_y, GLint side) {
  int divider = 0;
  int half_side = side / 2;
  GLfloat sum = 0;
  if(center_x - half_side >= 0) { // top
    sum += heights[center_x - half_side][center_y];
    divider++;
  }
  
  if(center_x + half_side < SIDE_LEN) { // bottom
    sum += heights[center_x + half_side][center_y];
    divider++;
  }

  if(center_y - half_side >= 0) { // left
    sum += heights[center_x][center_y - half_side];
    divider++;
  }

  if(center_y + half_side < SIDE_LEN) { // right
    sum += heights[center_x][center_y + half_side];
    divider++;
  }

  heights[center_x][center_y] = sum / (divider * 1.0);
}

void printSquare() {
  for(int i = 0; i < SIDE_LEN; i++) {
    for(int j = 0; j < SIDE_LEN; j++) {
      cout << heights[i][j] << "  ";
    }
    cout << endl;
  }
}
