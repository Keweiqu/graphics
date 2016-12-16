#include "raytracer.hpp"

Ray compute_ray(float i, float j) {
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
	  r.intersect_normal = sphere_normal(sphere, r.intersect_point, r);
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
      case POLYHEDRON:
      {
        Polyhedron poly = obj->polyhedron;
	int plane_index = -1;
        float t_p = polyhedron_intersect(r, poly, plane_index);
        if (plane_index != -1 && t_p > EPSILON && r.t > t_p) {
	  r.t = t_p;
	  r.intersect_obj_index = i;
	  r.intersect_point = r.o + r.d * r.t;
	  r.intersect_normal = plane_normal(*(planes[plane_index]));
	  intersect = INTERSECT;
        }
        break;
      }
    case MESH:
      {
	TriangleMesh mesh = obj->mesh;
	int triangle_index = -1;
	float t_t = trianglemesh_intersect(r, mesh, triangle_index);
	if(triangle_index != -1 && t_t > EPSILON && r.t > t_t) {
	  r.t = t_t;
	  r.intersect_obj_index = i;
	  r.intersect_point = r.o + r.d * r.t;
	  Triangle *tri = triangles[triangle_index];
	  r.intersect_normal = vec3::normalize(vec3(tri->a, tri->b, tri->c));
	  intersect = INTERSECT;
	}
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
  Color refract = Color(0.0, 0.0, 0.0);
  Object* obj = objects[r.intersect_obj_index];
  if(depth == 0) {
    local = local + (lights[0].intensity * 0.2 * obj->sf.cof[AMBIENT]);
  }
  for(unsigned int i = 1; i < lights.size(); i++) {
    Light light = lights[i];
    if(visible(r.intersect_point, light)) {
      vec3 normal = r.intersect_normal;
      if(inside_obj(r.o, obj) && inside_obj(light.coord, obj)) {
	normal = normal * -1;
      }
      local = local + phong(light, r.intersect_point, obj, normal);
    }
  }
  if(obj->sf.cof[REFLECT] > 0) {
    glm::vec3 ray_dir = glm::normalize(glm::vec3(r.d[0], r.d[1], r.d[2]));
    glm::vec3 normal = glm::normalize(glm::vec3(r.intersect_normal[0], r.intersect_normal[1], r.intersect_normal[2]));
    glm::vec3 refl = glm::reflect(ray_dir, normal);
    vec3 reflect_dir = vec3::normalize(vec3(refl[0], refl[1], refl[2]));
    vec3 p = vec3(r.intersect_point.x, r.intersect_point.y, r.intersect_point.z);
    Ray reflectRay = Ray(p, reflect_dir);
    reflect = trace(reflectRay, depth + 1) * obj->sf.cof[REFLECT];
  }

  if(obj->sf.cof[TRANSMIT] > 0) {
    int inside = FALSE;
    vec3 normal = r.intersect_normal;
    if((r.d *r.intersect_normal) > 0) {
      inside = TRUE;
      normal = normal * -1;
    }
   
    float n1 = 1; //air
    float n2 = obj->sf.cof[REFRACT]; //obj material
    float n = inside ? n2 / n1 : n1 / n2;
    float c1 = (r.d * -1) * normal;
    float c2 = 1 - (n * n * (1 - c1 * c1));
    vec3 refra_dir = (r.d * n) + (normal * (n * c1 - sqrt(c2)));
    vec3 p = vec3(r.intersect_point.x, r.intersect_point.y, r.intersect_point.z);
    Ray refractRay = Ray(p, refra_dir);
    refract = trace(refractRay, depth + 1) * obj->sf.cof[TRANSMIT];
  }
  return local + reflect + refract;
}

int visible(Point p, Light light) {
  Ray shadow_ray = Ray(p, light.coord);
  for(unsigned int i = 0; i < objects.size(); i++) {
    if(intersect(shadow_ray) == INTERSECT) {
      if((light.coord - p).len() < shadow_ray.t) {
	return TRUE;
      }
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


vec3 sphere_normal(Sphere sphere, Point p, Ray& r) {
  return vec3::normalize(p - sphere.center);
}

float plane_intersect(Ray& r, Plane plane) {
  vec3 n = plane_normal(plane);
  return (vec3(r.o.x, r.o.y, r.o.z) * n + plane.d) / (r.d * n) * -1;
}

vec3 plane_normal(Plane plane) {
  return vec3::normalize(vec3(plane.a, plane.b, plane.c));
}

float polyhedron_intersect(Ray& r, Polyhedron poly, int& plane_index) {
  float t = FLT_MAX;
  for (unsigned int i = poly.start_index; i < poly.start_index + poly.num_faces; i++) {
    float cur = plane_intersect(r, *(planes[i]));
    if (cur > EPSILON && cur < t) {
      Point p = r.o + r.d * cur;
      if(inside_poly(p, poly)) {
	t = cur;
	plane_index = i;
      }
    }
  }
  return t;
}


float trianglemesh_intersect(Ray& r, TriangleMesh mesh, int& triangle_index) {
  float t = FLT_MAX;
  for(unsigned int i = mesh.start_index; i < mesh.start_index + mesh.num_faces; i++) {
    Triangle triangle = *(triangles[i]);
    Plane plane = Plane(triangle.a, triangle.b, triangle.c, triangle.d);
    float cur = plane_intersect(r, plane);
    if(cur > EPSILON && cur < t) {
      Point p = r.o + r.d * cur;
      if(inside_triangle(p, triangle)) {
	t = cur;
	triangle_index = i;
      }
    }
  }
  return t;
}

int inside_poly(Point point, Polyhedron poly) {
  for(unsigned int i = poly.start_index; i < poly.start_index + poly.num_faces; i++) {
    Plane plane = *(planes[i]);
    if(point.x * plane.a + point.y * plane.b + point.z * plane.c + plane.d > EPSILON) {
      return FALSE;
    }
  }
  return TRUE;
}

int inside_sphere(Point point, Sphere sphere) {
  if((point - sphere.center).len() < sphere.radius) {
    return TRUE;
  }
  return FALSE;
}

int inside_plane(Point point, Plane plane) {
  if(point.x * plane.a + point.y * plane.b + point.z * plane.c + plane.d > EPSILON) {
    return FALSE;
  }
  return TRUE;
}

int inside_obj(Point point, Object* obj) {
  switch(obj->ot) {
  case SPHERE:
    return inside_sphere(point, obj->sphere);
  case PLANE:
    return inside_plane(point, obj->plane);
  case POLYHEDRON:
    return inside_poly(point, obj->polyhedron);
  default:
    return FALSE;
  }
}

int inside_triangle(Point point, Triangle triangle) {
  Point a = triangle.a1;
  Point b = triangle.a2;
  Point c = triangle.a3;

  if(same_side(point, a, b, c) && same_side(point, b, a, c) && same_side(point, c, a, b)) {
    return TRUE;
  }

  return FALSE;
  
}

int same_side(Point point, Point a1, Point a2, Point a3) {
  vec3 cp1 = vec3::cross(a2 - a1, point -a3);
  vec3 cp2 = vec3::cross(a2 - a1, a3 - a1);
  if((cp1 * cp2) >= EPSILON) {
    return TRUE;
  }
  return FALSE;
}
