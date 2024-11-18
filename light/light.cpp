#include "light.h"
#include "../vec/vec3_utils.h"
#include "../geometry/common_math.h"

using namespace vec3_utils;

light::light(point pos, vec3 intens): location(pos), intensity(intens) {};

bool light::is_in_shadow(point intersection_loc, const ray_intersectable& env) {
    vec3 light_dir = unit_vector(location - intersection_loc);

    ray light_ray = ray(intersection_loc, light_dir);
    double dist_to_light = (location - intersection_loc).length();

    // Check no intersection with other world objects
    intersect_history history;
    if(env.intersect(light_ray, 0, dist_to_light, history)) {
        return true;
    }
    return false;
}
const ray_intersectable& light::get_obj_in_shadow(const ray& r, const ray_intersectable& env) {
    intersect_history history;
    env.intersect(r, 0, inf, history);
    return env;
}

