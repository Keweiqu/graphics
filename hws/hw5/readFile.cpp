#include "readFile.hpp"

int main(int argc, char* argv[]) {
  if(argc <= 1) {
    cout << "No file, no render, goodbye." << endl;
    return 0;
  }
  int numMeshes = argc - 1;
  for(int i = 0; i < numMeshes; i++) {
    char* filename = argv[i + 1];
    ifstream source;
    source.open(filename);
    if(source.fail()){
      cerr << "Cannot open file " << filename << endl;
      continue;
    }
    
    string line;
    getline(source, line);
    
    if(line != "OFF") {
      cout << "File format incorrect" << endl;
      continue;
    } else {
      cout << "is OFF file" << endl;
      getline(source, line);
      int num_vertices, num_faces, num_edges;
      stringstream stream(line);
      stream >> num_vertices >> num_faces >> num_edges;
      cout <<"num_vertices: " << num_vertices << " num_faces: " << num_faces << " num_edges" << num_edges << endl;
      for(int i = 0; i < num_vertices; i++) {
	getline(source, line);
	stringstream stream(line);
	GLfloat x, y, z;
	stream >> x >> y >> z;
	cout << "x: " << x << " y: " << y << " z: " << z << endl;
      }
      for(int i = 0; i < num_faces; i++) {
	getline(source, line);
	stringstream stream(line);
	int num;
	stream >> num;
	if(num == 3) {
	  int index1, index2, index3;
	  stream >> index1 >> index2 >> index3;
	  cout << "Face of triangle " << index1 << " : " << index2 << " : " << index3 << endl;
	} else if(num == 4) {
	  int index1, index2, index3, index4;
	  stream >> index1 >> index2 >> index3 >> index4;
	  cout << "Face of quad " << index1 << " : " << index2 << " : " << index3 << " : " << index4 << endl;
	}
      }
    }
  }
  return 0;
}
