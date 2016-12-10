#include "readin_util.hpp"
void readin(char* file) {
  ifstream source;
  source.open(file);
  string line;
  
  /*Output file name and antialias*/
  getline(source, line);
  if(line == "antialias") {
    antialias = TRUE;
    getline(source, line);
    out_name = line;
  } else {
    out_name = line;
  }
  
  /*width and height*/

  getline(source, line);
  stringstream stream(line);
  stream >> width >> height;

  /*camera*/
  for(int i = 0; i < 3; i++) {
    float x, y, z;
    get_next_line(source, line, stream);
    stream >> x >> y >> z;
    switch(i) {
    case 0:
      eye = glm::vec3(x, y, z);
      break;
    case 1:
      look = glm::vec3(x, y, z);
      break;
    case 2:
      up = glm::vec3(x, y, z);
      break;
    default:
      break;
    }
  }

  /*fovy*/
  get_next_line(source, line, stream);
  stream >> fovy_deg;

  /*Lights*/
  get_next_line(source, line, stream);
  int num_lights;
  stream >> num_lights;
  for(int i = 0; i < num_lights; i++) {
    float x, y, z, i_r, i_g, i_b, a, b, c;
    get_next_line(source, line, stream);
    stream >> x >> y >> z >> i_r >> i_g >> i_b >> a >> b >> c;
    glm::vec3 coord = glm::vec3(x, y, z);
    glm::vec3 intensity = glm::vec3(i_r, i_g, i_b);
  }
}


void get_next_line(ifstream& source, string line, stringstream& stream) {
  getline(source, line);
  stream.clear();
  stream.str(line);
}
