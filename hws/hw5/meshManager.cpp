#include "meshManager.hpp"

meshManager::meshManager() {
  vn_offset = 0;
  idx_offset = 0;
  flat_offset = 0;
  vertices_normals = new vector<GLfloat>();
  indices = new vector<GLuint>();
  index_faces = new map< GLuint, vector<GLuint> >();
  filename_metadata = new map< string, metadata>();
  flat_vertices_normals = new vector<GLfloat>();
  face_normals = new vector<GLfloat>();
  draw_sequence = new vector<string>();
}

void meshManager::readFiles(int num_files, char* argv[]) {
  for(int i = 0; i < num_files; i++) {
    char* filename = argv[i];
    this->readFile(filename);
    //this->calc_normal(this->idx_offset, this->vn_offset, filename);
  }
}

void meshManager::readFile(char* filename) {
  string f_string = string(filename);
  (*filename_metadata)[f_string].vn_offset = this->vn_offset;
  (*filename_metadata)[f_string].indices_offset = this->idx_offset;
  (*filename_metadata)[f_string].flat_vn_offset = this->flat_offset;

  ifstream source;
  source.open(filename);
  if(source.fail()){
    cerr << "Cannot open file " << filename << endl;
  }
  
  string line;
  getline(source, line);
  
  if(line != "OFF") {
    cout << "File " << filename << " has incorrect format" << endl;
    return;
  } else {
    cout << "Reading data for " << filename << "..." << endl;
    this->draw_sequence->push_back(filename);
    getline(source, line);
    int num_vertices, num_faces, num_edges;
    stringstream stream(line);
    stream >> num_vertices >> num_faces >> num_edges;
  
    cout <<"num_vertices: " << num_vertices << " num_faces: " << num_faces << " num_edges: " << num_edges << endl;
    cout << "Reading vertices..." << endl;
    for(int i = 0; i < num_vertices; i++) {
      getline(source, line);
      if (source.fail()) { 
		cout << "Warning: incorrent line number, will draw crazily..." << filename << endl;
      }
      stringstream stream(line);
      GLfloat x, y, z;
      stream >> x >> y >> z;
      this->vertices_normals->push_back(x);
      this->vertices_normals->push_back(y);
      this->vertices_normals->push_back(z);
    }

    GLuint face_index = 0;
    for(int i = 0; i < num_faces; i++) {
      getline(source, line);
      if (source.fail()) {
		cout << "Warning: incorrect line number... will draw crazily with dummy values " << filename << endl;
      }
      stringstream stream(line);
      GLuint n;
      stream >> n;
      if (n < 3 || n > 4) {
		cout << "bad line ignored.. will draw crazily" << endl;
      } else if (n == 3) {
		GLuint n1, n2, n3;
		stream >> n1 >> n2 >> n3;
		this->indices->push_back(n1);
		this->indices->push_back(n2);
		this->indices->push_back(n3);
		(*this->index_faces)[n1].push_back(face_index);
		(*this->index_faces)[n2].push_back(face_index);
		(*this->index_faces)[n3].push_back(face_index);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n1]);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n2]);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n3]);
		face_index++;
	
      } else if (n == 4) {
		GLuint n1, n2, n3, n4;
		stream >> n1 >> n2 >> n3 >> n4;
		this->indices->push_back(n1);
		this->indices->push_back(n2);
		this->indices->push_back(n3);
		(*this->index_faces)[n1].push_back(face_index);
		(*this->index_faces)[n2].push_back(face_index);
		(*this->index_faces)[n3].push_back(face_index);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n1]);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n2]);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n3]);
		face_index++;

		this->indices->push_back(n1);
		this->indices->push_back(n3);
		this->indices->push_back(n4);
		(*this->index_faces)[n1].push_back(face_index);
		(*this->index_faces)[n3].push_back(face_index);
		(*this->index_faces)[n4].push_back(face_index);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n1]);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n3]);
		this->flat_vertices_normals->push_back((*this->vertices_normals)[this->vn_offset + n4]);
		face_index++;
      }
    }

    (*filename_metadata)[f_string].num_of_vertices = num_vertices;
    (*this->filename_metadata)[f_string].num_of_indices = face_index * 3;
    this->vn_offset = this->vertices_normals->size();
    this->idx_offset = this->indices->size();
    this->flat_offset += face_index * 6;

    if (getline(source, line)) {
      cout << "Warning: extra lines in " << filename << " ignored" << endl;
    }
  }
}
