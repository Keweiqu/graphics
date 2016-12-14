#include "raytracer.hpp"

Ray compute_ray(int i, int j) {
  float px = width * (j / (float) nCols) - width / 2.0;
  float py = -height * (i / (float) nRows) + height / 2.0;
  float pz = -1.0;
  vec3 dest = camera.o + camera.x * px + camera.y * py + camera.z * pz;
  vec3 source = camera.o;
  vec3 direction = vec3::normalize(dest - source);
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
    return lit(r, depth);
  }

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
	if (t_s > EPSILON && r.t > t_s) {
	  r.t = t_s;
	  r.intersect_obj_index = i;
	  r.intersect_point = r.o + r.d * r.t;
	  r.intersect_normal = sphere_normal(sphere, r.intersect_point);
	  intersect = INTERSECT;
	}
	break;
      }
      case PLANE:
      {
        Plane plane = obj->plane;
        float t_p = plane_intersect(r, plane);
        if (t_p > EPSILON && r.t > t_p) {
          r.t = t_p;
          r.intersect_obj_index = i;
          r.intersect_point = r.o + r.d * r.t;
          r.intersect_normal = plane_normal(plane);
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

Color lit(Ray r, int depth) {
  Color local = Color(0.0, 0.0, 0.0);
  Color reflect = Color(0.0, 0.0, 0.0);
  Object* obj = objects[r.intersect_obj_index];
  if(depth == 0) {
    local = local + (lights[0].intensity * 0.2 * obj->sf.cof[AMBIENT]);
  }
  for(unsigned int i = 1; i < lights.size(); i++) {
    Light light = lights[i];
    if(visible(r.intersect_point, light)) {
      local = local + phong(light, r.intersect_point, obj, r.intersect_normal);

    }
  }
  if(obj->sf.cof[REFLECT] > 0) {
    // cout << "r.d " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
    vec3 reflect_dir = vec3::reflect(r.d, r.intersect_normal);
    //vec3 reflect_dir = vec3(0.0, 0.0, 0.0);
    //cout << "reflect_dir " << reflect_dir[0] << " " << reflect_dir[1] << " " << reflect_dir[2] << endl;
    Ray reflectRay = Ray(r.intersect_point, reflect_dir);
    reflect = trace(reflectRay, depth + 1) * obj->sf.cof[REFLECT];
    //reflect = Color(vec3(1.0, 0.0, 0.0));
    //cout << "reflect color is " << reflect.rgb[0] << " " << reflect.rgb[1] << " " << reflect.rgb[2] << endl;
  }
  return local + reflect;
}

int visible(Point p, Light light) {
  Ray shadow_ray = Ray(p, light.coord);
  for(unsigned int i = 0; i < objects.size(); i++) {
    if(intersect(shadow_ray) == INTERSECT) {
      return FALSE;
    }
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

   if(t1 < EPSILON && t2 < EPSILON) {
       return -1;
   }

   if(t1 < EPSILON) return t2;
   if(t2 < EPSILON) return t1;
   return fmin(t1, t2);
}


vec3 sphere_normal(Sphere sphere, Point p) {
  return vec3::normalize(p - sphere.center);
}

float plane_intersect(Ray& r, Plane plane) {
  vec3 n = plane_normal(plane);
  return -(vec3(r.o.x, r.o.y, r.o.z)* n + plane.d) / (r.d * n);
}

vec3 plane_normal(Plane plane) {
  return vec3::normalize(vec3(plane.a, plane.b, plane.c));
}
