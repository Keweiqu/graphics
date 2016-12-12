#include "util.hpp"

unsigned char convert_rgb(float c) {
  float clamped = clamp(c, 0, 1);
  return (unsigned char)(clamped * 255);
}

float clamp(float f, float lower, float upper) {
  return max(lower, min(f, upper));
}



void init_image() {
  for (int i = 0; i < width * height; i++) {
    Color* c = new Color(0.5, 0.5, 0.5);
    pixels.push_back(c);
  }
}

void dump_image() {
  ofstream output;
  output.open("output.ppm", ios::binary);
  if (output.is_open()) {
    output << "P6\n" << width << " " << height << "\n" << 255 << "\n";
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        Color* c = pixels[i * width + j];
        output << convert_rgb(c->rgb[0]) << convert_rgb(c->rgb[1]) << convert_rgb(c->rgb[2]);
      }
    }
  } else {
    cout << "Fail to open output file" << endl;
  }
  output.close();
}
