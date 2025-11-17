#ifndef AABB_H
#define AABB_H

#include "../../vec/vector3.h"
#include "../ray.h"

// Axis aligned bounding box (using in BVH)
// Each primitive/intersectable obj. will have an AABB.
class aabb {
    public:
        point min;
        point max;
        aabb() = default;
        aabb(const point a, const point b);

        bool intersect(const ray& r, double t_min, double t_max) const;

        aabb surrounding_bbox(const aabb& box_a, const aabb& box_b) const;
};


#endif