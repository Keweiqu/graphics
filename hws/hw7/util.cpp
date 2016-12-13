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
  vec3 r = vec3::normalize(n * 2 * (l * n) - l);

  float ka = obj->sf.cof[AMBIENT];
  vec3 La = lights[0].intensity.rgb;
  vec3 Ia = La * ka;

  float kd = obj->sf.cof[DIFFUSE];
  vec3 Ld = light.intensity.rgb;
  float attenu = attenuation(light, point);
  vec3 Id = Ld * kd * (l * n) * attenu;

  float ks = obj->sf.cof[SPECULAR];
  vec3 Ls = light.intensity.rgb;
  float alpha = obj->sf.cof[SHININESS];
  vec3 h = vec3::normalize(l + v);
  vec3 Is = Ls * (ks * pow(fmax(n * h, 0.0), alpha));
  // cout << "v*r: " << v * r << endl;
  // cout << "alpha: " << alpha << endl;
  // cout << "ks: " << ks << endl;
  // cout << "Ls: " << Ls[0] << " " << Ls[1] << " " << Ls[2] << endl;
  // cout << "intersection point: " << point.x << " " << point.y << " " << point.z << endl;
  // cout << "normal: " << normal[0] << " " << normal[1] << " " << normal[2] << " " << endl;
  // cout << "h: " << h[0] << " " << h[1] << " " << h[2] << endl;
  // cout << "ray dir: " << -v[0] << " " << -v[1] << " " << -v[2] << endl;
  // if(Is[0] > 0) {
    // cout << "Is: " << Is[0] << " " << Is[1] << " " << Is[2] << endl;
  // }

  vec3 I = Ia + Id + Is;
  return Color(I) * (obj->p.solid.color);
}

float attenuation(Light light, Point point) {
  float a = light.attenu[0], b = light.attenu[1], c = light.attenu[2];
  float d = (light.coord - point).len();
  float denom = 1 / (a + b * d + c * pow(d, 2));
  if (abs(denom - 0.0001) < 0) {
    return abs(denom - 0.0001);
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
