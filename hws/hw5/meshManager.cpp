#include "meshManager.hpp"
#include "main.hpp"

extern GLuint fs_shader, modelview, project;
extern GLfloat angle;
extern glm::mat4 view_mat;
extern enum draw_mode mode;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

meshManager::meshManager() {
  vn_offset = 0;
  idx_offset = 0;
  flat_offset = 0;
  vertices = new vector<GLfloat>();
  normals = new vector<GLfloat>();
  indices = new vector<GLuint>();
  face_normals = new vector<glm::vec3>();
  index_faces = new map< GLuint, vector<GLuint> >();
  filename_metadata = new map< string, metadata>();
  flat_vertices = new vector<GLfloat>();
  flat_normals = new vector<GLfloat>();
}

void meshManager::readFiles(int num_files, char* argv[]) {
  for(int i = 0; i < num_files; i++) {
    char* filename = argv[i];
    this->readFile(filename);
    this->calc_normal(filename);

  }
}

void meshManager::readFile(char* filename) {
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
    this->draw_sequence.push_back(filename);
    if(this->filename_metadata->find(filename) != this->filename_metadata->end()) {
      return;
    }

    this->index_faces->clear();
    string f_string = string(filename);
    (*filename_metadata)[f_string].vn_offset = this->vn_offset;
    (*filename_metadata)[f_string].indices_offset = this->idx_offset;
    (*filename_metadata)[f_string].flat_offset = this->flat_offset;
    cout << "Reading data for " << filename << "..." << endl;
    getline(source, line);
    int num_vertices, num_faces, num_edges;
    stringstream stream(line);
    stream >> num_vertices >> num_faces >> num_edges;

    cout <<"num_vertices: " << num_vertices << " num_faces: " << num_faces << " num_edges: " << num_edges << endl;
    cout << "Reading vertices..." << endl;

    GLfloat x_max = FLT_MIN, x_min = FLT_MAX, y_max = FLT_MIN, y_min = FLT_MAX, z_max = FLT_MIN, z_min = FLT_MAX;
    for(int i = 0; i < num_vertices; i++) {
      getline(source, line);
      if (source.fail()) {
	cout << "Warning: incorrent line number, will draw crazily..." << filename << endl;
      }
      stringstream stream(line);
      GLfloat x, y, z;
      stream >> x >> y >> z;
      x_max = max(x_max, x); x_min = min(x_min, x);
      y_max = max(y_max, y); y_min = min(y_min, y);
      z_max = max(z_max, z); z_min = min(z_min, z);
      this->vertices->push_back(x);
      this->vertices->push_back(y);
      this->vertices->push_back(z);
    }

    GLfloat x_center = (x_max + x_min) / 2.0;
    GLfloat y_center = (y_max + y_min) / 2.0;
    GLfloat z_center = (z_max + z_min) / 2.0;
    GLfloat x_scale = WIDTH / (x_max - x_min);
    GLfloat y_scale = WIDTH / (y_max - y_min);
    GLfloat z_scale = WIDTH / (z_max - z_min);
    GLfloat scale = min(min(x_scale, y_scale), z_scale);
    (*filename_metadata)[f_string].model_mat =
      glm::scale(glm::vec3(scale, scale, scale)) *
      glm::translate(glm::vec3(-x_center,-y_center, -z_center));

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
	this->indices->push_back(n1 + this->vn_offset / 3);
	this->indices->push_back(n2 + this->vn_offset / 3);
	this->indices->push_back(n3 + this->vn_offset / 3);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n2].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3 + 2]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n2 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n2 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n2 * 3 + 2]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3 + 2]);

	glm::vec3 face_normal = this->calc_face_normal(n1 + this->vn_offset / 3, n2 + this->vn_offset / 3, n3 + this->vn_offset / 3);
	this->face_normals->push_back(face_normal);
	face_index++;

      } else if (n == 4) {
	GLuint n1, n2, n3, n4;
	stream >> n1 >> n2 >> n3 >> n4;
	this->indices->push_back(n1 + this->vn_offset / 3);
	this->indices->push_back(n2 + this->vn_offset / 3);
	this->indices->push_back(n3 + this->vn_offset / 3);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n2].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3 + 2]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n2 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n2 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n2 * 3 + 2]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3 + 2]);
	glm::vec3 face_normal1 = this->calc_face_normal(n1 + this->vn_offset / 3, n2 + this->vn_offset / 3, n3 + this->vn_offset / 3);
	this->face_normals->push_back(face_normal1);
	face_index++;

	this->indices->push_back(n1 + this->vn_offset / 3);
	this->indices->push_back(n3 + this->vn_offset / 3);
	this->indices->push_back(n4 + this->vn_offset / 3);
	(*this->index_faces)[n1].push_back(face_index);
	(*this->index_faces)[n3].push_back(face_index);
	(*this->index_faces)[n4].push_back(face_index);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n1 * 3 + 2]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n3 * 3 + 2]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n4 * 3]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n4 * 3 + 1]);
	this->flat_vertices->push_back((*this->vertices)[this->vn_offset + n4 * 3 + 2]);
	glm::vec3 face_normal2 = this->calc_face_normal(n1 + this->vn_offset / 3, n3 + this->vn_offset / 3, n4 + this->vn_offset / 3);
	this->face_normals->push_back(face_normal2);
	face_index++;
      }
    }

    (*filename_metadata)[f_string].num_of_vertices = num_vertices;
    (*this->filename_metadata)[f_string].num_of_indices = face_index * 3;
    this->vn_offset = this->vertices->size();
    this->idx_offset = this->indices->size();
    this->flat_offset += face_index * 9;
    cout << "vn_offset: " << this->vn_offset << endl;

    if (getline(source, line)) {
      cout << "Warning: extra lines in " << filename << " ignored" << endl;
    }
  }
}


