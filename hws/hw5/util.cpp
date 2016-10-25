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
      cout << filename << " is OFF file" << endl;
      bool hasError = FALSE;
      vector<GLfloat> v_buffer;
      vector<GLuint> f_buffer;
      getline(source, line);
      int num_vertices, num_faces, num_edges;
      stringstream stream(line);
      stream >> num_vertices >> num_faces >> num_edges;
      cout <<"num_vertices: " << num_vertices << " num_faces: " << num_faces << " num_edges: " << num_edges << endl;
      for(int i = 0; i < num_vertices && !hasError; i++) {
        vector<GLfloat> vertex_tmp;
      	getline(source, line);
        if (source.fail()) {
          cout << "Warning: incorrent line number, discard " << filename << endl;
          hasError = TRUE;
          continue;
        }
      	stringstream stream(line);
        GLfloat tmp;
        while (!stream.eof()) {
          stream >> tmp;
          vertex_tmp.push_back(tmp);
        }
        if (vertex_tmp.size() != 3) {
          cout << "Malformed file" << endl;
          hasError = TRUE;
          continue;
        }
        for (unsigned int i = 0; i < vertex_tmp.size(); i++) {
          cout << vertex_tmp[i] << " ";
        }
        cout << endl;
        v_buffer.insert(v_buffer.end(), vertex_tmp.begin(), vertex_tmp.end());
      }

      for(int i = 0; i < num_faces && !hasError; i++) {
        vector<GLuint> index_tmp;
      	getline(source, line);
        if (source.fail()) {
          cout << "Warning: incorrect line number, discard " << filename << endl;
          hasError = TRUE;
          continue;
        }
      	stringstream stream(line);
      	GLuint num;
        while (!stream.eof()) {
          stream >> num;
          index_tmp.push_back(num);
        }
        if (index_tmp.size() != 4 && index_tmp.size() != 5) {
          cout << "Malformed file" << endl;
          hasError = TRUE;
          continue;
        }
        if (index_tmp.size() == 4) {
          cout << "Face of triangle ";
        } else {
          cout << "Face of quad ";
        }
        for (unsigned int i = 0; i < index_tmp.size(); i++) {
          if (i != 0) cout << index_tmp[i];
          if (i != 0 && i != index_tmp.size() - 1) cout << " : ";
        }
        cout << endl;
        f_buffer.insert(f_buffer.end(), index_tmp.begin() + 1, index_tmp.end());
      }
      if (getline(source, line)) {
        cout << "Warning: extra lines in " << filename << " ignored" << endl;
      }
      if (!hasError) {
        mesh->vertices->insert(mesh->vertices->end(), v_buffer.begin(), v_buffer.end());
        mesh->indices->insert(mesh->indices->end(), f_buffer.begin(), f_buffer.end());
      }
    }
  }
  return 0;
}
