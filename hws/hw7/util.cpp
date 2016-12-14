#include "util.hpp"

unsigned char convert_rgb(float c) {
  float clamped = clamp(c, 0, 1);
  return (unsigned char)(clamped * 255);
}

Color background_color() {
  return Color(0.5, 0.5, 0.5);
}

float clamp(float f, float lower, float upper) {
  return max(lower, min(f, upper));
}

void init_image() {
  for (int i = 0; i < nCols * nRows; i++) {
    Color* c = new Color(0.5, 0.5, 0.5);
    pixels.push_back(c);
  }
}

void dump_image() {
  ofstream output;
  output.open(out_name, ios::binary);
  if (output.is_open()) {
    output << "P6\n" << nCols << " " << nRows << "\n" << 255 << "\n";
    for (int i = 0; i < nRows; i++) {
      for (int j = 0; j < nCols; j++) {
        Color* c = pixels[i * nCols + j];
        output << convert_rgb(c->rgb[0]) << convert_rgb(c->rgb[1]) << convert_rgb(c->rgb[2]);
      }
    }
  } else {
    cout << "Fail to open output file" << endl;
  }
  output.close();
}

void write_pixel(int i, int j, Color c) {
  int index = i * nCols + j;
  *(pixels[index]) = c;
}


Color phong(Light light, Point point, Object* obj, vec3 normal) {
  vec3 l = vec3::normalize(light.coord - point);
  vec3 v = vec3::normalize(camera.o - vec3(point.x, point.y, point.z));
  vec3 n = vec3::normalize(normal);
  vec3 r = vec3::normalize(n * (2 * (l * n)) - l);

  float kd = obj->sf.cof[DIFFUSE];
  vec3 Ld = light.intensity.rgb;
  float attenu = attenuation(light, point);
  float diffuse = fmax((l*n), 0.0);
  vec3 Id = Ld * kd * diffuse * attenu;
  vec3 diffuseProduct = vec3::dot(Id, map_color(obj, point).rgb);
  // cout << "diffuseProduct: " << diffuseProduct[0] << " " << diffuseProduct[1] << " " << diffuseProduct[2] << endl;
  // vec3 c = obj->p.solid.color.rgb;
  // cout << "color: " << c[0] << " " << c[1] << " " << c[2] << endl;

  float ks = obj->sf.cof[SPECULAR];
  vec3 Ls = vec3(1.0, 1.0, 1.0);
  float alpha = obj->sf.cof[SHININESS];
  vec3 Is = Ls * (ks * pow(fmax(v * r, 0.0), alpha));

  vec3 color = diffuseProduct + Is;
  return Color(color);
}

Color map_color(Object* obj, Point point) {
  Pigment pigment = obj->p;
  Color c(0.0, 0.0, 0.0);
  if (pigment.type == SOLID) {
    c = pigment.solid.color;
  } else if (pigment.type == CHECKER) {
    float size = pigment.checker.size;
    int sum = floor(point.x / size) + floor(point.y / size) + floor(point.z / size);
    c = sum % 2 == 0 ? pigment.checker.c0 : pigment.checker.c1;
  }
  return c;
}

float attenuation(Light light, Point point) {
  float a = light.attenu[0], b = light.attenu[1], c = light.attenu[2];
  float d = (light.coord - point).len();
  float denom = a + b * d + c * pow(d, 2);
  if (abs(denom - 0) < EPSILON) {
    denom = EPSILON;
  }
  return 1 / denom;
}

void free_pixels() {
  for (unsigned int i = 0; i < pixels.size(); i++) {
    delete pixels[i];
  }
}

void free_objects() {
  for (unsigned int i = 0; i < objects.size(); i++) {
    delete objects[i];
  }
}
