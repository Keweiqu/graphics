#include "meshManager.hpp"

meshManager::meshManager() {
  vertices = new vector<GLfloat>();
  normals = new vector<GLfloat>();
  tex_coords = new vector<GLfloat>();
  indices = new vector<GLuint>();
  face_normals = new vector<glm::vec3>();
  index_faces = new map< GLuint, vector<GLuint> >();
  scale = 1.0;
  trans_vec = glm::vec3(0.0, 0.0, 0.0);
  rotate_angles = glm::vec3(0.0, 0.0, 0.0);
}

meshManager::~meshManager() {
  delete vertices;
  delete normals;
  delete tex_coords;
  delete indices;
  delete index_faces;
  delete face_normals;
}

void meshManager::readFile(string filename) {
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
    this->index_faces->clear();
    string f_string = string(filename);
    cout << "Reading data for " << filename << "..." << endl;
    getline(source, line);
    int num_vertices, num_faces, num_edges;
    stringstream stream(line);
    stream >> num_vertices >> num_faces >> num_edges;

    cout <<"num_vertices: " << num_vertices << " num_faces: " << num_faces << " num_edges: " << num_edges << endl;
    cout << "Reading vertices..." << endl;

    this->num_of_vertices = num_vertices;

    for(int i = 0; i < num_vertices; i++) {
      getline(source, line);
      if (source.fail()) {
	cout << "Warning: incorrent line number, will draw crazily..." << filename << endl;
}
      stringstream stream(line);
      GLfloat x, y, z;
      stream >> x >> y >> z;
      this->vertices->push_back(x);
      this->vertices->push_back(y);
      this->vertices->push_back(z);
      this->tex_coords->push_back(x / 50.0 / 129.0);
      this->tex_coords->push_back(y / 50.0 / 129.0);
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
	glm::vec3 face_normal = this->calc_face_normal(n1, n2, n3);
	this->face_normals->push_back(face_normal);
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
	glm::vec3 face_normal1 = this->calc_face_normal(n1, n2, n3);
	this->face_normals->push_back(face_normal1);
	face_index++;

	this->indices->push_back(n1);
	this->indices->push_back(n3);
	this->indices->push_back(n4);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	(*this->index_faces)[n4].push_back(face_index);
	glm::vec3 face_normal2 = this->calc_face_normal(n1, n3, n4);
	this->face_normals->push_back(face_normal2);
	face_index++;
      }
    }

    this->num_of_indices = face_index * 3;
    if (getline(source, line)) {
      cout << "Warning: extra lines in " << filename << " ignored" << endl;
    }
  }
  this->calc_normal();
}


glm::vec3 meshManager::calc_face_normal(GLuint v0, GLuint v1, GLuint v2) {
  glm::vec3 vertex_0 = glm::vec3((*this->vertices)[v0 * 3], (*this->vertices)[v0 * 3 + 1], (*this->vertices)[v0 * 3 + 2]);
  glm::vec3 vertex_1 = glm::vec3((*this->vertices)[v1 * 3], (*this->vertices)[v1 * 3 + 1], (*this->vertices)[v1 * 3 + 2]);
  glm::vec3 vertex_2 = glm::vec3((*this->vertices)[v2 * 3], (*this->vertices)[v2 * 3 + 1], (*this->vertices)[v2 * 3 + 2]);
  glm::vec3 vector0 = vertex_2 - vertex_1;
  glm::vec3 vector1 = vertex_0 - vertex_1;
  return glm::normalize(glm::cross(vector0, vector1));
}


void meshManager::calc_normal() {
  for(GLuint i = 0; i < this->num_of_vertices; i++) {
    vector<GLuint> faces = (*this->index_faces)[i];
    glm::vec3 normal = glm::vec3(0.0);
    for(GLuint j = 0; j < faces.size(); j++) {
      GLuint face_no = faces[j];
      normal = normal + (*this->face_normals)[face_no];
    }
    normal = normal / (faces.size() * 1.0f);
    (*this->normals).push_back(normal.x);
    (*this->normals).push_back(normal.y);
    (*this->normals).push_back(normal.z);
  }

  //Do this after all flat and smooth normals are calculated
  this->face_normals->clear();
}
