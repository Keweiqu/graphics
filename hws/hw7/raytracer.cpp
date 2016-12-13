#include "raytracer.hpp"

Ray compute_ray(int i, int j) {
  float px = width * (j / (float) nCols) - width / 2.0;
  float py = -height * (i / (float) nRows) + height / 2.0;
  float pz = -1.0;
  //vec3 dest = vec3(px, py, pz);
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
    cout << "intersection normal " << r.intersect_normal[0] << " " << r.intersect_normal[1] << " " << r.intersect_normal[2] << endl;
    return lit(r);
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
	if (t_s > 0 && r.t > t_s) {
	  r.t = t_s;
	  r.intersect_obj_index = i;
	  r.intersect_point = r.o + r.d * r.t;
    //cout << "intersection point " << r.intersect_point.x << " " << r.intersect_point.y << " " << r.intersect_point.z << endl;
	  r.intersect_normal = sphere_normal(sphere, r.intersect_point);
	  intersect = INTERSECT;
	}
	break;
      }
      case PLANE:
      {
        Plane plane = obj->plane;
        float t_p = plane_intersect(r, plane);
        if (t_p > 0 && r.t > t_p) {
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
  // vec3 co = r.o - sphere.center;
  // float A = r.d * r.d;
  // float B = 2 * (co * r.d);
  // float C = (co * co) - pow(sphere.radius, 2);
  // float delta = pow(B, 2) - 4 * A * C;
  // if(delta < 0) {
  //   return -1;
  // }
  //
  // float t1 = (-B + sqrt(delta)) / (2 * A);
  // float t2 = (-B - sqrt(delta)) / (2 * A);
  //
  // if(t1 < EPSILON && t2 < EPSILON) {
  //     return FLT_MAX;
  // }
  //
  // if(t1 < EPSILON) return t2;
  // if(t2 < EPSILON) return t1;
  // return fmin(t1, t2);
  //



  vec3 co = r.o - sphere.center;
  double a = r.d * r.d;
  double b = 2 * ((r.o - sphere.center) * r.d);
  double c = (co * co) - pow(sphere.radius, 2);
//  double b = 2* glm::dot(r.origin - center, r.dir);
//  double c = pow( glm::distance(r.origin, center) ,2) - pow( radius ,2);

  double delta = pow(b, 2) - 4*a*c;
  if(delta < 0){return -1;}
  else if(delta == 0){
      double result = -b/(double)(2*a);
      if(result > 0){
        /*
        if(result < 3) {
          cout << "Ray " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
          cout << "Radius " << sphere.radius << endl;
          cout << "Sphere center " << sphere.center.x << " " << sphere.center.y  <<" " << sphere.center.z << endl;
          cout << "co " << co[0] << " " << co[1] << " " << co[2] << endl;
          cout << "a " << a << " b " << b << " c " << c << endl;
          cout << "return is " << result << endl;
        }
        */
        return result;
    }
  }else{
      double result1 = (-b + sqrt(delta))/(double)(2*a);
      double result2 = (-b - sqrt(delta))/(double)(2*a);

      if( result1 < 0 && result2 < 0){
          return -1;
      }else if( result1 < 0 && result2 > 0){
        /*
        if(result2 < 3) {
          cout << "=====================================" << endl;
          cout << "Ray " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
          cout << "Radius " << sphere.radius << endl;
          cout << "Sphere center " << sphere.center.x << " " << sphere.center.y  <<" " << sphere.center.z << endl;
          cout << "co " << co[0] << " " << co[1] << " " << co[2] << endl;
          cout << "a " << a << " b " << b << " c " << c << endl;
          cout << "return2 is " << result2 << endl;
        }
        */
          return result2;
      }else if( result1 > 0 && result2 < 0){
        /*
        if(result1 < 3) {
          cout << "=====================================" << endl;
          cout << "Ray " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
          cout << "Ray origin is " << r.o.x << " " << r.o.y << " " << r.o.z << endl;
          cout << "Radius " << sphere.radius << endl;
          cout << "Sphere center " << sphere.center.x << " " << sphere.center.y  <<" " << sphere.center.z << endl;
          cout << "co " << co[0] << " " << co[1] << " " << co[2] << endl;
          cout << "a " << a << " b " << b << " c " << c << endl;
          cout << "result1 is " << result1 << endl;
        }
        */
          return result1;
      }else{
        /*
        if(min(result1, result2) < 3) {
          cout << "=====================================" << endl;
          cout << "Ray " << r.d[0] << " " << r.d[1] << " " << r.d[2] << endl;
          cout << "Radius " << sphere.radius << endl;
          cout << "Sphere center " << sphere.center.x << " " << sphere.center.y  <<" " << sphere.center.z << endl;
          cout << "co " << co[0] << " " << co[1] << " " << co[2] << endl;
          cout << "a " << a << " b " << b << " c " << c << endl;
          cout << "result1 is " << result1 << " " << "result2 is " << result2 << endl;
          cout << "result min is " << min(result1, result2) << endl;
        }
        */
          return min(result1, result2);
      }

  }

  return -1;
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
