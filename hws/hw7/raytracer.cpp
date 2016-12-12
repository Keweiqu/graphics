#include "raytracer.hpp"

Ray compute_ray(int i, int j) {
  float px = width * (j / (float) nCols) - width / 2.0;
  float py = -height * (i / (float) nRows) + height / 2.0;
  float pz = -1.0;
  vec3 dest = vec3(px, py, pz);
  vec3 source = camera.o;
  vec3 direction = vec3::normalize(dest - source);
  Ray r = Ray(source, direction);
  return r;
}

Color trace(Ray r, int depth) {
  if(depth > MAX_DEPTH) {
    return background_color();
  }
  Status status;
  vec3 normal;
  int obj_index;
  Point p = intersect(r, status, normal, obj_index);
  if(status == INTERSECT) {

  }
  //for each light source, compute!
  return background_color();
}

Point intersect(Ray& r, Status& status, vec3& normal, int& obj_index) {
  float t = FLT_MAX;
  for(unsigned int i = 0; i < objects.size(); i++) {
    Object* obj = objects[i];
    switch(obj->ot) {
    case SPHERE:{
      Sphere sphere = obj->sphere;
      float t_s = sphere_intersect(r, sphere);
      if (t_s > 0 && t < t_s) {
	t = t_s;
	obj_index = i;
      }
      return r.o + r.d * t;
      break;}
    default:
      break;
    }
  }
  return Point();
}

void print_ray(Ray r) {
  cout << "===============Print Ray==============" << endl;
  cout << "origin is " << r.o.x  << " " << r.o.y << " " << r.o.z << endl;
  cout << "direction is " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
  cout << "t is " << r.t << endl;
}


float sphere_intersect(Ray r, Sphere sphere) {
  Point ro = r.o;
  vec3 co = sphere.center - ro;
  float A = r.d * r.d;
  float B = 2 * (co * r.d);
  float C = co * co;
  float delta = pow(B, 2) - 4 * A * C;
  if(delta < 0) {
    return -1;
  }

  float t1 = (-B + sqrt(delta)) / (2 * A);
  float t2 = (-B - sqrt(delta)) / (2 * A);
  
  if(abs(delta - 0) < 0.00001) {
    if(t1 < 0 && t2 < 0 && B > 0) {
      return -1;
    } else {
      return -B / (2 * A);
    }
  }
  
 
  if(t1 < 0 && t2 < 0) {
    return -1;
  }

  float tmin = min(t1, t2);
  float tmax = max(t1, t2);
  if(tmin < 0) {
    return tmax;
  } else {
    return tmin;
  }
}


vec3 sphere_normal(Sphere sphere, Point p) {
  return vec3(0.0);
}
