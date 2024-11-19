#include "light_utils.h"
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

namespace light_utils{
    vec3 reflect(vec3 incident, vec3 normal) {
        return incident - 2 * dot(incident, normal) * normal;
    }

    // refrac_dir pass by reference so we can see if it refracts and then 
    // set refraction direction at the same time.
    bool does_refract(vec3 incident, vec3 normal, double refractive_index, vec3& refrac_dir) {
       double n1 = 1.0;  // Air
        double n2 = refractive_index;  // Your material
        double eta = n1 / n2;  // This is the ratio we need
        
        double cos_i = -dot(normal, incident);
        
        // If we're exiting the material (cos_i < 0), flip everything
        if (cos_i < 0) {
            cos_i = -cos_i;
            normal = -normal;
            // Flip the ratio since we're going from material to air
            eta = n2 / n1;
        }
        
        double sin_t2 = eta * eta * (1.0 - cos_i * cos_i);
        if (sin_t2 > 1.0) {
            return false;  // Total internal reflection
        }
        
        double cos_t = sqrt(1.0 - sin_t2);
        refrac_dir = eta * incident + (eta * cos_i - cos_t) * normal;
        refrac_dir = unit_vector(refrac_dir);
        return true;
    }

}