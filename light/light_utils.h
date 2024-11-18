#ifndef LIGHT_UTILS
#define LIGHT_UTILS
#include "../vec/vec3.h";

namespace light_utils{
    vec3 reflect(vec3 incident, vec3 normal);
    bool does_refract(vec3 incident, vec3 normal, double eta, vec3& refrac_dir);
}

#endif