#include "meshManager.hpp"

meshManager::meshManager() {
  v_offset = 0;
  idx_offset = 0;
  vertices_normals = new vector<GLfloat>();
  indices = new vector<GLuint>();
  index_faces = new map< GLuint, vector<GLuint> >();
  filename_metadata = new map< string, metadata>();
}

void meshManager::readFiles(int num_files, char* argv[]) {
  for(int i = 0; i < num_files; i++) {
    char* filename = argv[i];
    this->readFile(filename);
    //this->calc_normal(this->idx_offset, this->vn_offset, filename);
  }
}

void meshManager::readFile(char* filename) {
  this->index_faces->clear();
  string f_string = string(filename);
  (*filename_metadata)[f_string].vn_offset = this->vn_offset;
  (*filename_metadata)[f_string].indices_offset = this->idx_offset;
  (*filename_metadata)[f_string].flat_offset = this->flat_offset;

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
	this->indices->push_back(n1 + this->v_offset);
	this->indices->push_back(n2 + this->v_offset);
	this->indices->push_back(n3 + this->v_offset);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n2].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	glm::vec3 face_normal = this->calc_face_normal(n1 + this->v_offset, n2 + this->v_offset, n3 + this->v_offset);
	this->face_normals->push_back(face_normal);
	face_index++;
	
      } else if (n == 4) {
	GLuint n1, n2, n3, n4;
	stream >> n1 >> n2 >> n3 >> n4;
	this->indices->push_back(n1 + this->v_offset);
	this->indices->push_back(n2 + this->v_offset);
	this->indices->push_back(n3 + this->v_offset);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n2].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	glm::vec3 face_normal = this->calc_face_normal(n1 + this->v_offset, n2 + this->v_offset, n3 + this->v_offset);
	this->face_normals->push_back(face_normal);
	face_index++;

	this->indices->push_back(n1 + this->v_offset);
	this->indices->push_back(n3 + this->v_offset);
	this->indices->push_back(n4 + this->v_offset);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	(*this->index_faces)[n4].push_back(face_index);
	glm::vec3 face_normal = this->calc_face_normal(n1 + this->v_offset, n3 + this->v_offset, n4 + this->v_offset);
	this->face_normals->push_back(face_normal);
	face_index++;
      }
    }

    (*filename_metadata)[f_string].num_of_vertices = num_vertices;
    (*this->filename_metadata)[f_string].num_of_indices = face_index * 3;

    if (getline(source, line)) {
      cout << "Warning: extra lines in " << filename << " ignored" << endl;
    }
  }
}


glm::vec3 meshManager::calc_face_normal(GLuint v0, GLuint v1, GLuint v2) {
  glm::vec3 vertex_0 = glm::vec3(this->vertices[v0 * 3], this->vertices[v0 * 3 + 1], this->vertices[v0 * 3 + 2]);
  glm::vec3 vertex_1 = glm::vec3(this->vertices[v1 * 3], this->vertices[v1 * 3 + 1], this->vertices[v1 * 3 + 2]);
  glm::vec3 vertex_2 = glm::vec3(this->vertices[v2 * 3], this->vertices[v2 * 3 + 1], this->vertices[v2 * 3 + 2]);
  glm::vec3 vector0 = vertex_2 - vertex_1;
  glm::vec3 vector1 = vertex_0 - vertex_1;
  return glm::normalize(glm::cross(vector0, vector1));
}


