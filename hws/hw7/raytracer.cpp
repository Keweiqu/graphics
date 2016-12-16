#include "raytracer.hpp"

Ray compute_ray(float i, float j) {
  float px = width * (j / (float) nCols) - width / 2.0;
  float py = -height * (i / (float) nRows) + height / 2.0;
  float pz = -1.0;
  vec3 dest = camera.o + camera.x * px + camera.y * py + camera.z * pz;
  vec3 source = camera.o;
  vec3 direction = vec3::normalize(dest - source);
  Ray r = Ray(source, direction);
  #if DEBUG == 1
  print_ray(r);
  #endif
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
    #if DEBUG == 1
    cout << "================================Intersection=================================" << endl;
    cout << "Intersects with a sphere at object index " << i <<
    " at point (" << r.intersect_point.x << ", " << r.intersect_point.y << ", " << r.intersect_point.z << ")" << endl;
    cout << "Normal at intersection point is <" << r.intersect_normal[0] << ", " << r.intersect_normal[1] << ", " << r.intersect_normal[2] << ">" << endl;
    cout << "t = " << r.t << endl;
    cout << "=============================================================================" << endl;
    #endif
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
        #if DEBUG == 1
        cout << "=================================Intersection================================" << endl;
        cout << "Intersects with a plane at object index " << i <<
        " at point (" << r.intersect_point.x << ", " << r.intersect_point.y << ", " << r.intersect_point.z << ")" << endl;
        cout << "Normal at intersection point is <" << r.intersect_normal[0] << ", " << r.intersect_normal[1] << ", " << r.intersect_normal[2] << ">" << endl;
        cout << "t = " << r.t << endl;
        cout << "=============================================================================" << endl;
        #endif
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
    #if DEBUG == 1
    cout << "===============================Intersection==================================" << endl;
    cout << "Intersects with a polyhedron at object index " << i <<
    " at point (" << r.intersect_point.x << ", " << r.intersect_point.y << ", " << r.intersect_point.z << ")" << endl;
    cout << "Normal at intersection point is <" << r.intersect_normal[0] << ", " << r.intersect_normal[1] << ", " << r.intersect_normal[2] << ">" << endl;
    cout << "t = " << r.t << endl;
    cout << "=============================================================================" << endl;
    #endif
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
    #if DEBUG == 1
    cout << "===============================Intersection==================================" << endl;
    cout << "Intersects with a triangle mesh at object index " << i <<
    " at point (" << r.intersect_point.x << ", " << r.intersect_point.y << ", " << r.intersect_point.z << ")" << endl;
    cout << "Normal at intersection point is <" << r.intersect_normal[0] << ", " << r.intersect_normal[1] << ", " << r.intersect_normal[2] << ">" << endl;
    cout << "t = " << r.t << endl;
    cout << "=============================================================================" << endl;
    #endif
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
    #if DEBUG == 1
    cout << "===============================Reflection==================================" << endl;
    cout << "Reflected at point (" << p[0] << ", " << p[1] << ", " << p[2] << ") on object at index " << r.intersect_obj_index << endl;
    cout << "Direction of reflected ray: <" << reflect_dir[0] << ", " << reflect_dir[1] << ", " << reflect_dir[2] << ">" << endl;
    cout << "Normal of reflection: <" << normal[0] << ", " << normal[1] << ", " << normal[2] << ">" << endl;
    cout << "===========================================================================" << endl;
    #endif
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
    #if DEBUG == 1
    cout << "===============================Refraction==================================" << endl;
    cout << "Refracted at point (" << p[0] << ", " << p[1] << ", " << p[2] << ") on object at index " << r.intersect_obj_index << endl;
    cout << "Direction of refracted ray: <" << refra_dir[0] << ", " << refra_dir[1] << ", " << refra_dir[2] << ">" << endl;
    cout << "Normal of refraction: <" << normal[0] << ", " << normal[1] << ", " << normal[2] << ">" << endl;
    cout << "===========================================================================" << endl;
    #endif
  }
  cout << "==================================Color====================================" << endl;
  cout << "Local " << local.toString() << endl;
  cout << "Reflect " << reflect.toString() << endl;
  cout << "Refract " << refract.toString() << endl;
  cout << "===========================================================================" << endl;
  return local + reflect + refract;
}

int visible(Point p, Light light) {
  Ray shadow_ray = Ray(p, light.coord);
  for(unsigned int i = 0; i < objects.size(); i++) {
    if(intersect(shadow_ray) == INTERSECT) {
      if((light.coord - p).len() < shadow_ray.t) {
	return TRUE;
      }
      #if DEBUG == 1
      cout << "===============================Visibility==================================" << endl;
      cout << "Point (" << p.x << ", " << p.y << ", " << p.z << ") is in shadow" << endl;
      cout << "===========================================================================" << endl;
      #endif
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
  float val = (vec3(r.o.x, r.o.y, r.o.z) * n + plane.d) / (r.d * n) * -1.0;
  return val;

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
  //cout << "FALSE" << endl;
  return FALSE;

}

int same_side(Point P1, Point P2, Point A, Point B) {
  glm::vec3 a = glm::vec3(A.x, A.y, A.z);
  glm::vec3 b = glm::vec3(B.x, B.y, B.z);
  glm::vec3 p1 = glm::vec3(P1.x, P1.y, P1.z);
  glm::vec3 p2 = glm::vec3(P2.x, P2.y, P2.z);
  glm::vec3 cp1 = glm::normalize(glm::cross(b - a, p1 - a));
  glm::vec3 cp2 = glm::normalize(glm::cross(b - a, p2 - a));
  float dotproduct = glm::dot(cp1, cp2);
  if(dotproduct >= 0) {
    return TRUE;
  }
  return FALSE;
}
