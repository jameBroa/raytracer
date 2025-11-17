#ifndef RAY_INTERSECT_HIST_H
#define RAY_INTERSECT_HIST_H

#include "ray.h"
#include "../material/material.h"

class intersect_history {
    public:
        intersect_history() = default;
        point point;
        vector3 normal;
        double time;
        bool face_of_norm; // Will have UNIT LENGTH
        material mat;
        float u,v;

        void set_face_of_norm(const ray& r, const vector3& outward_norm);
        void set_material(const material& mat);
};

#endif