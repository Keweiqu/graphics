#include "terrain.hpp"

GLfloat heights[SIDE_LEN][SIDE_LEN];
GLfloat simplified_heights[SIDE_LEN / 2 + 1][SIDE_LEN / 2 + 1];
int recurse = RECURSE;
GLfloat rand_range = (GLfloat) RAND_RANGE;
GLfloat terrain_vertices[SIDE_LEN * SIDE_LEN * 3];
GLuint terrain_indices[SIDE_LEN * SIDE_LEN * 2 * 3]; // s * s grids * 2 triangle each gird * 3 indices per triangle

int main() {
  /*
  srand(time(NULL));
  initCorner(0.5);

  genTerrain(0.5);
  clampContour();
  //printSquare();
  genMeshOff();
  genVertex();
  genFaces();
  */

  simplify("terrain3.off");
  //printSquare(simplified_heights, SIDE_LEN / 2 + 1);
  genMeshOff("s_terrain3.off", SIDE_LEN / 2 + 1, simplified_heights);
  return 0;
}

void genTerrain(GLfloat height) {
  initCorner(height);
  srand(time(NULL));
  while(recurse >= 1) {
    diamondStep(pow(2, recurse));
    squareStep(pow(2, recurse));
    rand_range = rand_range / 2.0;
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
  int row = 0;
  int half_side = side_length / 2;
  int center_x = 0, center_y = 0;
  while(center_x < SIDE_LEN) {
    if(row % 2 == 0) {
      center_y += half_side;
    }
    while(center_y < SIDE_LEN) {
      fillDiamondCenter(center_x, center_y, side_length);
      center_y += side_length;
    }
    row++;
    center_y = 0;
    center_x += half_side;
  }
}


void fillSquareCenter(GLint top_left_x, GLint top_left_y, GLint side) {
  GLfloat sum = 0;
  sum += heights[top_left_x][top_left_y];
  sum += heights[top_left_x + side][top_left_y];
  sum += heights[top_left_x][top_left_y + side];
  sum += heights[top_left_x + side][top_left_y + side];
  sum /= 4.0;
  GLint center_x = top_left_x + (side / 2);
  GLint center_y = top_left_y + (side / 2);
  sum += (rand() / (GLfloat) RAND_MAX) * rand_range;
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

  sum /= (GLfloat) divider;
  sum += (rand() / (GLfloat) RAND_MAX) * rand_range;
  heights[center_x][center_y] = sum;
}

void printSquare(GLfloat hgts[][SIDE_LEN / 2 + 1], int length) {
  for(int i = 0; i < length; i++) {
    for(int j = 0; j < length; j++) {
      cout << hgts[i][j] << "  ";
    }
    cout << endl;
    cout << "===========================================" << endl;
  }
}

void genMeshOff(string filename, int length, GLfloat hgts[][SIDE_LEN / 2 + 1]) {
  ofstream mesh;
  mesh.open(filename);
  mesh << "OFF\n";
  mesh << length * length << " ";
  mesh << (length - 1) * (length - 1) * 2 << " ";
  mesh << length * (length - 1) * 2 + (length - 1) * (length - 1) << "\n";
  genVertexOFF(mesh, length, hgts);
  genFacesOFF(mesh, length);
}

void genVertexOFF(ofstream &mesh, int length, GLfloat hgts[][SIDE_LEN / 2 + 1]) {
  GLfloat width = 100;
  int row, col;
  for(row = 0; row < length; row++) {
    for(col = 0; col < length; col++) {
      mesh << row * width << " " << col * width << " " << hgts[row][col] << "\n";
    }
  }
}

void genFacesOFF(ofstream &mesh, int length) {
  int index = 0;
  for(int i = 0; i < length - 1; i++) {
    for(int j = 0; j < length - 1; j++) {
      mesh << "3 " << index + j << " " << index + j + length + 1 << " " << index + j + 1 << "\n";
      mesh << "3 " << index + j << " " << index + j + length << " " << index + j + length + 1 << "\n";
    }
    index += SIDE_LEN / 2 + 1;
  }
}

void genVertex() {
  GLfloat width = 50;
  int row, col;
  for(row = 0; row < SIDE_LEN; row++) {
    for(col = 0; col < SIDE_LEN; col++) {
      terrain_vertices[row * SIDE_LEN + col] = row * width;
      terrain_vertices[row * SIDE_LEN + col + 1] = col * width;
      terrain_vertices[row * SIDE_LEN + col + 2] = heights[row][col];
    }
  }
}

void genFaces() {
  int counter = 0, index = 0;
  for(int i = 0; i < SIDE_LEN; i++) {
    for(int j = 0; j < SIDE_LEN; j++) {
      terrain_indices[counter++] = index + j;
      terrain_indices[counter++] = index + j + SIDE_LEN + 1;
      terrain_indices[counter++] = index + j + 1;

      terrain_indices[counter++] = index + j;
      terrain_indices[counter++] = index + j + SIDE_LEN;
      terrain_indices[counter++] = index + j + SIDE_LEN + 1;
    }
    index += SIDE_LEN;
  }
  
}


void clampContour() {
  GLfloat range = 350.0;
  GLfloat drop = 350;
  for(int i = 20; i > 0; i--) {
    cout << "i is " << i << endl;
    for(int j = 0; j < SIDE_LEN - i; j++) {
      //top
      heights[i - 1][j + i] = heights[i][j + i]  - drop + (rand() / (GLfloat) RAND_MAX) * range;
      //bottom
      heights[SIDE_LEN - i][j + i] = heights[SIDE_LEN - i - 1][j + i] - drop + (rand() / (GLfloat) RAND_MAX) * range;
      //left
      heights[j + i][i - 1] = heights[j + i][i] - drop + (rand() / (GLfloat) RAND_MAX) * range;
      //right
      heights[j + i][SIDE_LEN - i] = heights[j + i][SIDE_LEN - i - 1] - drop + (rand() / (GLfloat) RAND_MAX) * range;
    }
  }
}


GLfloat distToCenter(GLint x, GLint y) {
  GLfloat result = sqrt(sqrt(pow(x - MIDDLE, 2) +  pow(y - MIDDLE, 2)));
  return result;
}

void simplify(string filename) {
  ifstream source;
  source.open(filename);
  if(source.fail()) {
    cerr << "Cannot open file" << filename << endl;
  }

  string line;
  getline(source, line);//OFF
  getline(source, line);
  int num_vertices;
  stringstream stream(line);
  stream >> num_vertices;
  for(int i = 0; i < SIDE_LEN; i++) {
    for(int j = 0; j < SIDE_LEN; j++) {
      GLfloat x, y, height;
      getline(source, line);
      stringstream stream(line);
      stream >> x >> y >> height;
      heights[i][j] = height;
    }
  }
  
  for(int i = 0; i < SIDE_LEN; i += 2) {
    for(int j = 0; j < SIDE_LEN; j += 2) {
      cout << "i is " << i << " j is " << j << " " << heights[i][j] << endl;
      cout << "i / 2 is " << i / 2 << " j / 2 is " << j / 2 << endl;
      simplified_heights[i / 2][j / 2] = heights[i][j];
    }
  }
}
