#include "Ray.hpp"

Ray::Ray(){}

Ray::Ray(vec3 origin, vec3 direction, float scalar): o(origin), d(direction), t(scalar){}
