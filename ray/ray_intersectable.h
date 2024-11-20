#ifndef RAY_INTERSECTABLE_H
#define RAY_INTERSECTABLE_H

#include "ray.h"
#include "ray_intersect_history.h"
#include "../material/material.h"
#include <string>

class ray_intersectable {
    public:
        virtual ~ray_intersectable() = default;
        const virtual material& get_material() const = 0;
        const virtual std::string& get_name() const = 0;
        virtual void get_uv(const point& p, double& u, double& v) const = 0;
        virtual bool intersect(const ray& r, 
        double ray_inters_min, // min time the ray intersects
        double ray_inters_max, // max time the ray intersects
        intersect_history& history // history of ray intersections
        ) const = 0;
};


#endif

