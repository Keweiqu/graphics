#include "util.hpp"

int readFile(meshManager* mesh, int argc, char* argv[]) {
  if(argc <= 1) {
    cout << "No file, no render, goodbye." << endl;
    return -1;
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
      vector<GLfloat> v_tmp;
      vector<GLuint> f_tmp;
      getline(source, line);
      int num_vertices, num_faces, num_edges;
      stringstream stream(line);
      stream >> num_vertices >> num_faces >> num_edges;
      cout <<"num_vertices: " << num_vertices << " num_faces: " << num_faces << " num_edges" << num_edges << endl;
      for(int i = 0; i < num_vertices; i++) {
      	getline(source, line);
        if (source.fail()) {
          cout << "Input error" << endl;
          return -1;
        }
      	stringstream stream(line);
      	GLfloat x, y, z;
      	stream >> x >> y >> z;
      	cout << "x: " << x << " y: " << y << " z: " << z << endl;
        v_tmp.push_back(x);
        v_tmp.push_back(y);
        v_tmp.push_back(z);
      }

      for(int i = 0; i < num_faces; i++) {
      	getline(source, line);
        if (source.fail()) {
          cout << "Input error" << endl;
          return -1;
        }
      	stringstream stream(line);
      	int num;
      	stream >> num;
      	if(num == 3) {
      	  int index1, index2, index3;
      	  stream >> index1 >> index2 >> index3;
      	  cout << "Face of triangle " << index1 << " : " << index2 << " : " << index3 << endl;
          f_tmp.push_back(index1);
          f_tmp.push_back(index2);
          f_tmp.push_back(index3);
      	} else if(num == 4) {
      	  int index1, index2, index3, index4;
      	  stream >> index1 >> index2 >> index3 >> index4;
      	  cout << "Face of quad " << index1 << " : " << index2 << " : " << index3 << " : " << index4 << endl;
          f_tmp.push_back(index1);
          f_tmp.push_back(index2);
          f_tmp.push_back(index3);
          f_tmp.push_back(index4);
      	}
      }
      mesh->vertices->insert(mesh->vertices->end(), v_tmp.begin(), v_tmp.end());
      mesh->indices->insert(mesh->indices->end(), f_tmp.begin(), f_tmp.end());
    }
  }
  return 0;
}
