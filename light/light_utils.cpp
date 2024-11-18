#include "light_utils.h"
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

namespace light_utils{
    vec3 reflect(vec3 incident, vec3 normal) {
        return incident - 2 * dot(incident, normal) * normal;
    }

    // refrac_dir pass by reference so we can see if it refracts and then 
    // set refraction direction at the same time.
    bool does_refract(vec3 incident, vec3 normal, double eta, vec3& refrac_dir) {
        double cos_i = dot(normal, incident);
        double sin_t2 = eta*eta * (1.0-cos_i*cos_i);
        if(sin_t2 > 1.0) {
            return false;
        }
        double cos_t = std::sqrt(1.0 - sin_t2);
        refrac_dir = eta * incident +(eta*cos_i - cos_t) * normal;
    }

}