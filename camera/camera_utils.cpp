#include "camera_utils.h"
#include "../vec/vec3_utils.h"
#include "../geometry/common_math.h"

using namespace vec3_utils;

namespace camera_utils{
    bool is_in_sight(point pt, ray r, const ray_intersectable& env) {
        double dist_to_pt = (r.origin() - pt).length();

        ray reverse_ray = ray(pt, -r.direction());

        intersect_history history;

        if(env.intersect(reverse_ray, 0, inf, history)){
            return false; 
        }
        return true;
    }

}