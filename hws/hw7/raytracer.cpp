#include "raytracer.hpp"

Ray compute_ray(int i, int j) {
  float px = width * (j / (float) nCols) - width / 2.0;
  float py = -height * (i / (float) nRows) + height / 2.0;
  float pz = -1.0;
  //vec3 dest = vec3(px, py, pz);
  vec3 dest = camera.o + camera.x * px + camera.y * py + camera.z * pz;
  vec3 source = camera.o;
  vec3 direction = vec3::normalize(dest - source);
  //cout << "ray dir is " << direction[0] << " " << direction[1] << " " << direction[2] << endl;
  Ray r = Ray(source, direction);
  return r;
}

Color trace(Ray r, int depth) {
  if(depth > MAX_DEPTH) {
    return background_color();
  }

  if((intersect(r) == NO_INTERSECT)) {
    return background_color();
  } else {
    return lit(r);
  }

  cout << "Tracer has intersection" << endl;
  return lit(r);
}

Status intersect(Ray& r) {
  Status intersect = NO_INTERSECT;
  for(unsigned int i = 0; i < objects.size(); i++) {
    Object* obj = objects[i];
    switch(obj->ot) {
    case SPHERE:
      {
	Sphere sphere = obj->sphere;
	float t_s = sphere_intersect(r, sphere);
	if (t_s > 0 && r.t > t_s) {
	  r.t = t_s;
	  r.intersect_obj_index = i;
	  r.intersect_point = r.o + r.d * r.t;
	  r.intersect_normal = sphere_normal(sphere, r.intersect_point);
	  intersect = INTERSECT;
	}
	break;
      }
    default:
      break;
    }
  }
  return intersect;
}

Color lit(Ray r) {
  Color local = Color(0.0, 0.0, 0.0);
  for(unsigned int i = 1; i < lights.size(); i++) {
    Light light = lights[i];
    if(visible(r.intersect_point, light)) {
    	local = local + phong(light, r.intersect_point, objects[r.intersect_obj_index], r.intersect_normal);
    }
  }
  return local;
}

int visible(Point p, Light light) {
  Ray shadow_ray = Ray(p, light.coord);
  if(intersect(shadow_ray) == INTERSECT) {
    return FALSE;
  }
  return TRUE;
}

void print_ray(Ray r) {
  cout << "===============Print Ray==============" << endl;
  cout << "origin is " << r.o.x  << " " << r.o.y << " " << r.o.z << endl;
  cout << "direction is " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
  cout << "t is " << r.t << endl;
}


float sphere_intersect(Ray& r, Sphere sphere) {
  vec3 co = r.o - sphere.center;
  float A = r.d * r.d;
  float B = 2 * (co * r.d);
  float C = (co * co) - pow(sphere.radius, 2);
  float delta = pow(B, 2) - 4 * A * C;
  if(delta < 0) {
    return -1;
  }

  float t1 = (-B + sqrt(delta)) / (2 * A);
  float t2 = (-B - sqrt(delta)) / (2 * A);

  if(abs(delta - 0) < 0.0001) {
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
  return vec3::normalize(p - sphere.center);
}
