#include "readin_util.hpp"
void readin(char* file) {
  ifstream source;
  source.open(file);
  string line;

  /*Output file name and antialias*/
  getline(source, line);
  if(line == "antialiasing") {
    antialias = TRUE;
    getline(source, line);
    out_name = line;
  } else {
    out_name = line;
  }

  /*nCols and nRows*/

  getline(source, line);
  stringstream stream(line);
  stream >> nCols >> nRows;

  /*camera*/
  float x, y, z;
  get_next_line(source, line, stream);
  stream >> x >> y >> z;
  vec3 eye = vec3(x, y, z);
  get_next_line(source, line, stream);
  stream >> x >> y >> z;
  vec3 look = vec3(x, y, z);
  get_next_line(source, line, stream);
  stream >> x >> y >> z;
  vec3 up = vec3(x, y, z);
  camera = Camera(eye, look, up);

  /*fovy + width and height*/
  get_next_line(source, line, stream);
  stream >> fovy_deg;
  float aspectRadio = (float) nCols / (float) nRows;
  height = 2 * tan(fovy_deg * TO_RAD / 2);
  width = height * aspectRadio;

  /*Lights*/
  get_next_line(source, line, stream);
  int num_lights;
  stream >> num_lights;
  for(int i = 0; i < num_lights; i++) {
    float x, y, z, i_r, i_g, i_b, a, b, c;
    get_next_line(source, line, stream);
    stream >> x >> y >> z >> i_r >> i_g >> i_b >> a >> b >> c;
    Point coord = Point(x, y, z);
    Color intensity(i_r, i_g, i_b);
    vec3 attenuation = vec3(a, b, c);
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
    } else if(type == "image") {
      string image_file;
      stream >> image_file;
      const char* file = image_file.c_str();
      Image img;
      if(read_ppm(file, &img) == FALSE) {
	cout << "Bad image input!" << endl;
	exit(EXIT_FAILURE);
      }
      pigment.type = IMAGE;
      pigment.image = img;
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
      int num_faces;
      stream >> num_faces;
      Polyhedron poly(num_faces, planes.size());
      float a, b, c, d;
      for (int j = 0; j < num_faces; j++) {
        get_next_line(source, line, stream);
        stream >> a >> b >> c >> d;
        Plane* plane = new Plane(a, b, c, d);
        planes.push_back(plane);
      }
      obj->ot = POLYHEDRON;
      obj->polyhedron = poly;
    } else if (obj_name == "trianglemesh") {
      string filename;
      stream >> filename;
      TriangleMesh mesh;
      readoff(mesh, filename);
      obj->ot = MESH;
      obj->mesh = mesh;
    }
    obj->apply_trans();
    objects.push_back(obj);
  }
}

void readoff(TriangleMesh& mesh, string filename) {
  ifstream source;
  source.open(filename);
  string line;

  getline(source, line);
  if (line != "OFF") {
    cout << "wrong file format" << endl;
    return;
  }

  stringstream stream(line);
  get_next_line(source, line, stream);
  int num_vertices, num_faces, num_edges;
  stream >> num_vertices >> num_faces >> num_edges;

  vector<vec3> vertices;
  for (int i = 0; i < num_vertices; i++) {
    get_next_line(source, line, stream);
    float x, y, z;
    stream >> x >> y >> z;
    vec3 vertex = vec3(x, y, z);
    vertices.push_back(vertex);
  }

  mesh.start_index = triangles.size();
  mesh.end_index = mesh.start_index + num_faces - 1;
  mesh.num_faces = num_faces;

  for (int i = 0; i < num_faces; i++) {
    get_next_line(source, line, stream);
    int num_indices, index1, index2, index3;
    stream >> num_indices >> index1 >> index2 >> index3;
    Triangle* triangle = calc_cof(vertices[index1], vertices[index2], vertices[index3]);
    triangles.push_back(triangle);
  }
  source.close();
}


void get_next_line(ifstream& source, string line, stringstream& stream) {
  getline(source, line);
  stream.clear();
  stream.str(line);
}

void readin_debugg_output() {
  cout << "antialias? " << antialias << endl;
  cout << "out file name is " << out_name << endl;
  cout << "width " << width << " height " << height << endl;
  cout << "Camera center is " << camera.o[0] << " " << camera.o[1] << " " << camera.o[2] << endl;
  cout << "Camera x is " << camera.x[0] << " " << camera.x[1] << " " << camera.x[2] << endl;
  cout << "Camera y is " << camera.y[0] << " " << camera.y[1] << " " << camera.y[2] << endl;
  cout << "Camera z is " << camera.z[0] << " " << camera.z[1] << " " << camera.z[2] << endl;
  cout << "fovy is " << fovy_deg << endl;
  cout << "There are " << objects.size() << " objects" << endl;

  for (unsigned int i = 0; i < pigments.size(); i++) {
    Pigment p = pigments[i];
    cout << p.toString() << endl;
  }

  for (unsigned int i = 0; i < surface_finishes.size(); i++) {
    SurfaceFinish sf = surface_finishes[i];
    cout << sf.toString() << endl;
  }

  for (unsigned int i = 0; i < trans_vecs.size(); i++) {
    cout << trans_vecs[i].toString() << endl;
  }

  for (unsigned int i = 0; i < objects.size(); i++) {
    Object* obj = objects[i];
    cout << "object: " << obj->toString() << "\n" << endl;
  }
}
