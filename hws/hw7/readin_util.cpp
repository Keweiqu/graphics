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
    Color intensity(i_r, i_g, i_b);
    glm::vec3 attenuation = glm::vec3(a, b, c);
    Light light(coord, intensity, attenuation);
    lights.push_back(light);
  }

  /*Pigments*/
  get_next_line(source, line, stream);
  int num_pigments;
  stream >> num_pigments;
  for (int i = 0; i < num_pigments; i++) {
    Pigment pigment;
    string type;
    get_next_line(source, line, stream);
    stream >> type;
    if (type == "solid") {
      float r, g, b;
      stream >> r >> g >> b;
      Solid solid(r, g, b);
      pigment.type = SOLID;
      pigment.solid = solid;
      pigments.push_back(pigment);
    } else if (type == "checker") {
      float r0, g0, b0, r1, g1, b1, s;
      stream >> r0 >> g0 >> b0 >> r1 >> g1 >> b1 >> s;
      Checker checker(r0, g0, b0, r1, g1, b1, s);
      pigment.type = CHECKER;
      pigment.checker = checker;
      pigments.push_back(pigment);
    }
  }

  /*Surface finishes*/
  get_next_line(source, line, stream);
  int num_surface_finishes;
  stream >> num_surface_finishes;
  for (int i = 0; i < num_surface_finishes; i++) {
    float ka, kd, ks, alpha, kr, kt, et;
    get_next_line(source, line, stream);
    stream >> ka >> kd >> ks >> alpha >> kr >> kt >> et;
    SurfaceFinish sf(ka, kd, ks, alpha, kr, kt, et);
    surface_finishes.push_back(sf);
  }

  /*Transformations*/
  get_next_line(source, line, stream);
  int num_transformations;
  stream >> num_transformations;
  for (int i = 0; i < num_transformations; i++) {
    string trans_type;
    float x, y, z;
    get_next_line(source, line, stream);
    stream >> trans_type >> x >> y >> z;
    TransType tt = trans_type == "scale" ? SCALE : TRANSLATE;
    Trans trans(tt, x, y, z);
    trans_vecs.push_back(trans);
  }

  /*Objects*/
  get_next_line(source, line, stream);
  int num_objects;
  stream >> num_objects;
  for (int i = 0; i < num_objects; i++) {
    get_next_line(source, line, stream);
    int pig_id, sf_id, num_trans, trans_id;
    string obj_name;
    stream >> pig_id >> sf_id >> num_trans;
    Object* obj = new Object(pigments[pig_id], surface_finishes[sf_id]);

    for (int i = 0; i < num_trans; i++) {
      stream >> trans_id;
      obj->addTrans(trans_vecs[trans_id]);
    }

    stream >> obj_name;
    if (obj_name == "sphere") {
      float cx, cy, cz, r;
      stream >> cx >> cy >> cz >> r;
      Sphere sphere(cx, cy, cz, r);
      obj->ot = SPHERE;
      obj->sphere = sphere;
    } else if (obj_name == "plane") {
      float a, b, c, d;
      stream >> a >> b >> c >> d;
      Plane plane(a, b, c, d);
      obj->ot = PLANE;
      obj->plane = plane;
    } else if (obj_name == "polyhedron") {
      Polyhedron polyhedron;
      int num_faces;
      stream >> num_faces;
      float a, b, c, d;
      for (int j = 0; j < num_faces; j++) {
        get_next_line(source, line, stream);
        stream >> a >> b >> c >> d;
        Plane plane(a, b, c, d);
        polyhedron.addPlane(plane);
      }
      obj->ot = POLYHEDRON;
      obj->polyhedron = polyhedron;
    }

    objects.push_back(obj);
  }
}


void get_next_line(ifstream& source, string line, stringstream& stream) {
  getline(source, line);
  stream.clear();
  stream.str(line);
}
