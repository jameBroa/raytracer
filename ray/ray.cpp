#include "ray.h"
#include "../vec/vec3.h"
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

ray::ray() {

}

ray::ray(const point& origin, const vec3& direction) {
    org = origin;
    dir = direction;
}

const point& ray::origin() const {
    return org;
}

const vec3& ray::direction() const {
    return dir;
}

const point ray::position_at(double t) const {
    return org + (t * dir);
} 



