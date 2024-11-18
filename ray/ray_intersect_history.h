#ifndef RAY_INTERSECT_HIST_H
#define RAY_INTERSECT_HIST_H

#include "ray.h"
#include "../material/material.h"

class intersect_history {
    public:
        point point;
        vec3 normal;
        double time;
        bool face_of_norm; // Will have UNIT LENGTH
        material mat;

        void set_face_of_norm(const ray& r, const vec3& outward_norm);
        void set_material(const material& mat);
};

#endif