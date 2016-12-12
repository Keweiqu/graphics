#include "Ray.hpp"

Ray::Ray(){}

Ray::Ray(vec3 origin, vec3 direction): d(direction) {
  o = Point(origin);
  t = FLT_MAX;
}

Ray::Ray(vec3 origin, vec3 direction, float scalar): d(direction), t(scalar){
  o = Point(origin);
}

Ray::Ray(Point source, Point dest): o(source) {
  d = vec3::normalize(dest - source);
  t = FLT_MAX;
}
