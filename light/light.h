#ifndef LIGHT_H
#define LIGHT_H
#include "../vec/vector3.h"
#include "../ray/ray_intersectable.h"

class light {
    public:
        point location;
        vector3 intensity;

        light(point pos, vector3 intens);

        bool is_in_shadow(point intersersection_loc, const ray_intersectable& env);

        const ray_intersectable& get_obj_in_shadow(const ray& r, const ray_intersectable& env);
};

#endif