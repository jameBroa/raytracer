#include "camera_utils.h"
#include "../vec/vec3_utils.h"
#include "../geometry/common_math.h"
#include <random> //For real distribution
using namespace vec3_utils;
using namespace std;

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
    vec3 random_in_unit_disk() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1, 1);
        vec3 p;
        do {
            p = vec3(dis(gen), dis(gen), 0);
        } while (dot(p,p) >= 1.0);
        return p;
    }


}