glm::vec3 meshManager::calc_face_normal(GLuint v0, GLuint v1, GLuint v2) {
  glm::vec3 vertex_0 = glm::vec3((*this->vertices)[v0 * 3], (*this->vertices)[v0 * 3 + 1], (*this->vertices)[v0 * 3 + 2]);
  glm::vec3 vertex_1 = glm::vec3((*this->vertices)[v1 * 3], (*this->vertices)[v1 * 3 + 1], (*this->vertices)[v1 * 3 + 2]);
  glm::vec3 vertex_2 = glm::vec3((*this->vertices)[v2 * 3], (*this->vertices)[v2 * 3 + 1], (*this->vertices)[v2 * 3 + 2]);
  glm::vec3 vector0 = vertex_2 - vertex_1;
  glm::vec3 vector1 = vertex_0 - vertex_1;
  return glm::normalize(glm::cross(vector0, vector1));
}


void meshManager::calc_normal(string filename) {
  metadata md = (*this->filename_metadata)[filename];
  for(GLuint i = 0; i < md.num_of_vertices; i++) {
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

  for (GLuint i = 0; i < md.num_of_indices / 3; i++) {
    glm::vec3 normal = (*this->face_normals)[i];
    for (int i = 0; i < 3; i++) {
      (*this->flat_normals).push_back(normal.x);
      (*this->flat_normals).push_back(normal.y);
      (*this->flat_normals).push_back(normal.z);
    }
  }

  //Do this after all flat and smooth normals are calculated
  this->face_normals->clear();
}

void meshManager::init() {
  this->vbo_pos = make_bo(GL_ARRAY_BUFFER,
			  &(this->vertices->front()),
			  this->vertices->size() * sizeof(GLfloat));

  this->vbo_normal = make_bo(GL_ARRAY_BUFFER,
			     &(this->normals->front()),
			     this->normals->size() * sizeof(GLfloat));

  this->ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER,
		      &(this->indices->front()),
		      this->indices->size() * sizeof(GLuint));
  this->flat_vbo_pos = make_bo(GL_ARRAY_BUFFER,
			       &(this->flat_vertices->front()),
			       this->flat_vertices->size() * sizeof(GLfloat));
  glGenVertexArrays(1, &(this->vao));
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_pos);
  GLuint pos = glGetAttribLocation(fs_shader, "vPos");
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normal);
  GLuint normal = glGetAttribLocation(fs_shader, "vNormal");
  glEnableVertexAttribArray(normal);
  glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

}

void meshManager::draw_default() {
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  for(GLuint i = 0; i < this->draw_sequence.size(); i++) {
    string filename = this->draw_sequence[i];
    metadata md = (*this->filename_metadata)[filename];
    glm::mat4 model_mat = md.model_mat * glm::rotate(angle, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 modelview_mat = view_mat * model_mat;
    glUniformMatrix4fv(modelview, 1, GL_FALSE, glm::value_ptr(modelview_mat));
    glDrawElements(GL_TRIANGLES, md.num_of_indices, GL_UNSIGNED_INT, (void*) (md.indices_offset * sizeof(GLuint)));
  }
}

void meshManager::draw_vertex_mode() {
  glBindVertexArray(this->vao);
  for (GLuint i = 0; i < this->draw_sequence.size(); i++) {
    string filename = this->draw_sequence[i];
    metadata md = (*this->filename_metadata)[filename];
    glDrawArrays(GL_POINTS, md.vn_offset, md.vn_offset + md.num_of_vertices * sizeof(GLfloat));
  }
}

void meshManager::draw() {
  switch(mode) {
  case EDGE:
  case FACE:
    this->draw_default();
    break;
  case VERTEX:
    this->draw_vertex_mode();
    break;
  }
}
